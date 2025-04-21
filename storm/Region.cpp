#include "storm/Region.hpp"
#include "storm/region/RGN.hpp"
#include "storm/Error.hpp"
#include "storm/Hash.hpp"
#include "storm/Thread.hpp"
#include <limits>


static TSExportTableSyncReuse<RGN, HSRGN, HLOCKEDRGN, CCritSect> s_rgntable;


void DeleteCombinedRect(TSGrowableArray<RECTF>* combinedArray, uint32_t index);
void DeleteRect(RECTF* rect);
int32_t IsNullRect(RECTF* rect);


void AddCombinedRect(TSGrowableArray<RECTF>* combinedArray, const RECTF* rect) {
    RECTF* newRect = combinedArray->New();
    *newRect = *rect;
}

void AddSourceRect(TSGrowableArray<SOURCE>* sourceArray, const RECTF* rect, void* param, int32_t sequence, uint32_t flags) {
    auto source = sourceArray->New();

    source->rect = *rect;
    source->param = param;
    source->sequence = sequence;
    source->flags = flags;
}

int32_t CheckForIntersection(const RECTF* sourceRect, const RECTF* targetRect) {
    return sourceRect->left < targetRect->right
        && sourceRect->bottom < targetRect->top
        && sourceRect->right > targetRect->left
        && sourceRect->top > targetRect->bottom;
}

void CombineRectangles(TSGrowableArray<RECTF>* combinedArray) {
    for (uint32_t i = 1; i < combinedArray->Count(); i++) {
        for (uint32_t j = 0; j < i; j++) {
            RECTF* rctA = &(*combinedArray)[i];
            RECTF* rctB = &(*combinedArray)[j];

            if (rctA->left == rctB->left && rctA->right == rctB->right) {
                if (rctA->bottom == rctB->top || rctB->bottom == rctA->top) {
                    rctA->bottom = std::min(rctB->bottom, rctA->bottom);
                    rctA->top = std::max(rctB->top, rctA->top);
                    DeleteRect(rctB);
                    break;
                }
            }
            if (rctA->left == rctB->right || rctB->left == rctA->right) {
                if (rctA->bottom == rctB->bottom && rctA->top == rctB->top) {
                    rctA->left = std::min(rctB->left, rctA->left);
                    rctA->right = std::max(rctB->right, rctA->right);
                    DeleteRect(rctB);
                    break;
                }
            }

            if (rctA->left == rctB->right || rctB->left == rctA->right) {
                if (rctA->bottom < rctB->top && rctB->bottom < rctA->top) {
                    RECTF newrect[5];

                    newrect[0] = { rctA->left, rctA->bottom, rctA->right, rctB->bottom };
                    newrect[1] = { rctB->left, rctB->bottom, rctB->right, rctA->bottom };
                    newrect[2] = { rctA->left, rctB->top, rctA->right, rctA->top };
                    newrect[3] = { rctB->left, rctA->top, rctB->right, rctB->top };

                    newrect[4] = {
                        std::min(rctB->left, rctA->left),
                        std::max(rctB->bottom, rctA->bottom),
                        std::max(rctB->right, rctA->right),
                        std::min(rctB->top, rctA->top)
                    };

                    for (uint32_t k = 0; k < 5; k++) {
                        if (!IsNullRect(&newrect[k])) {
                            AddCombinedRect(combinedArray, &newrect[k]);
                        }
                    }
                    DeleteCombinedRect(combinedArray, i);
                    DeleteCombinedRect(combinedArray, j);
                    break;
                }
            }
        }
    }
}

int32_t CompareRects(const RECTF* rect1, const RECTF* rect2) {
    return rect1->left == rect2->left
        && rect1->bottom == rect2->bottom
        && rect1->right == rect2->right
        && rect1->top == rect2->top;
}

void DeleteCombinedRect(TSGrowableArray<RECTF>* combinedArray, uint32_t index) {
    DeleteRect(&(*combinedArray)[index]);
}

void DeleteRect(RECTF* rect) {
    rect->left = std::numeric_limits<float>::max();
    rect->bottom = std::numeric_limits<float>::max();
    rect->right = std::numeric_limits<float>::max();
    rect->top = std::numeric_limits<float>::max();
}

