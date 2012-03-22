/*
* Copyright 2012 Michał Rychlik
*/
#include <cstdlib>
#include <algorithm>
#include <list>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <sparsehash/dense_hash_map>
#include <ext/hash_map>
#include "AliceTable.h"
#include <prototypes/common/src/Row.h>

struct eqint {
  bool operator()(int a, int b) const {
    return (a == b);
  }
};

AliceTable::AliceTable(const Schema& schema) : Table(schema) {
    listTypes.push_back(Schema::NUMERICAL_LIST);
    listTypes.push_back(Schema::STRING_LIST);
    for (int i = 0; i < this->schema.size(); ++i) {
        switch (this->schema[i]) {
            case Schema::NUMERICAL: columns.push_back(new TypedColumn<double>()); break;
            case Schema::STRING: columns.push_back(new TypedColumn<std::string>()); break;
            case Schema::NUMERICAL_LIST: columns.push_back(new TypedColumn<double>()); break;
            case Schema::STRING_LIST: columns.push_back(new TypedColumn<std::string>()); break;
        }
    }
}

void AliceTable::addRow(Row& row) {
    rows.push_back(&row);
    for (int i = 0; i < schema.size(); ++i) {
        if (std::find(listTypes.begin(), listTypes.end(), schema[i]) != listTypes.end()) {
            if (schema[i] == Schema::NUMERICAL_LIST) {
                TypedListField<double> listField(row.get< std::list<double> >(i), rows.size() - 1);
                std::list< TypedField<double>* > listMembers = listField.get_fields();
                for (std::list< TypedField<double>* >::iterator it = listMembers.begin(); it != listMembers.end(); ++it) {
                    static_cast< TypedColumn<double>* >(columns[i])->addField(*it);
                }
            } else {
                TypedListField<std::string> listField(row.get< std::list<std::string> >(i), rows.size() - 1);
                std::list< TypedField<std::string>* > listMembers = listField.get_fields();
                for (std::list< TypedField<std::string>* >::iterator it = listMembers.begin(); it != listMembers.end(); ++it) {
                    static_cast< TypedColumn<std::string>* >(columns[i])->addField(*it);
                }
            } 
        } else {
            if (schema[i] == Schema::NUMERICAL) {
                TypedField<double> field(row.get<double>(i), rows.size() - 1);
                static_cast< TypedColumn<double>* >(columns[i])->addField(&field);
            } else {
                TypedField<std::string> field(row.get<std::string>(i), rows.size() - 1);
                static_cast< TypedColumn<std::string>* >(columns[i])->addField(&field);
            }
        }
    }
}

void AliceTable::prepareStructure() {
    for (int i = 0; i < columns.size(); ++i) {
        if (schema[i] == Schema::NUMERICAL || schema[i] == Schema::NUMERICAL_LIST) {
            static_cast<TypedColumn<double>*>(columns[i])->sort();
        } else {
            static_cast<TypedColumn<std::string>*>(columns[i])->sort();
        }
    }
}

Result* AliceTable::select(const Query& q) {
    google::dense_hash_map<int, int, __gnu_cxx::hash<int>, eqint> hashMap;
    const std::list<Constraint*>& constraints = q.getConstraints();
    for (std::list<Constraint*>::const_iterator it = constraints.begin();
            it != constraints.end(); ++it) {
        int affected_column = (*it)->getAffectedColumn();
        std::set<int> matchingIds;
        if (schema[affected_column] == Schema::NUMERICAL || schema[affected_column] == Schema::NUMERICAL_LIST) {
            matchingIds = static_cast<TypedColumn<double>*>(columns[affected_column])->applyConstraint(static_cast<TypedConstraint<double>*>(*it));
        } else {
            matchingIds = static_cast<TypedColumn<std::string>*>(columns[affected_column])->applyConstraint(static_cast<TypedConstraint<std::string>*>(*it));
        }
        for (std::set<int>::iterator sit = matchingIds.begin(); sit != matchingIds.end(); ++sit) {
            hashMap[*sit] += 1;
        }
    }
    int no_constraints = constraints.size();
    std::vector<int> resultingIds;
    for (google::dense_hash_map<int, int, __gnu_cxx::hash<int>, eqint>::iterator it = hashMap.begin(); it != hashMap.end(); ++it) {
        if (it->second == no_constraints) {
            resultingIds.push_back(it->first);
        }
    }
    std::list<Row*>* resultingRows = new std::list<Row*>;
    for (int i = 0; i < q.getLimit(); ++i) {
        resultingRows->push_back(rows[resultingIds[i]]);        
    }
    return new Result(resultingRows);
}
