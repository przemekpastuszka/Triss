/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_BOBTABLE_H_
#define PROTOTYPES_BOB_SRC_BOBTABLE_H_

#include <prototypes/common/src/Table.h>
#include <prototypes/common/src/Schema.h>
#include "columns/Column.h"
#include "columns/IndexRange.h"

class BobTable : public Table {
    private:
    std::vector<Column*> columns;

    struct MainColumnInfo {
        int mainColumnId;
        IndexRange mainColumnRange;
    };

    /*** preparing structure ***/
    void prepareColumns();
    Column* generateColumn(Schema::DataType type);
    void prepareCrossColumnPointers();
    void sortColumns();

    /*** 'select' auxiliary methods ***/
    void prepareColumnsForQuery(std::vector<ColumnQueryState*>& columnStates) const;
    void applyConstraintsToColumns(const Query& q, std::vector<ColumnQueryState*>& columnStates) const;
    MainColumnInfo chooseMainColumn(std::vector<ColumnQueryState*>& columnStates)  const;
    bool retrieveRowBeginningWith(int indexOnMainColumn, Row*, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info) const;
    Result* gatherResults(const Query& q, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info) const;

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

