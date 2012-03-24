/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_ALICE_SRC_ALICETABLE_H_
#define PROTOTYPES_ALICE_SRC_ALICETABLE_H_

#include <prototypes/common/src/Table.h>
//#include <prototypes/common/src/Schema.h>
//#include <prototypes/common/src/Row.h>
#include "columns/Column.h"
#include "columns/Fields.h"

class AliceTable : public Table {
    private:
        std::vector<Column*> columns;
        std::vector<Row*> rows;
    public:
        AliceTable(const Schema& schema);
        void prepareStructure();
        void addRow(Row& row);
        Result* select(const Query& q);
        ~AliceTable();
};

#endif  // PROTOTYPES_ALICE_SRC_ALICETABLE_H_

