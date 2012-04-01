/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_TYPEDCOLUMN_H_
#define TRISS_ENGINE_SRC_COLUMNS_TYPEDCOLUMN_H_

#include <algorithm>
#include "Fields.h"
#include "Column.h"

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
    std::vector<Field<T> > fields;

    int lowerBound(const T& value) const {
        typename std::vector<Field<T> >::const_iterator it =
                std::lower_bound(fields.begin(), fields.end(), value);
        return int(it - fields.begin());
    }
    int upperBound(const T& value) const {
        typename std::vector<Field<T> >::const_iterator it =
                std::upper_bound(fields.begin(), fields.end(), value);
        return int(it - fields.begin()) - 1;
    }
    void addField(const T& value, int nextFieldId);

    virtual TypedColumnQueryState<T>* getTypedState(ColumnQueryState* state) const {
        return static_cast<TypedColumnQueryState<T>*>(state);
    }

    public:
    void sort() {
        std::sort(fields.begin(), fields.end());
    }
    Field<T>* getField(unsigned int i) { return &fields[i]; }
    unsigned int getSize() const { return fields.size(); }

    virtual ~TypedColumn() {}

    /*** preparing structure ***/
    void createMappingFromCurrentToSortedPositions(std::vector<int>& mapping);

    /*** 'select' auxiliary methods ***/
    virtual ColumnQueryState* prepareColumnForQuery() const = 0;
    virtual void addConstraint(Constraint* constraint, ColumnQueryState* state) const;
    virtual IndexRange reduceConstraintsToRange(ColumnQueryState* state) const;

    friend class AbstractTableTest;
};

template <class T>
void TypedColumn<T>::addField(const T& value, int nextFieldId) {
    Field<T> field;
    field.value = value;
    field.nextFieldId = nextFieldId;
    this -> fields.push_back(field);
}

template <class T>
void TypedColumn<T>::createMappingFromCurrentToSortedPositions(std::vector<int>& mapping) {
    std::vector<Position> positions;
    positions.resize(fields.size(), Position());

    for(unsigned int i = 0; i < fields.size(); ++i) {
        positions[i].element = fields[i].value;
        positions[i].position = i;
    }
    std::sort(positions.begin(), positions.end());

    mapping.resize(fields.size(), 0);
    for(unsigned int i = 0; i < fields.size(); ++i) {
        mapping[positions[i].position] = i;
    }
}

template<class T> void TypedColumn<T>::addConstraint(Constraint *constraint, ColumnQueryState* state) const {
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

template<class T> IndexRange TypedColumn<T>::reduceConstraintsToRange(ColumnQueryState* state) const {
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


#endif  // TRISS_ENGINE_SRC_COLUMNS_TYPEDCOLUMN_H_
