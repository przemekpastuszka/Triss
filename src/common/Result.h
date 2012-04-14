/*
* Copyright 2012 Michał Rychlik
*/
#ifndef TRISS_SRC_COMMON_RESULT_H_
#define TRISS_SRC_COMMON_RESULT_H_

#include <list>
#include <vector>

#include "Row.h"
#include "ColumnDesc.h"


class Result {
    private:
    std::vector<ColumnDesc> schema;
    std::list<Row*>* rows;
    std::list<Row*>::iterator current;

    public:
    Result(const std::vector<ColumnDesc>& schema, std::list<Row*>* rows);
    ~Result();
    bool hasNext();
    Row* next();
    std::list<Row*>* fetchAll();
};

#endif  // TRISS_SRC_COMMON_RESULT_H_

