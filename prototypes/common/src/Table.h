/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_COMMON_SRC_TABLE_H_
#define PROTOTYPES_COMMON_SRC_TABLE_H_

#include <string>
#include <list>
#include <algorithm>
#include "columns/Column.h"
#include "Schema.h"
#include "Row.h"

class Table {
    protected:
    std::vector<Schema::DataType> schema;
    std::vector<Column*> columns;

    public:
    Table(const Schema& schema);
    ~Table();

    void addRow(Row& row);
    virtual void prepareStructure() = 0;
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

#endif  // PROTOTYPES_COMMON_SRC_TABLE_H_
