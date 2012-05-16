/*
* Copyright 2012 Michał Rychlik
*/
#ifndef TRISS_SRC_COMMON_QUERY_H_
#define TRISS_SRC_COMMON_QUERY_H_

#include <algorithm>
#include <list>
#include <climits>

#include "Constraint.h"

class Query {
    private:
    std::list<int> selectedColumns;
    std::list<Constraint*> constraints;
    unsigned int _limit;

    public:
    void selectColumns(std::list<int>& columns);
    void addConstraint(Constraint* constraint);
    Query() : _limit(INT_MAX) {}
    ~Query();
    void limit(unsigned int limit);
    
    const std::list<Constraint*>& getConstraints() const {
        return constraints;
    }
    unsigned int getLimit() const {
        return _limit;
    }
    const std::list<int>& getSelectedColumns() const {
        return selectedColumns;
    }
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version);

    friend class QueryTest;
};

#endif  // TRISS_SRC_COMMON_ALICE_QUERY_H_

