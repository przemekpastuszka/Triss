/*
* Copyright 2012 Przemysław Pastuszka
*/
#include <cstdlib>
#include <string>
#include "Table.h"
#include "columns/ScalarColumn.h"
#include "columns/ListColumn.h"

Column* generateColumn(Schema::DataType type) {
    switch(type) {
        case Schema::STRING:
            return new ScalarColumn<std::string>();
        case Schema::NUMERICAL:
            return new ScalarColumn<double>();
        case Schema::NUMERICAL_LIST:
            return new ListColumn<double>();
        case Schema::STRING_LIST:
            return new ListColumn<std::string>();
    }
}

Table::~Table() {
    for(unsigned int i = 0; i < this -> schema.size(); ++i) {
        delete columns[i];
    }
}

Table::Table(const Schema& schema) {
    this -> schema = schema.schema;

    columns.reserve(this -> schema.size());
    for(unsigned int i = 0; i < this -> schema.size(); ++i) {
        columns[i] = generateColumn(this -> schema[i]);
    }
}

void Table::addRow(Row& row) {
    int firstColumnSize = columns[0] -> getSize();
    for(unsigned int i = 0; i < schema.size() - 1; ++i) {
        columns[i] -> add(row.get(i), columns[i + 1] -> getSize());
    }
    columns[schema.size() - 1] -> add(row.get(schema.size() - 1), firstColumnSize);
}

