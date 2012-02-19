/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_TABLE_H_
#define PROTOTYPES_BOB_SRC_TABLE_H_

#include <string>
#include <list>
#include <algorithm>
#include "columns/Column.h"
#include "Schema.h"
#include "Row.h"

class Table {
    private:
    std::vector<Schema::DataType> schema;
    std::vector<Column*> columns;

    void prepareCrossColumnPointers();

    public:
    Table(const Schema& schema);
    ~Table();

    void addRow(Row& row);
    void prepareStructure();
    int getNrOfColumns() const {
        return schema.size();
    }

#ifdef TRISS_TEST
    template <class T>
    TypedColumn<T>* getColumn(int i) {
        return static_cast<TypedColumn<T>*>(columns[i]);
    }
#endif
};

#endif  // PROTOTYPES_BOB_SRC_TABLE_H_
