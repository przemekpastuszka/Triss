#ifndef PROTOTYPES_BOB_SRC_TABLE_H_
#define PROTOTYPES_BOB_SRC_TABLE_H_

#include "columns/Column.h"

class Table {
    public:
    enum DataType { NUMERICAL, STRING, NUMERICAL_LIST };

    private:
    DataType* schema;
    Column** columns;
    int nrOfColumns;

    public:
    Table(int nrOfColumns, DataType* schema);
    ~Table();

    void initCapacities(int* capacities);
    void addRow(void** row);
    void prepareStructure();

    int getNrOfColumns() { return nrOfColumns; }

#ifdef TRISS_TEST
    Column** getColumns() { return columns; }
#endif
};

#endif  // PROTOTYPES_BOB_SRC_TABLE_H_
