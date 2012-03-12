/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_BOBTABLE_H_
#define PROTOTYPES_BOB_SRC_BOBTABLE_H_

#include <prototypes/common/src/Table.h>
#include <prototypes/common/src/Schema.h>
#include "columns/Column.h"
#include "columns/Column.cpp"

class BobTable : public Table {
    private:
    std::vector<Column*> columns;

    int mainColumnId;
    Column::IndexRange mainColumnRange;

    /*** preparing structure ***/
    void prepareColumns();
    Column* generateColumn(Schema::DataType type);
    void prepareCrossColumnPointers();
    void sortColumns();

    /*** 'select' auxiliary methods ***/
    void prepareColumnsForQuery();
    void applyConstraintsToColumns(const Query& q);
    void chooseMainColumn();
    bool retrieveRowBeginningWith(int indexOnMainColumn, Row*);
    Result* gatherResults(const Query& q);

    public:
    BobTable(const Schema& schema) : Table(schema) {
        prepareColumns();
    };
    virtual ~BobTable();

    void prepareStructure();
    void addRow(Row& row);
    Result* select(const Query& q);

    friend class AbstractBobTest;
};

#endif  // PROTOTYPES_BOB_SRC_BOBTABLE_H_

