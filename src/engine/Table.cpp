/*
* Copyright 2012 Przemysław Pastuszka
*/
#include <cstdlib>
#include <string>
#include "Table.h"
#include "columns/ScalarColumn.h"
#include "columns/ListColumn.h"
#include <src/common/ValueRange.h>
#include <src/common/Row.h>

void Table::prepareColumns() {
    columns.reserve(schema.size());
    for(unsigned int i = 0; i < schema.size();++i){
        columns[i] = generateColumn(schema[i]);
        columns[i] -> setColumnId(i);
    }
}

Column* Table :: generateColumn(Schema::DataType type) {
    switch(type) {
        case Schema::STRING:
            return new ScalarColumn<std::string>();
        case Schema::NUMERICAL:
            return new ScalarColumn<double>();
        case Schema::NUMERICAL_LIST:
            return new ListColumn<double>();
        case Schema::STRING_LIST:
            return new ListColumn<std::string>();
    }
    return NULL;
}

void Table::addRow(Row& row) {
    int initialFirstColumnSize = columns[0] -> getSize();
    for(unsigned int i = 0; i < schema.size() - 1; ++i) {
        columns[i] -> add(row, columns[i + 1] -> getSize());
    }
    columns[schema.size() - 1] -> add(row, initialFirstColumnSize);
}

void Table::prepareStructure() {
    prepareCrossColumnPointers();
    sortColumns();
}

void Table::prepareCrossColumnPointers() {
    std::vector<int> mappings[2];
    int totalNumberOfFields = columns[schema.size() - 1] -> getSize();

    columns[0] -> createMappingFromCurrentToSortedPositions(mappings[1]);
    unsigned int i;
    for(i = 0; i < schema.size() - 1; ++i) {
        columns[i + 1] -> createMappingFromCurrentToSortedPositions(mappings[i % 2]);

        columns[i] -> setGlobalPosition(totalNumberOfFields);
        totalNumberOfFields += columns[i] -> getSize();

        columns[i] -> updateNextFieldIdsUsingMapping(mappings[(i + 1) % 2], mappings[i % 2], totalNumberOfFields);
    }

    columns[0] -> createMappingFromCurrentToSortedPositions(mappings[i % 2]);
    columns[schema.size() - 1] -> setGlobalPosition(totalNumberOfFields);
    columns[schema.size() - 1] -> updateNextFieldIdsUsingMapping(mappings[(i + 1) % 2], mappings[i % 2], columns[schema.size() - 1] -> getSize());
}

void Table::sortColumns() {
    for(unsigned int i = 0; i < schema.size(); ++i) {
        columns[i] -> sort();
    }
}

Result* Table::select(const Query& q) const {
    std::vector<ColumnQueryState*> columnStates;

    prepareColumnsForQuery(columnStates);
    applyConstraintsToColumns(q, columnStates);
    MainColumnInfo mainColumnInfo = chooseMainColumn(columnStates);

    Result* results = gatherResults(q, columnStates, mainColumnInfo);

    for(unsigned int i = 0; i < this -> schema.size(); ++i) {
        delete columnStates[i];
    }

    return results;
}

Result* Table::gatherResults(const Query& q, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info) const {
    std::list<Row*>* results = new std::list<Row*>();
    int limit = q.getLimit();

    if(info.mainColumnRange.left >= 0) {
        Row* row = createTableRow();

        for(int i = 0; i < info.mainColumnRange.length() && results -> size() < limit; ++i) {
            int firstFieldPosition = columns[info.mainColumnId] -> getGlobalPosition() + info.mainColumnRange.left + i;
            if(retrieveRowBeginningWith(firstFieldPosition, row, columnStates, info, false)) {
                retrieveRowBeginningWith(firstFieldPosition, row, columnStates, info, true);
                results -> push_back(row);
                row = createTableRow();
            }
        }
        delete row;
    }
    return new Result(results);
}

bool Table::retrieveRowBeginningWith(int nextFieldId, Row* row, std::vector<ColumnQueryState*>& columnStates, MainColumnInfo& info, bool fill) const {
    unsigned int i;
    for(i = 0; i <= schema.size() && nextFieldId >= 0; ++i) {
        int nextColumnId = (info.mainColumnId + i) % schema.size();
        int relativeFieldId = nextFieldId - columns[nextColumnId] -> getGlobalPosition();
        nextFieldId = columns[nextColumnId] -> fillRowWithValueAndGetNextFieldId(relativeFieldId, row, columnStates[nextColumnId], fill);
    }
    return i > schema.size();
}

void Table::prepareColumnsForQuery(std::vector<ColumnQueryState*>& columnStates) const {
    for(unsigned int i = 0; i < schema.size(); ++i) {
        columnStates.push_back(columns[i] -> prepareColumnForQuery());
    }
}

void Table::applyConstraintsToColumns(const Query& q, std::vector<ColumnQueryState*>& columnStates) const {
    std::list<Constraint*> constraints = q.getConstraints();
    for(std::list<Constraint*>::iterator it = constraints.begin(); it != constraints.end(); it++) {
        Constraint* c = *it;
        int columnId = c -> getAffectedColumn();
        columns[columnId] -> addConstraint(c, columnStates[columnId]);
    }
}

Table::MainColumnInfo Table::chooseMainColumn(std::vector<ColumnQueryState*>& columnStates) const {
    MainColumnInfo info;
    info.mainColumnRange = columns[0] -> reduceConstraintsToRange(columnStates[0]);
    info.mainColumnId = 0;
    for(unsigned int i = 1; i < schema.size(); ++i) {
        IndexRange candidateColumnRange = columns[i] -> reduceConstraintsToRange(columnStates[i]);
        if(candidateColumnRange.length() < info.mainColumnRange.length()) {
            info.mainColumnRange = candidateColumnRange;
            info.mainColumnId = i;
        }
    }
    columns[info.mainColumnId] -> markAsMainQueryColumn(columnStates[info.mainColumnId]);
    return info;
}

void Table::deleteColumns() {
    for(unsigned int i = 0; i < schema.size(); ++i) {
        delete columns[i];
    }
    columns.clear();
    schema.clear();
}
