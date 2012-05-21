/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_INDEX_RANGE_
#define TRISS_ENGINE_SRC_COLUMNS_INDEX_RANGE_

#include <vector>

struct IndexRange {
    int left, right;

    IndexRange() : left(-1), right(-1) {}
    IndexRange(int l, int r) : left(l), right(r) {}

    bool validate(int size) const {
        if(left >= size || right < 0 || left > right) {
            return false;
        }
        return true;
    }
    int length() const { return right - left + 1; }
    bool isInRange(int x) const { return left <= x && x <= right; }
};

struct IndexRangeSet {
    std::vector<IndexRange> ranges;
    
    int length() const {
        int length = 0;
        for(int i = 0; i < ranges.size(); ++i) {
            length += ranges[i].length();
        }
        return length;
    }
    
    bool isInRange(int x) const {
        for(int i = 0; i < ranges.size(); ++i) {
            if(ranges[i].isInRange(x)) {
                return true;
            }
        }
        return false;
    }
};

#endif
