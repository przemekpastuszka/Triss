/*
* Copyright 2012 Przemysław Pastuszka
*/

#ifndef TRISS_ENGINE_SRC_COLUMNS_COLUMNQUERYSTATE_H_
#define TRISS_ENGINE_SRC_COLUMNS_COLUMNQUERYSTATE_H_

#include "IndexRange.h"
#include <triss/src/common/ValueRangeSet.h>

class ColumnQueryState {
    public:
    IndexRangeSet constraintRangeSet;
    std::vector<int> positionsInResult;

    virtual bool hasAnyConstraint() const = 0;
    virtual ~ColumnQueryState() {}
};

template <class T>
class TypedColumnQueryState : public ColumnQueryState {
    public:
    TypedColumnQueryState() : valueRangeSet(NULL) {};
    ~TypedColumnQueryState() { deleteValueRange(); }

    ValueRangeSet<T>* valueRangeSet;

    bool hasAnyConstraint() const {
        return valueRangeSet != NULL && valueRangeSet -> hasAnyConstraint();
    }

    void deleteValueRange() {
        if(valueRangeSet != NULL) { delete valueRangeSet; }
        valueRangeSet = NULL;
    }
};

template <class T>
class TypedListColumnQueryState : public TypedColumnQueryState<T> {
    public:
    bool isMainColumn;
};
#endif /* TRISS_ENGINE_SRC_COLUMNS_COLUMNQUERYSTATE_H_ */
