#include "storm/Region.hpp"
#include "storm/region/RGN.hpp"
#include "storm/Error.hpp"
#include "storm/Hash.hpp"
#include "storm/Thread.hpp"
#include <limits>

static TSExportTableSyncReuse<RGN, HSRGN, HLOCKEDRGN, CCritSect> s_rgntable;

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

int32_t CompareRects(const RECTF* rect1, const RECTF* rect2) {
    return rect1->left == rect2->left
        && rect1->bottom == rect2->bottom
        && rect1->right == rect2->right
        && rect1->top == rect2->top;
}

void DeleteRect(RECTF* rect) {
    rect->left = std::numeric_limits<float>::max();
    rect->bottom = std::numeric_limits<float>::max();
    rect->right = std::numeric_limits<float>::max();
    rect->top = std::numeric_limits<float>::max();
}

void FragmentSourceRectangles(TSGrowableArray<SOURCE>* sourceArray, uint32_t firstIndex, uint32_t lastIndex, int32_t previousOverlap, const RECTF* rect, void* param, int32_t sequence) {
    if (firstIndex >= lastIndex) {
        AddSourceRect(sourceArray, rect, param, sequence, 0x1 | (previousOverlap ? 0x2 : 0x0));
        return;
    }

    auto overlapsExisting = previousOverlap;

    for (uint32_t i = firstIndex; i < lastIndex; i++) {
        auto source = &(*sourceArray)[i];

        if (CheckForIntersection(rect, &source->rect)) {
            if (!CompareRects(rect, &source->rect)) {
                break;
            }

            source->flags |= 0x2;
            overlapsExisting = 1;
        }

        if (i + 1 == lastIndex) {
            AddSourceRect(sourceArray, rect, param, sequence, 0x1 | (previousOverlap  ? 0x2 : 0x0));
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
    source->flags = 0x0;
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

        switch (combineMode) {
        case 1: {
            if ((~source->flags >> 1) & 0x1) {
                DeleteSourceRect(sourceArray, i);
            }
        }

        case 3: {
            if (source->flags & 0x2) {
                DeleteSourceRect(sourceArray, i);
            }
        }

        case 4: {
            if (source->flags & (0x1 | 0x2)) {
                DeleteSourceRect(sourceArray, i);
            }
        }

        case 5: {
            if (source->flags & 0x1) {
                DeleteSourceRect(sourceArray, i);
            }
        }

        default:
            break;
        }

        source->flags = 0x0;
    }
}

void SRgnCombineRectf(HSRGN handle, RECTF* rect, void* param, int32_t combineMode) {
    STORM_ASSERT(handle);
    STORM_ASSERT(rect);
    STORM_ASSERT(combineMode >= 1);
    STORM_ASSERT(combineMode <= 6);

    HLOCKEDRGN lockedHandle;
    auto rgn = s_rgntable.Lock(handle, &lockedHandle, 0);

    if (rgn) {
        if (combineMode == 2 || combineMode == 6) {
            if (!IsNullRect(rect)) {
                rgn->sequence++;
                AddSourceRect(&rgn->source, rect, param, rgn->sequence, combineMode == 6 ? 0x10000 : 0x0);
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

void SRgnCreate(HSRGN* handlePtr, uint32_t reserved) {
    STORM_ASSERT(handlePtr);
    STORM_ASSERT(!reserved);

    HLOCKEDRGN lockedHandle = nullptr;
    auto rgn = s_rgntable.NewLock(handlePtr, &lockedHandle);

    ClearRegion(rgn);

    s_rgntable.Unlock(lockedHandle);
}

void SRgnDelete(HSRGN handle) {
    STORM_ASSERT(handle);

    s_rgntable.Delete(handle);
}

void SRgnGetBoundingRectf(HSRGN handle, RECTF* rect) {
    STORM_ASSERT(handle);
    STORM_ASSERT(rect);

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

        if (!(source->flags & 0x10000)) {
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
