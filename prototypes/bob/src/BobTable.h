/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_BOBTABLE_H_
#define PROTOTYPES_BOB_SRC_BOBTABLE_H_

#include "../../common/src/Table.h"
#include "columns/Column.h"

class BobTable : public Table {
    private:
    std::vector<Column*> columns;
    bool columnsPrepared;

    void prepareCrossColumnPointers();
    void prepareColumns();

    public:
    BobTable(const Schema& schema) : Table(schema), columnsPrepared(false) {};
    virtual ~BobTable();

    void prepareStructure();
    void addRow(Row& row);

#ifdef TRISS_TEST
    template <class T>
    TypedColumn<T>* getColumn(int i) {
        return static_cast<TypedColumn<T>*>(columns[i]);
    }
#endif
};

#endif  // PROTOTYPES_BOB_SRC_BOBTABLE_H_

