#include "Event.hpp"

#include <cstdint>

#include "list/TSList.hpp"
#include "thread/CCritSect.hpp"
#include "Atomic.hpp"
#include "Error.hpp"
#include "Memory.hpp"


struct BREAKCMD : public TSLinkedNode<BREAKCMD> {
    void* data;
};

static CCritSect s_critsect;
static TSList<BREAKCMD, TSGetLink<BREAKCMD>> s_breakcmdlist;
static int32_t s_modified;
static ATOMIC32 s_dispatchesinprogress;


struct _IDHASHENTRY {
    uint32_t id;
    uint32_t sequence;
    SEVTHANDLER handler;
    _IDHASHENTRY* next;
};

struct _IDHASHTABLE {
    _IDHASHENTRY** data;
    uint32_t size;
    uint32_t used;
    _IDHASHTABLE* next;
};

struct _TYPEHASHENTRY {
    uint32_t type;
    uint32_t subtype;
    uint32_t sequence;
    _IDHASHTABLE* idhashtable;
    _TYPEHASHENTRY* next;
};

static _TYPEHASHENTRY **s_typehashtable;
static uint32_t s_typehashtablesize;
static uint32_t s_typehashtableused;

// Fake function to make tests not bleed into each other
void SEvtCleanExtraDataForTests() {
    s_breakcmdlist.Clear();
}

void DeleteIdHashTable(_IDHASHTABLE* pTable) {
    for (uint32_t i = 0; i < pTable->size; i++) {
        for (_IDHASHENTRY* pEntry = pTable->data[i]; pEntry; pEntry = pTable->data[i]) {
            pTable->data[i] = pEntry->next;
            delete pEntry;
        }
    }
    STORM_FREE(pTable->data);
    delete pTable;
}

_TYPEHASHENTRY* FindTypeHashEntry(uint32_t type, uint32_t subtype) {
    if (!s_typehashtable || s_typehashtablesize == 0) {
        return nullptr;
    }

    for (_TYPEHASHENTRY* pEntry = s_typehashtable[(s_typehashtablesize - 1) & (subtype ^ type)]; pEntry; pEntry = pEntry->next) {
        if (pEntry->type == type && pEntry->subtype == subtype) {
            return pEntry;
        }
    }
    return nullptr;
}

uint32_t ComputeNewTableSize(uint32_t size) {
    uint32_t result = 1;
    while (result <= size * 2 + 2) {
        result *= 2;
    }
    return result;
}

void CopyIdHashTable(_IDHASHTABLE *dest, _IDHASHTABLE *source) {
    dest->size = source->size;
    dest->used = source->used;
    dest->data = (_IDHASHENTRY**)STORM_ALLOC(sizeof(_IDHASHENTRY*) * dest->size);

    for (uint32_t i = 0; i < source->size; i++) {
        _IDHASHENTRY* pSourceData = source->data[i];
        _IDHASHENTRY** ppDestData = &dest->data[i];
        for (; pSourceData; pSourceData = pSourceData->next) {
            _IDHASHENTRY* pNewEntry = STORM_NEW(_IDHASHENTRY);

            *ppDestData = pNewEntry;
            *pNewEntry = *pSourceData;
            ppDestData = &(*ppDestData)->next;
        }
        *ppDestData = nullptr;
    }
}

int32_t STORMAPI SEvtBreakHandlerChain(void* data) {
    s_critsect.Enter();
    s_breakcmdlist.NewNode(2, 0, 0)->data = data;
    s_critsect.Leave();
    return 1;
}

int32_t STORMAPI SEvtDestroy() {
    s_critsect.Enter();

    for (uint32_t i = 0; i < s_typehashtablesize; i++) {
        for (_TYPEHASHENTRY* pTypeEntry = s_typehashtable[i]; pTypeEntry; pTypeEntry = s_typehashtable[i]) {
            for (_IDHASHTABLE* pTable = pTypeEntry->idhashtable; pTable; pTable = pTypeEntry->idhashtable) {
                pTypeEntry->idhashtable = pTable->next;
                DeleteIdHashTable(pTable);
            }
            s_typehashtable[i] = pTypeEntry->next;
            delete pTypeEntry;
        }
    }

    if (s_typehashtable) {
        delete s_typehashtable;
    }
    s_typehashtable = nullptr;
    s_typehashtablesize = 0;
    s_typehashtableused = 0;

    s_modified = 1;

    s_critsect.Leave();
    return 1;
}

