/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_ENGINE_SRC_COLUMNS_TYPEDCOLUMN_H_
#define TRISS_ENGINE_SRC_COLUMNS_TYPEDCOLUMN_H_

#include <algorithm>
#include "Fields.h"
#include "Column.h"
#include <src/utils/TrissException.h>

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
    int numberOfEmptyListFields;

    int lowerBound(const T& value, bool (*cmp)(const Field<T>&, const Field<T>&)) const {
        Field<T> field(value);
        typename std::vector<Field<T> >::const_iterator it =
                std::lower_bound(fields.begin(), fields.end() - numberOfEmptyListFields, field, cmp);
        return int(it - fields.begin());
    }
    int upperBound(const T& value, bool (*cmp)(const Field<T>&, const Field<T>&)) const {
        Field<T> field(value);
        typename std::vector<Field<T> >::const_iterator it =
                std::upper_bound(fields.begin(), fields.end() - numberOfEmptyListFields, field, cmp);
        return int(it - fields.begin()) - 1;
    }
    void addField(const T& value, int nextFieldId, bool isLastElement);
    void addNull(int nextFieldId);
    void addEmptyListField(int nextFieldId);

    virtual TypedColumnQueryState<T>* getTypedState(ColumnQueryState* state) const {
        return static_cast<TypedColumnQueryState<T>*>(state);
    }

    public:
    TypedColumn() {
        numberOfEmptyListFields = 0;
    }
    virtual ~TypedColumn() {}
    
    unsigned int getNumberOfEmptyLists() const { return numberOfEmptyListFields; }
    void setNextFieldIdAt(int position, int nextFieldId) { fields[position - globalPosition].nextFieldId = nextFieldId; }
    int getNextFieldIdAt(int position) const { return fields[position - globalPosition].nextFieldId; }
    bool hasNullValueAt(int position) const { return fields[position - globalPosition].isNull; }
    void removeNullsFromColumn();

    void sort() {
        std::sort(fields.begin(), fields.end());
    }
    Field<T>* getField(unsigned int i) { return &fields[i]; }
    unsigned int getSize() const { return fields.size(); }

    /*** preparing structure ***/
    void createMappingFromCurrentToSortedPositions(std::vector<int>& mapping);

    /*** 'select' auxiliary methods ***/
    virtual ColumnQueryState* prepareColumnForQuery() const = 0;
    virtual void addConstraint(Constraint* constraint, ColumnQueryState* state) const;
    virtual IndexRangeSet reduceConstraintsToRangeSet(ColumnQueryState* state) const;

    friend class AbstractTableTest;
};

template <class T>
void TypedColumn<T>::removeNullsFromColumn() {
    int position = fields.size() - 1;
    while(position > 0 && fields[position].isNull) {
        --position;
    }
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
void TypedColumn<T>::addEmptyListField(int nextFieldId) {
    Field<T> field;
    field.nextFieldId = nextFieldId;
    field.isEmptyList = true;
    this -> fields.push_back(field);
    ++numberOfEmptyListFields;
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
    TypedConstraint<T>* typedConstraint = dynamic_cast<TypedConstraint<T>*>(constraint);
    if(typedConstraint == NULL) {
        throw TrissException() << "Wrong constraint type given";
    }
    
    if(typedState -> valueRangeSet == NULL) {
        typedState -> valueRangeSet = ValueRangeSet<T>::createFromConstraint(typedConstraint);
    }
    else {
        typedState -> valueRangeSet -> intersectWith(typedConstraint);
    }
}

template<class T> bool closedRangeComparator(const Field<T>& left, const Field<T>& right) { return left.value < right.value; }
template<class T> bool openRangeComparator(const Field<T>& left, const Field<T>& right) { return left.value <= right.value; }

template<class T> IndexRangeSet TypedColumn<T>::reduceConstraintsToRangeSet(ColumnQueryState* state) const {
    TypedColumnQueryState<T>* typedState = getTypedState(state);
    
    if(typedState -> valueRangeSet != NULL) {
        typedState -> valueRangeSet -> begin();
        while(typedState -> valueRangeSet -> hasNext()) {
            ValueRange<T>* valueRange = typedState -> valueRangeSet -> next();
            IndexRange indexRange(0, getSize() - 1 - numberOfEmptyListFields);

            if(valueRange -> isEmpty()) {
                return typedState -> constraintRangeSet;
            }

            if(valueRange -> isFiniteOnTheLeft()) {
                if(valueRange -> isOpenOnTheLeft()) {
                    indexRange.left = lowerBound(valueRange -> getLeft(), openRangeComparator);
                }
                else {
                    indexRange.left = lowerBound(valueRange -> getLeft(), closedRangeComparator);
                }
            }
            if(valueRange -> isFiniteOnTheRight()) {
                if(valueRange -> isOpenOnTheRight()) {
                    indexRange.right = upperBound(valueRange -> getRight(), openRangeComparator);
                }
                else {
                    indexRange.right = upperBound(valueRange -> getRight(), closedRangeComparator);
                }
            }
            if(indexRange.validate(getSize())) {
                typedState -> constraintRangeSet.ranges.push_back(indexRange);
            }
        }
    }
    else {
        typedState -> constraintRangeSet.ranges.push_back(IndexRange(0, getSize() - 1 - numberOfEmptyListFields));
    }
    if(numberOfEmptyListFields > 0) {
        typedState -> constraintRangeSet.ranges.push_back(IndexRange(getSize() - numberOfEmptyListFields, getSize() - 1));
    }
    
    return typedState -> constraintRangeSet;
}


#endif  // TRISS_ENGINE_SRC_COLUMNS_TYPEDCOLUMN_H_
