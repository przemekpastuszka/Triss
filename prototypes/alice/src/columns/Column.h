/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_ALICE_SRC_COLUMNS_COLUMN_H_
#define PROTOTYPES_ALICE_SRC_COLUMNS_COLUMN_H_

#include <algorithm>
#include <vector>
#include <set>
#include "Fields.h"
#include <prototypes/common/src/Constraint.h>

class Column {
public:
    virtual void sort() = 0;
    virtual void addField(Field f) = 0;
    
    virtual std::set<int> applyConstraint(Constraint ct) = 0; 
};

template <class T>
class TypedColumn : public Column {
private:
    std::vector< TypedField<T> > fields;
public:
    TypedColumn() {};
    void addField(Field f) {
        fields.push_back(f);
    };
    void sort() { 
        std::sort(fields.begin(), fields.end());
    };
    
    std::set<int> applyConstraint(Constraint ct) {
        std::set<int> matchingIds;
        typename std::vector< TypedField<T> >::iterator first = fields.begin();
        typename std::vector< TypedField<T> >::iterator last = fields.end();
        switch(ct.getConstraintType()) {
            case Constraint::EQUALS:
            case Constraint::CONTAINS:
                first = std::lower_bound(fields.begin(), fields.end(), ct);
                last = std::upper_bound(fields.begin(), fields.end(), ct);
            case Constraint::GREATER_OR_EQUAL:
                first = std::lower_bound(fields.begin(), fields.end(), ct); 
            case Constraint::LESS_OR_EQUAL:
                last = std::upper_bound(fields.begin(), fields.end(), ct);
        }
        for (typename std::vector< TypedField<T> >::iterator it = first; it != last; ++it) {
            matchingIds.insert(it->id);
        }
        return matchingIds;
    }
};

#endif /* PROTOTYPES_ALICE_SRC_COLUMNS_COLUMN_H_ */
