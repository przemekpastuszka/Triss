/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_COLUMN_H_
#define TRISS_ENGINE_SRC_COLUMNS_COLUMN_H_

#include <cstdlib>
#include <src/common/Constraint.h>
#include <src/common/ValueRange.h>
#include <src/common/Row.h>
#include "ColumnQueryState.h"
#include "IndexRange.h"

class Column {
    protected:
    int globalPosition;
    int columnId;

    public:
    virtual ~Column() {};
    virtual unsigned int getSize() const = 0;

    /*** preparing structure ***/
    virtual void setColumnId(int id) { columnId = id; }
    virtual void add(const Row& row, int nextFieldId) = 0;
    virtual void setGlobalPosition(int position) { globalPosition = position; }
    virtual int getGlobalPosition() { return globalPosition; }
    virtual void createMappingFromCurrentToSortedPositions(std::vector<int>& mapping) = 0;
    virtual void updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next, int indicesShift) = 0;
    virtual void sort() = 0;

    /*** 'select' auxiliary methods ***/
    virtual ColumnQueryState* prepareColumnForQuery() const = 0;
    virtual void addConstraint(Constraint* constraint, ColumnQueryState* state) const = 0;
    virtual IndexRange reduceConstraintsToRange(ColumnQueryState* state) const = 0;
    virtual void markAsMainQueryColumn(ColumnQueryState* state) const {}
    virtual int fillRowWithValueAndGetNextFieldId(int valueIndex, int startPoint, Row* row, ColumnQueryState* state, bool fill) const = 0;
};
#endif  // TRISS_ENGINE_SRC_COLUMNS_COLUMN_H_
