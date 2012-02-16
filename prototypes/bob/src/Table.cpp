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

Table::Table(int nrOfColumns, DataType *schema) {
    this -> schema = schema;
    this -> nrOfColumns = nrOfColumns;

    columns = new Column*[nrOfColumns];
    for(int i = 0; i < nrOfColumns; ++i) {
        columns[i] = generateColumn(schema[i]);
    }
}

Table::~Table() {
    for(int i = 0; i < nrOfColumns; ++i) {
        delete columns[i];
    }
    delete [] columns;
}

void Table::initCapacities(int *capacities) {
    for(int i = 0; i < nrOfColumns; ++i) {
        columns[i] -> init(capacities[i]);
    }
}

void Table::addRow(void **row) {
    // TODO
}

void Table::prepareStructure(){
    // TODO
}
