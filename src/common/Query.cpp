/*
* Copyright 2012 Michał Rychlik
*/

#include "Query.h"

void Query::selectColumns(std::list<int>& columns) {
    selectedColumns = columns;
}

void Query::addConstraint(Constraint* constraint) {
    constraints.push_back(constraint);
}

Query::~Query() {
    for(std::list<Constraint *>::iterator it = constraints.begin();
        it != constraints.end(); ++it) {
        delete *it;
    }
}

void Query::limit(int limit) {
    _limit = limit;
}

