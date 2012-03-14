/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef PROTOTYPES_BOB_SRC_COLUMNS_TYPEDCOLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMNS_TYPEDCOLUMN_H_

#include <algorithm>
#include "Fields.h"
#include "Column.h"
#include "Column.cpp"

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

    void deleteValueRange() {
        if(valueRange != NULL) { delete valueRange; }
        valueRange = NULL;
    }

    protected:
    int columnId;
    IndexRange constraintRange;

    virtual Field<T>* getField(unsigned int i) = 0;
    virtual int lowerBound(const T& value) = 0;
    virtual int upperBound(const T& value) = 0;

    public:
    TypedColumn() : valueRange(NULL) {}
    virtual ~TypedColumn() { deleteValueRange(); }

    /*** preparing structure ***/
    void setColumnId(int id) { columnId = id; }
    void createMappingFromCurrentToSortedPositions(std::vector<int>& mapping);
    void updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next);

    /*** 'select' auxiliary methods ***/
    virtual void prepareColumnForQuery() { deleteValueRange(); }
    virtual void addConstraint(Constraint* constraint);
    virtual IndexRange reduceConstraintsToRange();

    friend class AbstractBobTest;
};

template <class T>
void TypedColumn<T>::createMappingFromCurrentToSortedPositions(std::vector<int>& mapping) {
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
void TypedColumn<T>::updateNextFieldIdsUsingMapping(std::vector<int>& current, std::vector<int>& next) {
    for(unsigned int i = 0; i < getSize(); ++i) {
        getField(i) -> updateNextFieldUsingMapping(current, next);
    }
}

template<class T> void TypedColumn<T>::addConstraint(Constraint *constraint) {
    ValueRange<T>* r = ValueRange<T>::createFromConstraint((TypedConstraint<T>*) constraint);
    if(valueRange == NULL) {
        valueRange = r;
    }
    else {
        valueRange -> intersectWith(r);
        delete r;
    }
}

template<class T> Column::IndexRange TypedColumn<T>::reduceConstraintsToRange() {
    if(valueRange != NULL && valueRange -> isEmpty()) {
        constraintRange = IndexRange();
        return constraintRange;
    }

    constraintRange = IndexRange(0, getSize() - 1);
    if(valueRange != NULL) {
        if(valueRange -> isFiniteOnTheLeft()) {
            constraintRange.left = lowerBound(valueRange -> getLeft());
        }
        if(valueRange -> isFiniteOnTheRight()) {
            constraintRange.right = upperBound(valueRange -> getRight());
        }
    }
    constraintRange.validate(getSize());
    return constraintRange;
}


#endif  // PROTOTYPES_BOB_SRC_COLUMNS_TYPEDCOLUMN_H_
