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
#include <google/dense_hash_map>
#include <ext/hash_map>
#include "AliceTable.h"
#include <prototypes/common/src/Row.h>

struct eqint {
  bool operator()(int a, int b) const {
    return (a == b);
  }
};

Alice::AliceTable::AliceTable(const Schema& schema) : Table(schema) {
    for (int i = 0; i < this->schema.size(); ++i) {
        switch (this->schema[i]) {
            case Schema::NUMERICAL: columns.push_back(new TypedColumn<double>()); break;
            case Schema::STRING: columns.push_back(new TypedColumn<std::string>()); break;
            case Schema::NUMERICAL_LIST: columns.push_back(new TypedColumn<double>()); break;
            case Schema::STRING_LIST: columns.push_back(new TypedColumn<std::string>()); break;
        }
    }
}

void Alice::AliceTable::addRow(Row& row) {
    rows.push_back(row.getRowCopy());
    for (int i = 0; i < schema.size(); ++i) {
        TypedListField<double> * numericalListField;
        std::list< TypedField<double>* > numericalListMembers;
        TypedListField<std::string> * stringListField;
        std::list< TypedField<std::string>* > stringListMembers;
        TypedField<double> * numericalField;
        TypedField<std::string> * stringField;
        switch (schema[i]) {
            case Schema::NUMERICAL_LIST: 
                numericalListField = new TypedListField<double>(row.get< std::list<double> >(i), rows.size() - 1);
                numericalListMembers = numericalListField->get_fields();
                for (std::list< TypedField<double>* >::iterator it = numericalListMembers.begin(); it != numericalListMembers.end(); ++it) {
                    static_cast< TypedColumn<double>* >(columns[i])->addField(*it);
                }
                delete numericalListField;
                break;
            case Schema::STRING_LIST:
                stringListField = new TypedListField<std::string>(row.get< std::list<std::string> >(i), rows.size() - 1);
                stringListMembers = stringListField->get_fields();
                for (std::list< TypedField<std::string>* >::iterator it = stringListMembers.begin(); it != stringListMembers.end(); ++it) {
                    static_cast< TypedColumn<std::string>* >(columns[i])->addField(*it);
                }
                delete stringListField;
                break;
            case Schema::NUMERICAL:
                numericalField = new TypedField<double>(row.get<double>(i), rows.size() - 1);
                static_cast< TypedColumn<double>* >(columns[i])->addField(numericalField);
                break;
            case Schema::STRING:
                stringField = new TypedField<std::string>(row.get<std::string>(i), rows.size() - 1);
                static_cast< TypedColumn<std::string>* >(columns[i])->addField(stringField);
                break;
        }
    }
}

void Alice::AliceTable::prepareStructure() {
    for (int i = 0; i < columns.size(); ++i) {
        if (schema[i] == Schema::NUMERICAL || schema[i] == Schema::NUMERICAL_LIST) {
            static_cast<TypedColumn<double>*>(columns[i])->sort();
        } else {
            static_cast<TypedColumn<std::string>*>(columns[i])->sort();
        }
    }
}

Result* Alice::AliceTable::select(const Query& q) {
    google::dense_hash_map<int, int, __gnu_cxx::hash<int>, eqint> hashMap;
    hashMap.set_empty_key(-1);
    const std::list<Constraint*>& constraints = q.getConstraints();
    int no_constraints = constraints.size();
    std::vector<int> resultingIds;
    if (no_constraints > 0) {
        std::vector< std::set<int> > matchedRows;
        std::set<int> matchingIds;
        for (std::list<Constraint*>::const_iterator it = constraints.begin();
                it != constraints.end(); ++it) {
            int affected_column = (*it)->getAffectedColumn();
            if (schema[affected_column] == Schema::NUMERICAL || schema[affected_column] == Schema::NUMERICAL_LIST) {
                matchingIds = static_cast<TypedColumn<double>*>(columns[affected_column])->applyConstraint(static_cast<TypedConstraint<double>*>(*it));
            } else {
                matchingIds = static_cast<TypedColumn<std::string>*>(columns[affected_column])->applyConstraint(static_cast<TypedConstraint<std::string>*>(*it));
            }
            matchedRows.push_back(matchingIds);         
        }
        
        for (int i = 1; i <= matchedRows.size(); ++i) {
            for (std::set<int>::iterator sit = matchedRows[i - 1].begin(); sit != matchedRows[i - 1].end(); ++sit) {
                if (hashMap[*sit] == i - 1) {
                    hashMap[*sit] += 1;
                }            
            }
        }

        for (google::dense_hash_map<int, int, __gnu_cxx::hash<int>, eqint>::iterator it = hashMap.begin(); it != hashMap.end(); ++it) {
            if (it->second == no_constraints) {
                resultingIds.push_back(it->first);
            }
        }
    } else {
        for (int i = 0; i < rows.size(); ++i) {
            resultingIds.push_back(i);
        }
    }
    std::list<Row*>* resultingRows = new std::list<Row*>;
    for (int i = 0; i < std::min(q.getLimit(), (int)resultingIds.size()); ++i) {
        resultingRows->push_back(rows[resultingIds[i]]->getRowCopy());        
    }
    return new Result(resultingRows);
}

Alice::AliceTable::~AliceTable(){
    for (int i = 0; i < columns.size(); ++i) {
        delete columns[i];
    }
    for (int i = 0; i < rows.size(); ++i) {
        delete rows[i];
    }
}
