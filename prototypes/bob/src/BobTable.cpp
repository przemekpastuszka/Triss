/*
* Copyright 2012 Przemysław Pastuszka
*/
#include <cstdlib>
#include <string>
#include "BobTable.h"
#include "columns/ScalarColumn.h"
#include "columns/ListColumn.h"
#include <prototypes/common/src/ValueRange.h>
#include <prototypes/common/src/Row.h>
#include <prototypes/common/src/Schema.h>

void BobTable::prepareCrossColumnPointers() {
    int* currentColumnMapping = NULL;
    int *nextColumnMapping = columns[0] -> getMappingFromCurrentToSortedPositions();
    for(unsigned int i = 0; i < schema.size(); ++i){
        int nextColumn = (i + 1) % schema.size();
        if(currentColumnMapping){
            delete [] currentColumnMapping;
        }
        currentColumnMapping = nextColumnMapping;
        nextColumnMapping = columns[nextColumn] -> getMappingFromCurrentToSortedPositions();
        columns[i] -> updateNextFieldIdsUsingMapping(currentColumnMapping, nextColumnMapping);
    }

    delete [] currentColumnMapping;
    delete [] nextColumnMapping;
}

void BobTable::prepareStructure() {
    prepareCrossColumnPointers();

    for(unsigned int i = 0; i < schema.size(); ++i) {
        columns[i] -> sort();
    }
}

Column* generateColumn(Schema::DataType type) {
    switch(type) {
        case Schema::STRING:
            return new ScalarColumn<std::string>();
        case Schema::NUMERICAL:
            return new ScalarColumn<double>();
        case Schema::NUMERICAL_LIST:
            return new ListColumn<double>();
        case Schema::STRING_LIST:
            return new ListColumn<std::string>();
    }
    return NULL;
}

BobTable::~BobTable() {
    for(unsigned int i = 0; i < this -> schema.size(); ++i) {
        delete columns[i];
    }
}

void BobTable::prepareColumns() {
    columns.reserve(schema.size());
    for(unsigned int i = 0; i < schema.size();++i){
        columns[i] = generateColumn(schema[i]);
    }
}

void BobTable::addRow(Row& row) {
    if(columnsPrepared == false) {
        prepareColumns();
        columnsPrepared = true;
    }

    int firstColumnSize = columns[0] -> getSize();
    for(unsigned int i = 0; i < schema.size() - 1; ++i) {
        columns[i] -> add(row.getPointer(i), columns[i + 1] -> getSize());
    }
    columns[schema.size() - 1] -> add(row.getPointer(schema.size() - 1), firstColumnSize);
}

Result *BobTable::select(const Query & q) const {

    std::list<Constraint*> constraints = q.getConstraints();
    for(std::list<Constraint*>::iterator it = constraints.begin(); it != constraints.end(); it++) {
        Constraint* c = *it;
        columns[c -> getAffectedColumn()] -> addConstraint(c);
    }

    std::vector<Column::IndexRange> computedRanges(schema.size());
    for(unsigned int i = 0; i < schema.size(); ++i) {
        computedRanges[i] = columns[i] -> getRangeFromConstraints();
    }

    int minRangeLength = computedRanges[0].length();
    int minRangeIndex = 0;

    for(unsigned int i = 1; i < schema.size(); ++i) {
        if(computedRanges[i].length() < minRangeLength) {
            minRangeLength = computedRanges[i].length();
            minRangeIndex = i;
        }
    }

    int left = computedRanges[minRangeIndex].left, right = computedRanges[minRangeIndex].right;
    int limit = q.getLimit();

    std::list<Row*> result;
    if(left < 0) {
        return new Result(result);
    }

    Schema s(schema);
    Row* row = new Row(s);

    columns[minRangeIndex] -> markFieldsAsUnvisitedInRange(left, right);
    for(;left <= right && result.size() < limit; ++left) {
        if(columns[minRangeIndex] -> isFieldVisitedAt(left)) {
            continue;
        }
        bool isRowOk = true;
        int nextFieldId = columns[minRangeIndex] -> fillRowWithValueAndGetNextFieldId(left, minRangeIndex, row, true);
        for(unsigned int i = 1; i < schema.size(); ++i) {
            int nextColumnId = (minRangeIndex + i) % schema.size();
            if(computedRanges[nextColumnId].isInRange(nextFieldId)) {
                nextFieldId = columns[nextColumnId] -> fillRowWithValueAndGetNextFieldId(nextFieldId, nextColumnId, row, false);
            }
            else {
                isRowOk = false;
                break;
            }
        }
        if(isRowOk) {
            columns[minRangeIndex] -> fillRowWithValueAndGetNextFieldId(nextFieldId, minRangeIndex, row, true);
            result.push_back(row);
            row = new Row(s);
        }
    }
    delete row;

    return new Result(result);
}
