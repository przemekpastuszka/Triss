/*
* Copyright 2012 Michał Rychlik
*/
#include <cstdlib>
#include <algorithm>
#include <list>
#include <set>
#include <vector>
#include <string>
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
}

void AliceTable::addRow(Row& row) {
    rows.push_back(&row);
    for (int i = 0; i < schema.size(); ++i) {
        if (std::find(listTypes.begin(), listTypes.end(), schema[i]) != listTypes.end()) {
            if (schema[i] == Schema::NUMERICAL_LIST) {
                TypedListField<double> listField(row.get< std::list<double> >(i), rows.size() - 1);
                std::list< TypedField<double> > listMembers = listField.get_fields();
                for (std::list< TypedField<double> >::iterator it = listMembers.begin(); it != listMembers.end(); ++it) {
                    columns[i].addField(*it);
                }
            } else {
                TypedListField<std::string> listField(row.get< std::list<std::string> >(i), rows.size() - 1);
                std::list< TypedField<std::string> > listMembers = listField.get_fields();
                for (std::list< TypedField<std::string> >::iterator it = listMembers.begin(); it != listMembers.end(); ++it) {
                    columns[i].addField(*it);
                }
            } 
        } else { 
            if (schema[i] == Schema::NUMERICAL) {
                TypedField<double> field(row.get<double>(i), rows.size() - 1);
                columns[i].addField(field);
            } else {
                TypedField<std::string> field(row.get<std::string>(i), rows.size() - 1);
                columns[i].addField(field);            
            }
        }
    }
}

void AliceTable::prepareStructure() {
    for (int i = 0; i < columns.size(); ++i) {
        columns[i].sort();
    }
}

Result* AliceTable::select(const Query& q) {
    google::dense_hash_map<int, int, __gnu_cxx::hash<int>, eqint> hashMap;
    const std::list<Constraint*>& constraints = q.getConstraints();
    for (std::list<Constraint*>::const_iterator it = constraints.begin();
            it != constraints.end(); ++it) {
        std::set<int> matchingIds = columns[(*it)->getAffectedColumn()].applyConstraint(**it);
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