void FragmentCombinedRectangles(TSGrowableArray<RECTF>* combinedArray, uint32_t firstIndex, uint32_t lastIndex, const RECTF* rect) {
    uint32_t index;
    RECTF* checkRect;

    for (index = firstIndex; index < lastIndex; index++) {
        checkRect = &(*combinedArray)[index];
        if (CheckForIntersection(rect, checkRect)) {
            break;
        }
    }
    if (index >= lastIndex) {
        AddCombinedRect(combinedArray, rect);
        return;
    }

    RECTF newrect[4];
    newrect[0] = { rect->left, rect->bottom, rect->right, checkRect->bottom };
    newrect[1] = { rect->left, checkRect->top, rect->right, rect->top };
    newrect[2] = {
        rect->left,
        std::max(checkRect->bottom, rect->bottom),
        checkRect->left,
        std::min(checkRect->top, rect->top)
    };
    newrect[3] = {
        checkRect->right,
        std::max(checkRect->bottom, rect->bottom),
        rect->right,
        std::min(checkRect->top, rect->top)
    };

    for (uint32_t i = 0; i < 4; i++) {
        if (!IsNullRect(&newrect[i])) {
            FragmentCombinedRectangles(combinedArray, index + 1, lastIndex, &newrect[i]);
        }
    }
}

void FragmentSourceRectangles(TSGrowableArray<SOURCE>* sourceArray, uint32_t firstIndex, uint32_t lastIndex, int32_t previousOverlap, const RECTF* rect, void* param, int32_t sequence) {
    if (firstIndex >= lastIndex) {
        AddSourceRect(sourceArray, rect, param, sequence, SF_ADDING | (previousOverlap ? SF_OVERLAPS : SF_NONE));
        return;
    }

    auto overlapsExisting = previousOverlap;

    for (uint32_t i = firstIndex; i < lastIndex; i++) {
        auto source = &(*sourceArray)[i];

        if (CheckForIntersection(rect, &source->rect)) {
            if (!CompareRects(rect, &source->rect)) {
                break;
            }

            source->flags |= SF_OVERLAPS;
            overlapsExisting = 1;
        }

        if (i + 1 == lastIndex) {
            AddSourceRect(sourceArray, rect, param, sequence, SF_ADDING | (previousOverlap ? SF_OVERLAPS : SF_NONE));
            return;
        }
    }

    // TODO
}

int32_t IsNullRect(RECTF* rect) {
    return rect->left >= rect->right || rect->bottom >= rect->top;
}

void ClearRegion(RGN* rgn) {
    rgn->source.SetCount(0);
    rgn->combined.SetCount(0);
    rgn->foundparams.SetCount(0);

    DeleteRect(&rgn->foundparamsrect);

    rgn->sequence = 0;
    rgn->dirty = 0;
}

void InvalidateRegion(RGN* rgn) {
    rgn->dirty = 1;
    DeleteRect(&rgn->foundparamsrect);
}

void DeleteSourceRect(TSGrowableArray<SOURCE>* sourceArray, uint32_t index) {
    auto source = &(*sourceArray)[index];

    DeleteRect(&source->rect);
    source->param = nullptr;
    source->sequence = -1;
    source->flags = SF_NONE;
}

void OptimizeSource(TSGrowableArray<SOURCE>* sourceArray) {
    for (uint32_t i = 0; i < sourceArray->Count(); i++) {
        auto source = &(*sourceArray)[i];

        if (IsNullRect(&source->rect)) {
            // Set current (null) element to last element
            auto last = &(*sourceArray)[sourceArray->Count() - 1];
            (*sourceArray)[i] = *last;

            // Decrement index by 1 to force null check on copied last element on next tick
            i--;

            // Shrink by 1 (to account for the removal of the null element)
            sourceArray->SetCount(sourceArray->Count() - 1);
        }
    }
}

void ProcessBooleanOperation(TSGrowableArray<SOURCE>* sourceArray, int32_t combineMode) {
    for (uint32_t i = 0; i < sourceArray->Count(); i++) {
        auto source = &(*sourceArray)[i];

        int32_t remove = 0;
        switch (combineMode) {
        case SRGN_AND:
            remove = !(source->flags & SF_OVERLAPS);
            break;
        case SRGN_XOR:
            remove = source->flags & SF_OVERLAPS;
            break;
        case SRGN_DIFF:
            remove = source->flags & (SF_ADDING | SF_OVERLAPS);
            break;
        case SRGN_COPY:
            remove = source->flags & SF_ADDING;
            break;
        }

        if (remove) {
            DeleteSourceRect(sourceArray, i);
        }

        source->flags = SF_NONE;
    }
}

int SortRectCallback(const void* elem1, const void* elem2) {
    RECTF* rct1 = (RECTF*)elem1;
    RECTF* rct2 = (RECTF*)elem2;

    double result = rct1->top == rct2->top ? rct1->left - rct2->left : rct1->top - rct2->top;

    if (result > 0.0) return 1;
    if (result < 0.0) return -1;
    return 0;
}

