/*
* Copyright 2012 Przemysław Pastuszka
*/
#include <cstdlib>
#include <string>
#include "BobTable.h"
#include "../../common/src/columns/ScalarColumn.h"
#include "../../common/src/columns/ListColumn.h"

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

BobTable::BobTable(const Schema& schema) : Table(schema) {}

void BobTable::prepareStructure() {
    prepareCrossColumnPointers();

    for(unsigned int i = 0; i < schema.size(); ++i) {
        columns[i] -> sort();
    }
}


