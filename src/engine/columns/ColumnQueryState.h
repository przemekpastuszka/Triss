/*
* Copyright 2012 Przemysław Pastuszka
*/

#ifndef TRISS_ENGINE_SRC_COLUMNS_COLUMNQUERYSTATE_H_
#define TRISS_ENGINE_SRC_COLUMNS_COLUMNQUERYSTATE_H_

#include "IndexRange.h"
#include <src/common/ValueRange.h>

class ColumnQueryState {
    public:
    IndexRange constraintRange;

    virtual ~ColumnQueryState() {}
};

template <class T>
class TypedColumnQueryState : public ColumnQueryState {
    public:
    TypedColumnQueryState() : valueRange(NULL) {};
    ~TypedColumnQueryState() { deleteValueRange(); }

    ValueRange<T>* valueRange;

    void deleteValueRange() {
        if(valueRange != NULL) { delete valueRange; }
        valueRange = NULL;
    }
};

template <class T>
class TypedListColumnQueryState : public TypedColumnQueryState<T> {
    public:
    std::vector<bool> visited;
    bool isMainColumn;
};
#endif /* TRISS_ENGINE_SRC_COLUMNS_COLUMNQUERYSTATE_H_ */
