/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_

#include <cstdlib>
#include <prototypes/common/src/Constraint.h>
#include <prototypes/common/src/ValueRange.h>
#include <prototypes/common/src/Row.h>

class Column {
    public:
    struct IndexRange;

    virtual ~Column() {};
    virtual unsigned int getSize() const = 0;

    /*** preparing structure ***/
    virtual void setColumnId(int id) = 0;
    virtual void add(const Row& row, int nextFieldId) = 0;
    virtual void createMappingFromCurrentToSortedPositions(std::vector<int>& mapping) = 0;
    virtual void updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next) = 0;
    virtual void sort() = 0;

    /*** 'select' auxiliary methods ***/
    virtual void prepareColumnForQuery() = 0;
    virtual void addConstraint(Constraint* constraint) = 0;
    virtual IndexRange reduceConstraintsToRange() = 0;
    virtual void markAsMainQueryColumn() {}
    virtual int fillRowWithValueAndGetNextFieldId(int valueIndex, Row* row) = 0;
};

#endif  // PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_
