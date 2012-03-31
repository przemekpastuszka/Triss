/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_INDEX_RANGE_
#define TRISS_ENGINE_SRC_COLUMNS_INDEX_RANGE_

struct IndexRange {
    int left, right;

    IndexRange() : left(-1), right(-1) {}
    IndexRange(int l, int r) : left(l), right(r) {}

    void validate(int size) {
        if(left >= size || right < 0 || left > right) {
            left = right = -1;
        }
    }
    int length() const { return right - left + 1; }
    bool isInRange(int x) const { return left <= x && x <= right; }
};

#endif
