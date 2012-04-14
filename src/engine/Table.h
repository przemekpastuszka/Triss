/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_TABLE_H_
#define TRISS_ENGINE_SRC_TABLE_H_

#include <vector>
#include <src/common/ColumnDesc.h>
#include <src/common/Query.h>
#include <src/common/Result.h>
#include "columns/Column.h"
#include "columns/IndexRange.h"

class Table {
    private:
    std::vector<ColumnDesc> schema;
    std::vector<Column*> columns;

    struct MainColumnInfo {
        int mainColumnId;
        IndexRangeSet mainColumnRange;
    };

    /*** preparing structure ***/
    void prepareColumns();
    Column* generateColumn(Schema::DataType type);
    void prepareCrossColumnPointers();
    void makePointersSkipNullValues();
    void removeNullsFromColumns();
    void sortColumns();
    void deleteColumns();

    /*** 'select' auxiliary methods ***/
    void prepareColumnsForQuery(std::vector<ColumnQueryState*>& columnStates) const;
    void applyConstraintsToColumns(const Query& q, std::vector<ColumnQueryState*>& columnStates) const;
    MainColumnInfo chooseMainColumn(std::vector<ColumnQueryState*>& columnStates)  const;
    bool retrieveRowBeginningWith(int indexOnMainColumn, Row*, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info, bool fill) const;
    Result* gatherResults(const Query& q, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info) const;

    public:
    void setSchema(const std::vector<ColumnDesc>& schema) {
        deleteColumns();

        this -> schema = schema;
        prepareColumns();
    };

    virtual ~Table() { deleteColumns(); }

    void prepareStructure();
    void addRow(Row& row);
    Result* select(const Query& q) const;

    virtual TableRow* createTableRow() const { return new TableRow(schema); }

    friend class AbstractTableTest;
};

#endif  // TRISS_ENGINE_SRC_TABLE_H_

