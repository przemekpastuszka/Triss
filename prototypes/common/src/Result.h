/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_COMMON_SRC_RESULT_H_
#define PROTOTYPES_COMMON_SRC_RESULT_H_

#include <list>

#include "Row.h"

class Result {
    private:
    std::list<Row*> rows;
    std::list<Row*>::iterator current;

    public:
    Result(std::list<Row*>& rows);
    ~Result();
    bool hasNext();
    Row* next();
    std::list<Row*> fetchAll();
};

#endif  // PROTOTYPES_COMMON_SRC_RESULT_H_

