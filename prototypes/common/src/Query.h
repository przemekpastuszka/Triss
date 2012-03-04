/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_COMMON_SRC_QUERY_H_
#define PROTOTYPES_COMMON_SRC_QUERY_H_

#include <algorithm>
#include <list>

#include "Constraint.h"

class Query {
    private:
    std::list<int> selectedColumns;
    std::list<Constraint*> constraints;
    int _limit;

    public:
    void selectColumns(std::list<int>& columns);
    void addConstraint(Constraint* constraint);
    ~Query();
    void limit(int limit);
    
#ifdef TRISS_TEST
    bool isColumnSelected(int i) {
        return std::find(selectedColumns.begin(), selectedColumns.end(), i) !=
               selectedColumns.end();
    }
    
    int numberOfConstraints() {
        return constraints.size();
    }
    
    int getLimit() {
        return _limit;
    }
#endif
    
};

#endif  // PROTOTYPES_COMMON_SRC_ALICE_QUERY_H_

