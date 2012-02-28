/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_COMMON_SRC_BOBTABLE_H_
#define PROTOTYPES_COMMON_SRC_BOBTABLE_H_

#include "../../common/src/Table.h"

class BobTable : public Table {
    private:
    void prepareCrossColumnPointers();

    public:
    BobTable(const Schema& schema);
    void prepareStructure();
};

#endif  // PROTOTYPES_COMMON_SRC_BOBTABLE_H_

