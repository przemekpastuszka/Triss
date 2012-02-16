#include <cstdlib>
#include "Table.h"
#include "columns/NumericalColumn.h"
#include "columns/NumericalListColumn.h"
#include "columns/StringColumn.h"


Column* generateColumn(Table::DataType type) {
    switch(type) {
        case Table::STRING:
            return new StringColumn();
        case Table::NUMERICAL:
            return new NumericalColumn();
        case Table::NUMERICAL_LIST:
            return new NumericalListColumn();
    }
}

Table::~Table() {
    for(int i = 0; i < nrOfColumns; ++i) {
        delete columns[i];
    }
    delete [] columns;
}

void Table::init(int nrOfColumns, DataType* schema, int *capacities) {
    this -> schema = schema;
    this -> nrOfColumns = nrOfColumns;

    columns = new Column*[nrOfColumns];
    for(int i = 0; i < nrOfColumns; ++i) {
        columns[i] = generateColumn(schema[i]);
        columns[i] -> init(capacities[i]);
    }
}

void Table::addRow(void **row) {
    int firstColumnSize = columns[0] -> getSize();
    for(int i = 0; i < nrOfColumns - 1; ++i) {
        columns[i] -> add(row[i], columns[i + 1] -> getSize());
    }
    columns[nrOfColumns - 1] -> add(row[nrOfColumns - 1], firstColumnSize);
}

void Table::prepareCrossColumnPointers() {
    int* currentColumnMapping = NULL;
    int *nextColumnMapping = columns[0] -> getMappingFromCurrentToSortedPositions();
    for(int i = 0; i < nrOfColumns; ++i){
        int nextColumn = (i + 1) % nrOfColumns;
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

void Table::prepareStructure() {
    prepareCrossColumnPointers();

    for(int i = 0; i < nrOfColumns; ++i) {
        columns[i] -> sort();
    }
}