int32_t STORMAPI SEvtDispatch(uint32_t type, uint32_t subtype, uint32_t id, void* data) {
    SInterlockedIncrement(&s_dispatchesinprogress);

    int32_t success = 0;
    uint32_t currsequence = -1;
    _IDHASHENTRY *currptr = nullptr;

    do {
        s_critsect.Enter();

        bool breakcmd = false;
        for (BREAKCMD* curr = s_breakcmdlist.Head(); reinterpret_cast<intptr_t>(curr) > 0; curr = s_breakcmdlist.RawNext(curr)) {
            if (curr->data == data) {
                breakcmd = true;
                s_breakcmdlist.DeleteNode(curr);
                break;
            }
        }
        if (breakcmd) {
            s_critsect.Leave();
            break;
        }

        if (!currptr || s_modified) {
            currptr = nullptr;
            auto typeentry = FindTypeHashEntry(type, subtype);
            if (typeentry) {
                _IDHASHTABLE* idhash = typeentry->idhashtable;
                if (idhash->data && idhash->size != 0) {
                    for (currptr = idhash->data[id & (idhash->size - 1)]; currptr; currptr = currptr->next) {
                        if (currptr->id == id && currptr->sequence < currsequence) {
                            break;
                        }
                    }

                    if (s_dispatchesinprogress == 1) {
                        s_modified = 0;
                    }
                }
            }
        }

        SEVTHANDLER handler = nullptr;

        if (currptr) {
            handler = currptr->handler;
            currsequence = currptr->sequence;
            do {
                currptr = currptr->next;
            } while (currptr && currptr->id != id);
        }

        s_critsect.Leave();

        if (handler) {
            success = 1;
            handler(data);
        }
    } while(currptr);
    
    SInterlockedDecrement(&s_dispatchesinprogress);

    if (s_breakcmdlist.Head()) {
        s_critsect.Enter();

        for (BREAKCMD* curr = s_breakcmdlist.Head(); reinterpret_cast<intptr_t>(curr) > 0;) {
            if (curr->data == data) {
                curr = s_breakcmdlist.DeleteNode(curr);
            }
            else {
                curr = s_breakcmdlist.RawNext(curr);
            }
        }

        s_critsect.Leave();
    }
    return success;
}

int32_t STORMAPI SEvtPopState(uint32_t type, uint32_t subtype) {
    int32_t success = 0;
    s_critsect.Enter();

    _TYPEHASHENTRY* typeentry = FindTypeHashEntry(type, subtype);
    if (typeentry) {
        _IDHASHTABLE *next = typeentry->idhashtable->next;
        if (next) {
            DeleteIdHashTable(typeentry->idhashtable);
            typeentry->idhashtable = next;
        }
        else {
            // This WILL hang if called without recursive CCritSect.
            // Provide a hack since it never gets called in WoW anyway.
            // It also doesn't get called by SC for that matter. Nothing calls it. Classic.
            #if !defined(WHOA_STORM_C_CRIT_SECT_RECURSIVE)
                s_critsect.Leave();
                success = SEvtUnregisterType(type, subtype);
                s_critsect.Enter();
            #else
                success = SEvtUnregisterType(type, subtype);
            #endif
        }
    }

    s_modified = 1;
    s_critsect.Leave();
    return success;
}

int32_t STORMAPI SEvtPushState(uint32_t type, uint32_t subtype) {
    int32_t success = 0;
    s_critsect.Enter();

    _TYPEHASHENTRY* pTypeHash = FindTypeHashEntry(type, subtype);
    if (pTypeHash) {
        _IDHASHTABLE* pNewTable = STORM_NEW(_IDHASHTABLE);

        CopyIdHashTable(pNewTable, pTypeHash->idhashtable);
        pNewTable->next = pTypeHash->idhashtable;
        pTypeHash->idhashtable = pNewTable;

        success = 1;
        s_modified = 1;
    }

    s_critsect.Leave();
    return success;
}

