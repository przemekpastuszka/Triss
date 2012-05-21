/*
* Copyright 2012 Przemysław Pastuszka
*/

#ifndef TRISS_SRC_COMMON_VALUERANGESET_H_
#define TRISS_SRC_COMMON_VALUERANGESET_H_

#include "ValueRange.h"
#include <vector>
#include <algorithm>

template <class T>
class ValueRangeSet {
    private:
    ValueRangeSet() : mainRange(NULL) {}
    
    ValueRange<T>* mainRange;
    std::vector<T> exclusions;

    std::vector<ValueRange<T>*> computedRanges;
    int currentRange;
    
    void addRangeToResult(ValueRange<T>* range);
    void deleteComputedRanges();

    public:
    static ValueRangeSet<T>* createFromConstraint(const TypedConstraint<T>* constraint);
    void intersectWith(const TypedConstraint<T>* constraint);
    
    void begin();
    ValueRange<T>* next();
    bool hasNext();
    
    bool isExcluded(const T& v) const;
    
    bool hasAnyConstraint() const;
    bool hasAnyNonExcludingConstraint() const {
        return mainRange != NULL;
    }

    ~ValueRangeSet();
};

template<class T>
ValueRangeSet<T>* ValueRangeSet<T>::createFromConstraint(const TypedConstraint<T>* constraint) {
    ValueRangeSet<T>* result = new ValueRangeSet<T>();
    result -> intersectWith(constraint);
    return result;
}

template <class T>
void ValueRangeSet<T>::intersectWith(const TypedConstraint<T>* constraint) {
    const T& value = constraint -> getConstraintValue();
    
    if(constraint -> getConstraintType() == Constraint::NOT_CONTAIN
            ||constraint -> getConstraintType() == Constraint::NOT_EQUAL) {
        exclusions.push_back(value);
    }
    else {
        ValueRange<T> *v = ValueRange<T>::createFromConstraint(constraint);
        if(mainRange == NULL) {
            mainRange = v;
        }
        else {
            mainRange -> intersectWith(v);
            delete v;
        }
    }
}

template <class T>
void ValueRangeSet<T>::begin() {
    currentRange = 0;
    deleteComputedRanges();
    
    if(exclusions.size() > 0) {
        std::sort(exclusions.begin(), exclusions.end());
        
        addRangeToResult(new ValueRange<T>(true, exclusions[0]));
        for(int i = 1; i < exclusions.size(); ++i) {
            addRangeToResult(new ValueRange<T>(exclusions[i - 1], exclusions[i], true, true));
        }
        addRangeToResult(new ValueRange<T>(exclusions[exclusions.size() - 1], true));
    }
    else {
        if(mainRange -> isEmpty() == false) {
            computedRanges.push_back(mainRange -> copy());
        }
    }
}

template <class T>
void ValueRangeSet<T>::addRangeToResult(ValueRange<T>* range) {
    if(mainRange != NULL) {
        range -> intersectWith(mainRange);
    }
    if(range -> isEmpty()) {
        delete range;
    }
    else {
        computedRanges.push_back(range);
    }
}

template <class T>
bool ValueRangeSet<T>::hasNext() {
    return currentRange < computedRanges.size();
}

template <class T>
ValueRange<T>* ValueRangeSet<T>::next() {
    return computedRanges[currentRange++];
}

template <class T>
bool ValueRangeSet<T>::hasAnyConstraint() const {
    return mainRange != NULL || exclusions.size() > 0;
}

template <class T>
bool ValueRangeSet<T>::isExcluded(const T& v) const {
    for(int i = 0; i < exclusions.size(); ++i) {
        if(exclusions[i] == v) {
            return true;
        }
    }
    return false;
}

template <class T>
ValueRangeSet<T>::~ValueRangeSet() {
    deleteComputedRanges();
    
    if(mainRange != NULL) {
        delete mainRange;
    }
}

template <class T>
void ValueRangeSet<T>::deleteComputedRanges() {
    for(int i = 0; i < computedRanges.size(); ++i) {
        delete computedRanges[i];
    }
}

#endif /* TRISS_SRC_COMMON_VALUERANGESET_H_ */
