/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_SCALARCOLUMN_H_
#define TRISS_ENGINE_SRC_COLUMNS_SCALARCOLUMN_H_

#include <algorithm>
#include <vector>
#include "TypedColumn.h"

template <class T>
class ScalarColumn : public TypedColumn<T> {
    public:
    void updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next, int indicesShift) {
        for(unsigned int i = 0; i < this -> fields.size(); ++i) {
            this -> fields[i].nextFieldId = next[this -> fields[i].nextFieldId] + indicesShift;
        }
    }

    void add(const Row& row, int nextFieldId) {
        addField(row.get<T>(this -> columnId), nextFieldId, true);
    }

    int fillRowWithValueAndGetNextFieldId(int valueIndex, Row* row, ColumnQueryState* state, bool fill) const {
        valueIndex -= this -> globalPosition;

        if(state -> constraintRange.isInRange(valueIndex) == false) {
            return -1;
        }

        if(fill) {
            row -> set<T>(this -> columnId, this -> fields[valueIndex].value);
        }
        return this -> fields[valueIndex].nextFieldId;
    }

    ColumnQueryState* prepareColumnForQuery() const {
        return new TypedColumnQueryState<T>();
    }
};

#endif /* TRISS_ENGINE_SRC_COLUMNS_SCALARCOLUMN_H_ */