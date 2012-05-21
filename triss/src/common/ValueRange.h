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
    bool leftOpen, rightOpen;
    bool empty;

    public:
    ValueRange() : empty(true) {}
    ValueRange(const T& l, bool lOpen)
        : left(l),  leftInfinity(false), rightInfinity(true), leftOpen(lOpen), empty(false) {}
    ValueRange(bool rOpen, const T& r)
        : right(r), leftInfinity(true), rightInfinity(false), rightOpen(rOpen), empty(false) {}
    ValueRange(const T& l, const T& r)
            : left(l), right(r), leftInfinity(false), rightInfinity(false), leftOpen(false), rightOpen(false), empty(false) {}
    ValueRange(const T& l, const T& r, bool lOpen, bool rOpen)
                : left(l), right(r), leftInfinity(false), rightInfinity(false), leftOpen(lOpen), rightOpen(rOpen), empty(false) {}
    
    static ValueRange<T>* createFromConstraint(const TypedConstraint<T>* constraint);
    
    bool isInRange(const T& value);
    bool isEmpty() const { return empty; }
    bool isInfiniteOnTheLeft() const { return leftInfinity; }
    bool isInfiniteOnTheRight() const { return rightInfinity; }
    bool isFiniteOnTheLeft() const { return !leftInfinity; }
    bool isFiniteOnTheRight() const { return !rightInfinity; }
    bool isOpenOnTheLeft() const { return leftOpen; }
    bool isOpenOnTheRight() const { return rightOpen; }
    
    ValueRange<T>* copy() const;

    T& getLeft() { return left; }
    T& getRight() { return right; }
    void intersectWith(ValueRange<T>* other);
};

template<class T>
ValueRange<T>* ValueRange<T>::copy() const {
    ValueRange<T>* v = new ValueRange<T>();
    *v = *this;
    return v;
}

template<class T>
ValueRange<T>* ValueRange<T>::createFromConstraint(const TypedConstraint<T>* constraint) {
    const T& value = constraint -> getConstraintValue();
    switch(constraint -> getConstraintType()) {
        case Constraint::EQUALS:
        case Constraint::CONTAINS:
            return new ValueRange<T>(value, value);
        case Constraint::GREATER_OR_EQUAL:
            return new ValueRange<T>(value, false);
        case Constraint::GREATER:
            return new ValueRange<T>(value, true);
        case Constraint::LESS_OR_EQUAL:
            return new ValueRange<T>(false, value);
        case Constraint::LESS:
            return new ValueRange<T>(true, value);
        case Constraint::NOT_CONTAIN:
        case Constraint::NOT_EQUAL:
            throw "Cannot create ValueRange from NOT_CONTAIN or NOT_EQUAL constraint";
    }
    return NULL;
}

template<class T>
bool ValueRange<T>::isInRange(const T & value) {
    if(empty) {
        return false;
    }
    if(leftInfinity == false) {
        if(value < left || (leftOpen && value == left)) {
            return false;
        }
    }
    if(rightInfinity == false) {
        if(value > right || (rightOpen && value == right)) {
            return false;
        }
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
            leftOpen = other -> leftOpen;
            leftInfinity = false;
        }
    }
    else {
        if(other -> isInfiniteOnTheLeft() == false) {
            if(other -> getLeft() > left) {
                left = other -> getLeft();
                leftOpen = other -> leftOpen;
            }
            else {
                if(leftOpen == false && other -> leftOpen && other -> getLeft() == left) {
                    leftOpen = true;
                }
            }
        }
    }
    if(rightInfinity) {
        if(other -> isInfiniteOnTheRight() == false) {
            right = other -> getRight();
            rightOpen = other -> rightOpen;
            rightInfinity = false;
        }
    }
    else {
        if(other -> isInfiniteOnTheRight() == false) {
            if(other -> getRight() < right) {
                right = other -> getRight();
                rightOpen = other -> rightOpen;
            }
            else {
                if(rightOpen == false && other -> rightOpen && other -> getRight() == right) {
                    rightOpen = true;
                }
            }
        }
    }
    if(left > right || ((leftOpen || rightOpen) && left == right)) {
        empty = true;
    }
}

#endif /* TRISS_SRC_COMMON_VALUERANGE_H_ */
