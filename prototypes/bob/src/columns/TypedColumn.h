/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_TYPEDCOLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_TYPEDCOLUMN_H_

#include <algorithm>
#include "Fields.h"
#include "Column.h"

namespace Bob {
    template <class T>
    class TypedColumn : public Column {
        private:
        struct Position {
            int position;
            T element;

            bool operator<(const Position& t) const {
                return element < t.element;
            }
        };

        protected:
        int columnId;

        virtual Field<T>* getField(unsigned int i) = 0;
        virtual int lowerBound(const T& value) const = 0;
        virtual int upperBound(const T& value) const = 0;
        virtual TypedColumnQueryState<T>* getTypedState(ColumnQueryState* state) const {
            return static_cast<TypedColumnQueryState<T>*>(state);
        }

        public:
        virtual ~TypedColumn() {}

        /*** preparing structure ***/
        void setColumnId(int id) { columnId = id; }
        void createMappingFromCurrentToSortedPositions(std::vector<int>& mapping);
        void updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next);

        /*** 'select' auxiliary methods ***/
        virtual ColumnQueryState* prepareColumnForQuery() const = 0;
        virtual void addConstraint(Constraint* constraint, ColumnQueryState* state) const;
        virtual IndexRange reduceConstraintsToRange(ColumnQueryState* state) const;

        friend class AbstractBobTest;
    };
}

template <class T>
void Bob::TypedColumn<T>::createMappingFromCurrentToSortedPositions(std::vector<int>& mapping) {
    std::vector<Position> positions;
    positions.resize(getSize(), Position());

    for(unsigned int i = 0; i < getSize(); ++i) {
        positions[i].element = this -> getField(i) -> value;
        positions[i].position = i;
    }
    std::sort(positions.begin(), positions.end());

    mapping.resize(getSize(), 0);
    for(unsigned int i = 0; i < getSize(); ++i) {
        mapping[positions[i].position] = i;
    }
}

template <class T>
void Bob::TypedColumn<T>::updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next) {
    for(unsigned int i = 0; i < getSize(); ++i) {
        getField(i) -> updateNextFieldUsingMapping(current, next);
    }
}

template<class T> void Bob::TypedColumn<T>::addConstraint(Constraint *constraint, ColumnQueryState* state) const {
    TypedColumnQueryState<T>* typedState = getTypedState(state);
    ValueRange<T>* r = ValueRange<T>::createFromConstraint((TypedConstraint<T>*) constraint);
    if(typedState -> valueRange == NULL) {
        typedState -> valueRange = r;
    }
    else {
        typedState -> valueRange -> intersectWith(r);
        delete r;
    }
}

template<class T> Bob::IndexRange Bob::TypedColumn<T>::reduceConstraintsToRange(ColumnQueryState* state) const {
    TypedColumnQueryState<T>* typedState = getTypedState(state);
    if(typedState -> valueRange != NULL && typedState -> valueRange -> isEmpty()) {
        typedState -> constraintRange = IndexRange();
        return typedState -> constraintRange;
    }

    typedState -> constraintRange = IndexRange(0, getSize() - 1);
    if(typedState -> valueRange != NULL) {
        if(typedState -> valueRange -> isFiniteOnTheLeft()) {
            typedState -> constraintRange.left = lowerBound(typedState -> valueRange -> getLeft());
        }
        if(typedState -> valueRange -> isFiniteOnTheRight()) {
            typedState -> constraintRange.right = upperBound(typedState -> valueRange -> getRight());
        }
    }
    typedState -> constraintRange.validate(getSize());
    return typedState -> constraintRange;
}


#endif  // PROTOTYPES_BOB_SRC_COLUMNS_TYPEDCOLUMN_H_
