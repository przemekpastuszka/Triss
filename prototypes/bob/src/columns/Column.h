/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_

#include <algorithm>
#include "Fields.h"
#include "../../../common/src/Constraint.h"
#include "../../../common/src/ValueRange.h"

class Column {
    public:
    struct IndexRange {
        int left, right;
        IndexRange() : left(-1), right(-1) {}

        void validate(int size) {
            if(left >= size || right < 0 || left > right) {
                left = right = -1;
            }
        }
    };

    virtual unsigned int getSize() const = 0;

    virtual void sort() = 0;
    virtual int* getMappingFromCurrentToSortedPositions() = 0;
    virtual void updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping) = 0;

    virtual void add(void* value, int nextFieldId) = 0;

    virtual void addConstraint(Constraint* constraint) = 0;
    virtual IndexRange getRangeFromConstraints() = 0;
};

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

    ValueRange<T>* valueRange;

    public:
    virtual Field<T>* getField(unsigned int i) = 0;
    virtual int lowerBound(const T& value) = 0;
    virtual int upperBound(const T& value) = 0;

    public:
    TypedColumn() : valueRange(NULL) {}

    int* getMappingFromCurrentToSortedPositions();
    void updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping);

    virtual void addConstraint(Constraint* constraint);
    virtual IndexRange getRangeFromConstraints();
};

template <class T>
int* TypedColumn<T>::getMappingFromCurrentToSortedPositions() {
    Position* positions = new Position[getSize()];
    for(unsigned int i = 0; i < getSize(); ++i) {
        positions[i].element = this -> getField(i) -> value;
        positions[i].position = i;
    }
    std::sort(positions, positions + getSize());

    int* mapping = new int[getSize()];
    for(unsigned int i = 0; i < getSize(); ++i) {
        mapping[positions[i].position] = i;
    }

    delete [] positions;

    return mapping;
}

template <class T>
void TypedColumn<T>::updateNextFieldIdsUsingMapping(int* currentColumnMapping, int *nextColumnMapping) {
    for(unsigned int i = 0; i < getSize(); ++i) {
        getField(i) -> updateNextFieldUsingMapping(currentColumnMapping, nextColumnMapping);
    }
}

template<class T> void TypedColumn<T>::addConstraint(Constraint *constraint) {
    ValueRange<T>* r = ValueRange<T>::createFromConstraint(*((TypedConstraint<T>*) constraint));
    if(valueRange == NULL) {
        valueRange = r;
    }
    else {
        valueRange -> intersectWith(r);
    }
}

template<class T> Column::IndexRange TypedColumn<T>::getRangeFromConstraints() {
    IndexRange range;
    if(valueRange -> isEmpty() == false) {
        if(valueRange -> isInfiniteOnTheLeft()) {
            range.left = 0;
        }
        else {
            range.left = lowerBound(valueRange -> getLeft());
        }
        if(valueRange -> isInfiniteOnTheRight()) {
            range.right = getSize() - 1;
        }
        else {
            range.right = upperBound(valueRange -> getRight());
        }
    }

    delete valueRange;
    valueRange = NULL;

    range.validate(getSize());
    return range;
}


#endif  // PROTOTYPES_BOB_SRC_COLUMNS_COLUMN_H_
