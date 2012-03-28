/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_ALICE_SRC_COLUMNS_COLUMN_H_
#define PROTOTYPES_ALICE_SRC_COLUMNS_COLUMN_H_

#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include "Fields.h"
#include <prototypes/common/src/Constraint.h>

namespace Alice {

    class Column {
    };

    template <class T>
    class TypedColumn : public Column {
    private:
        std::vector< TypedField<T>* > fields;
        struct TypedColumnLessThan {
            bool operator() (TypedField<T>* left, T right) {
                return *left < right;
            }
            bool operator() (T left, TypedField<T>* right) {
                return left < *right;
            }
        } comparator;
        struct sortComparator {
            bool operator() (TypedField<T>* left, TypedField<T>* right) {
                return *left < *right;
            }
        } sortComparator;
        
        
    public:
        TypedColumn() {};
        ~TypedColumn() {
            for (int i = 0; i < fields.size(); ++i) {
                delete fields[i];
            }
        }
        
        void addField(TypedField<T>* f) {
            fields.push_back(f);
        }
        
        void sort() { 
            std::sort(fields.begin(), fields.end(), sortComparator);
        }
        
        std::set<int> applyConstraint(TypedConstraint<T> * ct) {
            std::set<int> matchingIds;
            typename std::vector< TypedField<T>* >::iterator first = fields.begin();
            typename std::vector< TypedField<T>* >::iterator last = fields.end();             
            switch(ct->getConstraintType()) {
                case Constraint::EQUALS:
                case Constraint::CONTAINS:
                    first = std::lower_bound(fields.begin(), fields.end(), ct->getConstraintValue(), comparator);
                    last = std::upper_bound(fields.begin(), fields.end(), ct->getConstraintValue(), comparator);
                    break;
                case Constraint::GREATER_OR_EQUAL:
                    first = std::lower_bound(fields.begin(), fields.end(), ct->getConstraintValue(), comparator);
                    break; 
                case Constraint::LESS_OR_EQUAL:
                    last = std::upper_bound(fields.begin(), fields.end(), ct->getConstraintValue(), comparator);
                    break;
            }
            for (typename std::vector< TypedField<T>* >::iterator it = first; it != last; ++it) {
                matchingIds.insert((*it)->id);
            }
            return matchingIds;
        }
    };

}

#endif /* PROTOTYPES_ALICE_SRC_COLUMNS_COLUMN_H_ */
