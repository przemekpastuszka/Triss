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
    void setupResultSchema(std::vector<ColumnDesc>& resultSchema, const Query& q) const;
    void setupPositionsInResultForEachColumn(std::vector<ColumnQueryState*>& columnState, const Query& q) const;
    bool retrieveRowBeginningWith(int indexOnMainColumn, Row*, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info, bool fill, std::vector<ColumnDesc>& resultSchema) const;
    Result* gatherResults(const Query& q, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info) const;

    public:
    void setSchema(const std::vector<ColumnDesc>& schema) {
        deleteColumns();

        this -> schema = schema;
        prepareColumns();
    };

    virtual ~Table() { deleteColumns(); }

    void prepareStructure();
    void addRow(TableRow& row);
    Result* select(const Query& q) const;

    virtual TableRow* createTableRow() const { return new TableRow(schema); }

    friend class AbstractTableTest;
    friend class DataBase;
    /*** For serialization purposes ***/
    private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & schema;
        ar & columns;
    }
};

#endif  // TRISS_ENGINE_SRC_TABLE_H_

