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
        if(row.isNull(this -> columnId)) {
            this -> addNull(nextFieldId);
        }
        else {
            this -> addField(row.get<T>(this -> columnId), nextFieldId, true);
        }
    }

    int fillRowWithValueAndGetNextFieldId(int valueIndex, int startPoint, Row* row, ColumnQueryState* state, const std::vector<ColumnDesc>& schema, bool fill) const {
        if(state -> constraintRangeSet.isInRange(valueIndex) == false) {
            return -1;
        }

        if(fill) {
            for(int i = 0; i < state -> positionsInResult.size(); ++i)
            row -> set<T>(state -> positionsInResult[i], this -> fields[valueIndex].value, schema);
        }
        return this -> fields[valueIndex].nextFieldId;
    }

    ColumnQueryState* prepareColumnForQuery() const {
        return new TypedColumnQueryState<T>();
    }
};

#endif /* TRISS_ENGINE_SRC_COLUMNS_SCALARCOLUMN_H_ */
