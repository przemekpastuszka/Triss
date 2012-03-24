/*
* Copyright 2012 Przemysław Pastuszka
*/

#ifndef COLUMNQUERYSTATE_H_
#define COLUMNQUERYSTATE_H_

#include "IndexRange.h"
#include <prototypes/common/src/ValueRange.h>

namespace Bob {
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
}
#endif /* COLUMNQUERYSTATE_H_ */
