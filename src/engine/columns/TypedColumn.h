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
        Field<T> element;

        bool operator<(const Position& t) const {
            return element < t.element;
        }
    };

    protected:
    std::vector<Field<T> > fields;

    int lowerBound(const T& value, bool (*cmp)(const Field<T>&, const Field<T>&)) const {
        Field<T> field(value);
        typename std::vector<Field<T> >::const_iterator it =
                std::lower_bound(fields.begin(), fields.end(), field, cmp);
        return int(it - fields.begin());
    }
    int upperBound(const T& value, bool (*cmp)(const Field<T>&, const Field<T>&)) const {
        Field<T> field(value);
        typename std::vector<Field<T> >::const_iterator it =
                std::upper_bound(fields.begin(), fields.end(), field, cmp);
        return int(it - fields.begin()) - 1;
    }
    void addField(const T& value, int nextFieldId, bool isLastElement);
    void addNull(int nextFieldId);

    virtual TypedColumnQueryState<T>* getTypedState(ColumnQueryState* state) const {
        return static_cast<TypedColumnQueryState<T>*>(state);
    }

    public:
    void setNextFieldIdAt(int position, int nextFieldId) { fields[position - globalPosition].nextFieldId = nextFieldId; }
    int getNextFieldIdAt(int position) const { return fields[position - globalPosition].nextFieldId; }
    bool hasNullValueAt(int position) const { return fields[position - globalPosition].isNull; }
    void removeNullsFromColumn();

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
void TypedColumn<T>::removeNullsFromColumn() {
    int position = fields.size();
    while(fields[--position].isNull);
    fields.resize(position + 1);
}

template <class T>
void TypedColumn<T>::addNull(int nextFieldId) {
    Field<T> field;
    field.nextFieldId = nextFieldId;
    field.isNull = true;
    this -> fields.push_back(field);
}

template <class T>
void TypedColumn<T>::addField(const T& value, int nextFieldId, bool isLastElement) {
    Field<T> field;
    field.value = value;
    field.nextFieldId = nextFieldId;
    field.isLastElement = isLastElement;
    this -> fields.push_back(field);
}

template <class T>
void TypedColumn<T>::createMappingFromCurrentToSortedPositions(std::vector<int>& mapping) {
    std::vector<Position> positions;
    positions.resize(fields.size(), Position());

    for(unsigned int i = 0; i < fields.size(); ++i) {
        positions[i].element = fields[i];
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

template<class T> bool closedRangeComparator(const Field<T>& left, const Field<T>& right) { return left.value < right.value; }
template<class T> bool openRangeComparator(const Field<T>& left, const Field<T>& right) { return left.value <= right.value; }

template<class T> IndexRange TypedColumn<T>::reduceConstraintsToRange(ColumnQueryState* state) const {
    TypedColumnQueryState<T>* typedState = getTypedState(state);
    if(typedState -> valueRange != NULL && typedState -> valueRange -> isEmpty()) {
        typedState -> constraintRange = IndexRange();
        return typedState -> constraintRange;
    }

    typedState -> constraintRange = IndexRange(0, getSize() - 1);
    if(typedState -> valueRange != NULL) {
        if(typedState -> valueRange -> isFiniteOnTheLeft()) {
            if(typedState -> valueRange -> isOpenOnTheLeft()) {
                typedState -> constraintRange.left = lowerBound(typedState -> valueRange -> getLeft(), openRangeComparator);
            }
            else {
                typedState -> constraintRange.left = lowerBound(typedState -> valueRange -> getLeft(), closedRangeComparator);
            }
        }
        if(typedState -> valueRange -> isFiniteOnTheRight()) {
            if(typedState -> valueRange -> isOpenOnTheRight()) {
                typedState -> constraintRange.right = upperBound(typedState -> valueRange -> getRight(), openRangeComparator);
            }
            else {
                typedState -> constraintRange.right = upperBound(typedState -> valueRange -> getRight(), closedRangeComparator);
            }
        }
    }
    typedState -> constraintRange.validate(getSize());
    return typedState -> constraintRange;
}


#endif  // TRISS_ENGINE_SRC_COLUMNS_TYPEDCOLUMN_H_
