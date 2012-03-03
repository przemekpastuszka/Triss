/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_COMMON_SRC_TABLE_H_
#define PROTOTYPES_COMMON_SRC_TABLE_H_

#include <string>
#include <list>
#include <algorithm>
#include "Schema.h"
#include "Row.h"

class Table {
    protected:
    std::vector<Schema::DataType> schema;

    public:
    Table(const Schema& schema) {
        this -> schema = schema.schema;
    }

    virtual unsigned int getNrOfColumns() const {
        return schema.size();
    }
    virtual void prepareStructure() = 0;
    virtual void addRow(Row& row) = 0;
};

#endif  // PROTOTYPES_COMMON_SRC_TABLE_H_