int32_t STORMAPI SEvtRegisterHandler(uint32_t type, uint32_t subtype, uint32_t id, uint32_t flags, SEVTHANDLER handler) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handler);
    STORM_VALIDATE(!flags);
    STORM_VALIDATE_END;

    s_critsect.Enter();

    _TYPEHASHENTRY* pTypeHash = FindTypeHashEntry(type, subtype);
    if (!pTypeHash) {
        if (s_typehashtableused >= s_typehashtablesize / 2) {
            uint32_t newsize = ComputeNewTableSize(s_typehashtableused);
            _TYPEHASHENTRY** pNewTable = static_cast<_TYPEHASHENTRY**>(STORM_ALLOC_ZERO(sizeof(_TYPEHASHENTRY*) * newsize));

            if (s_typehashtable) {
                for (uint32_t i = 0; i < s_typehashtablesize; i++) {
                    _TYPEHASHENTRY* pNext;
                    for (_TYPEHASHENTRY* pTable = s_typehashtable[i]; pTable; pTable = pNext) {
                        pNext = pTable->next;

                        uint32_t idx = (newsize - 1) & (pTable->type ^ pTable->subtype);
                        pTable->next = pNewTable[idx];
                        pNewTable[idx] = pTable;
                    }
                }
                if (s_typehashtable) {
                    STORM_FREE(s_typehashtable);
                }
            }
            s_typehashtable = pNewTable;
            s_typehashtablesize = newsize;
        }

        uint32_t idx = (s_typehashtablesize - 1) & (type ^ subtype);

        _TYPEHASHENTRY* pNewTypeHash = STORM_NEW_ZERO(_TYPEHASHENTRY);

        pNewTypeHash->type = type;
        pNewTypeHash->subtype = subtype;
        pNewTypeHash->idhashtable = STORM_NEW_ZERO(_IDHASHTABLE);
        pNewTypeHash->next = s_typehashtable[idx];
        
        s_typehashtable[idx] = pNewTypeHash;
        s_typehashtableused++;

        pTypeHash = pNewTypeHash;
    }

    if (pTypeHash->idhashtable->used >= pTypeHash->idhashtable->size / 2) {
        uint32_t newsize = ComputeNewTableSize(pTypeHash->idhashtable->size);
        _IDHASHENTRY** pNewTable = static_cast<_IDHASHENTRY**>(STORM_ALLOC_ZERO(sizeof(_IDHASHENTRY*) * newsize));
        _IDHASHENTRY*** pTempTable = static_cast<_IDHASHENTRY***>(STORM_ALLOC_ZERO(sizeof(_IDHASHENTRY*) * newsize));

        for (uint32_t i = 0; i < newsize; i++) {
            pTempTable[i] = &pNewTable[i];
        }
        
        if (pTypeHash->idhashtable->data && pTypeHash->idhashtable->size != 0) {
            for (uint32_t i = 0; i < pTypeHash->idhashtable->size; i++) {
                _IDHASHENTRY* pNext;
                for (_IDHASHENTRY* pEntry = pTypeHash->idhashtable->data[i]; pEntry; pEntry = pNext) {
                    uint32_t idx = (newsize - 1) & pEntry->id;
                    pNext = pEntry->next;
                    pEntry->next = nullptr;

                    *pTempTable[idx] = pEntry;
                    pTempTable[idx] = &pEntry->next;
                }
            }
        }

        STORM_FREE(pTempTable);
        if (pTypeHash->idhashtable->data) {
            STORM_FREE(pTypeHash->idhashtable->data);
        }
        pTypeHash->idhashtable->data = pNewTable;
        pTypeHash->idhashtable->size = newsize;
    }
    
    uint32_t idx = (pTypeHash->idhashtable->size - 1) & id;
    _IDHASHENTRY* pNewIdHash = STORM_NEW_ZERO(_IDHASHENTRY);

    pNewIdHash->id = id;
    pNewIdHash->sequence = ++pTypeHash->sequence;
    pNewIdHash->handler = handler;
    
    pNewIdHash->next = pTypeHash->idhashtable->data[idx];
    pTypeHash->idhashtable->data[idx] = pNewIdHash;

    pTypeHash->idhashtable->used++;

    s_modified = 1;
    s_critsect.Leave();
    return 1;
}

int32_t STORMAPI SEvtUnregisterHandler(uint32_t type, uint32_t subtype, uint32_t id, SEVTHANDLER handler) {
    int32_t success = 0;
    s_critsect.Enter();

    _TYPEHASHENTRY* pTypeEntry = FindTypeHashEntry(type, subtype);
    if (pTypeEntry) {
        _IDHASHTABLE* pTable = pTypeEntry->idhashtable;
        if (pTable->data && pTable->size != 0) {
            _IDHASHENTRY** ppNextEntry = &pTable->data[id & (pTable->size - 1)];
            for (_IDHASHENTRY* pEntry = *ppNextEntry; pEntry; pEntry = *ppNextEntry) {
                if (pEntry->id == id && (!handler || pEntry->handler == handler)) {
                    *ppNextEntry = pEntry->next;
                    delete pEntry;
                    
                    success = 1;
                    s_modified = 1;
                    pTable->used--;
                }
                else {
                    ppNextEntry = &pEntry->next;
                }
            }
        }
    }

    s_critsect.Leave();
    return success;
}

int32_t STORMAPI SEvtUnregisterType(uint32_t type, uint32_t subtype) {
    int32_t success = 0;
    s_critsect.Enter();

    _TYPEHASHENTRY *pTypeEntry = FindTypeHashEntry(type, subtype);
    if (pTypeEntry) {
        for (auto pTable = pTypeEntry->idhashtable; pTable; pTable = pTypeEntry->idhashtable) {
            pTypeEntry->idhashtable = pTable->next;
            DeleteIdHashTable(pTable);
        }
        
        uint32_t idx = (s_typehashtablesize - 1) & (subtype ^ type);
        _TYPEHASHENTRY** ppNextEntry = &s_typehashtable[idx];
        for (_TYPEHASHENTRY* pEntry = *ppNextEntry; pEntry; pEntry = *ppNextEntry) {
            if (pEntry == pTypeEntry) {
                *ppNextEntry = pEntry->next;
                delete pEntry;
                s_typehashtableused--;
            }
            else {
                ppNextEntry = &pEntry->next;
            }
        }
        
        success = 1;
        s_modified = 1;
    }

    s_critsect.Leave();
    return success;
}