void ProduceCombinedRectangles(RGN* rgn) {
    rgn->combined.SetCount(0);

    uint32_t sourcerects = rgn->source.Count();
    SOURCE* sourcearray = rgn->source.Ptr();
    for (uint32_t i = 0; i < sourcerects; i++) {
        if (!(sourcearray[i].flags & SF_PARAMONLY)) {
            FragmentCombinedRectangles(&rgn->combined, 0, rgn->combined.Count(), &sourcearray[i].rect);
        }
    }

    CombineRectangles(&rgn->combined);

    std::qsort(rgn->combined.Ptr(), rgn->combined.Count(), sizeof(rgn->combined.Ptr()[0]), SortRectCallback);

    for (uint32_t i = rgn->combined.Count(); i > 0; i = rgn->combined.Count()) {
        if (!IsNullRect(&rgn->combined[i-1])) break;
        rgn->combined.SetCount(i - 1);
    }
}

void SRgnCombineRectf(HSRGN handle, RECTF* rect, void* param, int32_t combineMode) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handle);
    STORM_VALIDATE(rect);
    STORM_VALIDATE(combineMode >= 1);
    STORM_VALIDATE(combineMode <= 6);
    STORM_VALIDATE_END_VOID;

    HLOCKEDRGN lockedHandle;
    auto rgn = s_rgntable.Lock(handle, &lockedHandle, 0);

    if (rgn) {
        if (combineMode == SRGN_OR || combineMode == SRGN_PARAMONLY) {
            if (!IsNullRect(rect)) {
                rgn->sequence++;
                AddSourceRect(&rgn->source, rect, param, rgn->sequence, combineMode == SRGN_PARAMONLY ? SF_PARAMONLY : SF_NONE);
            }
        } else {
            if (!IsNullRect(rect)) {
                rgn->sequence++;
                FragmentSourceRectangles(&rgn->source, 0, rgn->source.Count(), 0, rect, param, rgn->sequence);
            }

            ProcessBooleanOperation(&rgn->source, combineMode);
            OptimizeSource(&rgn->source);
        }

        InvalidateRegion(rgn);
    }

    s_rgntable.Unlock(lockedHandle);
}

void SRgnClear(HSRGN handle) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END_VOID;

    HLOCKEDRGN lockedHandle;
    auto rgn = s_rgntable.Lock(handle, &lockedHandle, 0);

    if (rgn) {
        ClearRegion(rgn);
        s_rgntable.Unlock(lockedHandle);
    }
}

void SRgnCreate(HSRGN* handlePtr, uint32_t reserved) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handlePtr);
    STORM_VALIDATE(!reserved);
    STORM_VALIDATE_END_VOID;

    HLOCKEDRGN lockedHandle = nullptr;
    auto rgn = s_rgntable.NewLock(handlePtr, &lockedHandle);

    ClearRegion(rgn);

    s_rgntable.Unlock(lockedHandle);
}

void SRgnDelete(HSRGN handle) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handle);
    STORM_VALIDATE_END_VOID;

    s_rgntable.Delete(handle);
}

void SRgnGetBoundingRectf(HSRGN handle, RECTF* rect) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handle);
    STORM_VALIDATE(rect);
    STORM_VALIDATE_END_VOID;

    rect->left = std::numeric_limits<float>::max();
    rect->bottom = std::numeric_limits<float>::max();
    rect->right = std::numeric_limits<float>::min();
    rect->top = std::numeric_limits<float>::min();

    HLOCKEDRGN lockedHandle;
    auto rgn = s_rgntable.Lock(handle, &lockedHandle, 0);

    if (!rgn) {
        s_rgntable.Unlock(lockedHandle);
        return;
    }

    for (uint32_t i = 0; i < rgn->source.Count(); i++) {
        auto source = &rgn->source[i];

        if (!(source->flags & SF_PARAMONLY)) {
            rect->left = std::min(source->rect.left, rect->left);
            rect->bottom = std::min(source->rect.bottom, rect->bottom);
            rect->right = std::max(source->rect.right, rect->right);
            rect->top = std::max(source->rect.top, rect->top);
        }
    }

    s_rgntable.Unlock(lockedHandle);

    if (IsNullRect(rect)) {
        rect->left = 0.0f;
        rect->bottom = 0.0f;
        rect->right = 0.0f;
        rect->top = 0.0f;
    }
}

void SRgnGetRectsf(HSRGN handle, uint32_t* numrects, RECTF* buffer) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(handle);
    STORM_VALIDATE(numrects);
    STORM_VALIDATE_END_VOID;

    HLOCKEDRGN lockedHandle;
    auto rgn = s_rgntable.Lock(handle, &lockedHandle, 0);
    if (!rgn) {
        *numrects = 0;
        return;
    }

    if (rgn->dirty) {
        ProduceCombinedRectangles(rgn);
        rgn->dirty = 0;
    }

    if (buffer) {
        *numrects = std::min(*numrects, rgn->combined.Count());
        memcpy(buffer, rgn->combined.Ptr(), sizeof(rgn->combined.Ptr()[0]) * *numrects);
    }
    else {
        *numrects = rgn->combined.Count();
    }

    s_rgntable.Unlock(lockedHandle);
}
