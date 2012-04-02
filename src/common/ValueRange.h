/*
* Copyright 2012 Przemysław Pastuszka
*/

#ifndef TRISS_SRC_COMMON_VALUERANGE_H_
#define TRISS_SRC_COMMON_VALUERANGE_H_

#include "Constraint.h"

template <class T>
class ValueRange {
    private:
    T left, right;
    bool leftInfinity, rightInfinity;
    bool empty;

    ValueRange() : empty(true) {}
    ValueRange(const T& l)
        : left(l), leftInfinity(false), rightInfinity(true), empty(false) {}
    ValueRange(bool dummy, const T& r)
        : right(r), leftInfinity(true), rightInfinity(false), empty(false) {}
    ValueRange(const T& l, const T& r)
            : left(l), right(r), leftInfinity(false), rightInfinity(false), empty(false) {}

    public:
    static ValueRange<T>* createFromConstraint(const TypedConstraint<T>* constraint);
    bool isInRange(const T& value);
    bool isEmpty() const { return empty; }
    bool isInfiniteOnTheLeft() const { return leftInfinity; }
    bool isInfiniteOnTheRight() const { return rightInfinity; }
    bool isFiniteOnTheLeft() const { return !leftInfinity; }
    bool isFiniteOnTheRight() const { return !rightInfinity; }

    T& getLeft() { return left; }
    T& getRight() { return right; }
    void intersectWith(ValueRange<T>* other);

};

template<class T>
ValueRange<T>* ValueRange<T>::createFromConstraint(const TypedConstraint<T>* constraint) {
    const T& value = constraint -> getConstraintValue();
    switch(constraint -> getConstraintType()) {
        case Constraint::EQUALS:
        case Constraint::CONTAINS:
            return new ValueRange<T>(value, value);
        case Constraint::GREATER_OR_EQUAL:
            return new ValueRange<T>(value);
        case Constraint::LESS_OR_EQUAL:
            return new ValueRange<T>(false, value);
    }
    return NULL;
}

template<class T>
bool ValueRange<T>::isInRange(const T & value) {
    if(empty) {
        return false;
    }
    if(leftInfinity == false && value < left) {
        return false;
    }
    if(rightInfinity == false && value > right) {
        return false;
    }
    return true;
}

template<class T>
void ValueRange<T>::intersectWith(ValueRange<T> *other) {
    if(other -> isEmpty()) {
        empty = true;
        return;
    }
    if(leftInfinity) {
        if(other -> isInfiniteOnTheLeft() == false) {
            left = other -> getLeft();
            leftInfinity = false;
        }
    }
    else {
        if(other -> isInfiniteOnTheLeft() == false) {
            if(other -> getLeft() > left) {
                left = other -> getLeft();
            }
        }
    }
    if(rightInfinity) {
        if(other -> isInfiniteOnTheRight() == false) {
            right = other -> getRight();
            rightInfinity = false;
        }
    }
    else {
        if(other -> isInfiniteOnTheRight() == false) {
            if(other -> getRight() < right) {
                right = other -> getRight();
            }
        }
    }
    if(left > right) {
        empty = true;
    }
}

#endif /* TRISS_SRC_COMMON_VALUERANGE_H_ */
