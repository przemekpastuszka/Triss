/*
* Copyright 2012 Przemysław Pastuszka
*/
#include <cstdlib>
#include <string>
#include "BobTable.h"
#include "columns/ScalarColumn.h"
#include "columns/ListColumn.h"
#include <prototypes/common/src/ValueRange.h>

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
        columns[i] -> add(row.get(i), columns[i + 1] -> getSize());
    }
    columns[schema.size() - 1] -> add(row.get(schema.size() - 1), firstColumnSize);
}

Result *BobTable::select(const Query & q) const {
    std::list<Row*> ls;
    return new Result(ls);
}
