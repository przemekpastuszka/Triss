/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_BOBTABLE_H_
#define PROTOTYPES_BOB_SRC_BOBTABLE_H_

#include <prototypes/common/src/Table.h>
#include "columns/Column.h"

class BobTable : public Table {
    private:
    std::vector<Column*> columns;

    int mainColumnId;
    Column::IndexRange mainColumnRange;

    void prepareCrossColumnPointers();
    void prepareColumns();
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

#ifdef TRISS_TEST
    template <class T>
    TypedColumn<T>* getColumn(int i) {
        return static_cast<TypedColumn<T>*>(columns[i]);
    }
#endif
};

#endif  // PROTOTYPES_BOB_SRC_BOBTABLE_H_

