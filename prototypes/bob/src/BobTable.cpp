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

void BobTable::prepareColumns() {
    columns.reserve(schema.size());
    for(unsigned int i = 0; i < schema.size();++i){
        columns[i] = generateColumn(schema[i]);
        columns[i] -> setColumnId(i);
    }
}

Column* BobTable :: generateColumn(Schema::DataType type) {
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

void BobTable::addRow(Row& row) {
    int initialFirstColumnSize = columns[0] -> getSize();
    for(unsigned int i = 0; i < schema.size() - 1; ++i) {
        columns[i] -> add(row, columns[i + 1] -> getSize());
    }
    columns[schema.size() - 1] -> add(row, initialFirstColumnSize);
}

void BobTable::prepareStructure() {
    prepareCrossColumnPointers();
    sortColumns();
}

void BobTable::prepareCrossColumnPointers() {
    std::vector<int> mappings[2];
    columns[0] -> createMappingFromCurrentToSortedPositions(mappings[1]);
    for(unsigned int i = 0; i < schema.size(); ++i) {
        int nextColumn = (i + 1) % schema.size();
        columns[nextColumn] -> createMappingFromCurrentToSortedPositions(mappings[i % 2]);
        columns[i] -> updateNextFieldIdsUsingMapping(mappings[(i + 1) % 2], mappings[i % 2]);
    }
}

void BobTable::sortColumns() {
    for(unsigned int i = 0; i < schema.size(); ++i) {
        columns[i] -> sort();
    }
}

Result *BobTable::select(const Query & q) {
    prepareColumnsForQuery();
    applyConstraintsToColumns(q);
    chooseMainColumn();

    return gatherResults(q);
}

Result* BobTable::gatherResults(const Query& q){
    std::list<Row*>* results = new std::list<Row*>();
    int limit = q.getLimit();

    if(mainColumnRange.left >= 0) {
        Row* row = createTableRow();

        for(int i = 0; i < mainColumnRange.length() && results -> size() < limit; ++i) {
            if(retrieveRowBeginningWith(mainColumnRange.left + i, row)) {
                results -> push_back(row);
                row = createTableRow();
            }
        }
        delete row;
    }
    return new Result(results);
}

bool BobTable::retrieveRowBeginningWith(int nextFieldId, Row* row) {
    unsigned int i;
    for(i = 0; i <= schema.size() && nextFieldId >= 0; ++i) {
        int nextColumnId = (mainColumnId + i) % schema.size();
        nextFieldId = columns[nextColumnId] -> fillRowWithValueAndGetNextFieldId(nextFieldId, row);
    }
    return i > schema.size();
}

void BobTable::prepareColumnsForQuery() {
    for(unsigned int i = 0; i < schema.size(); ++i) {
        columns[i] -> prepareColumnForQuery();
    }
}

void BobTable::applyConstraintsToColumns(const Query& q) {
    std::list<Constraint*> constraints = q.getConstraints();
    for(std::list<Constraint*>::iterator it = constraints.begin(); it != constraints.end(); it++) {
        Constraint* c = *it;
        columns[c -> getAffectedColumn()] -> addConstraint(c);
    }
}

void BobTable::chooseMainColumn() {
    mainColumnRange = columns[0] -> reduceConstraintsToRange();
    mainColumnId = 0;
    for(unsigned int i = 1; i < schema.size(); ++i) {
        Column::IndexRange candidateColumnRange = columns[i] -> reduceConstraintsToRange();
        if(candidateColumnRange.length() < mainColumnRange.length()) {
            mainColumnRange = candidateColumnRange;
            mainColumnId = i;
        }
    }
    columns[mainColumnId] -> markAsMainQueryColumn();
}

BobTable::~BobTable() {
    for(unsigned int i = 0; i < this -> schema.size(); ++i) {
        delete columns[i];
    }
}
