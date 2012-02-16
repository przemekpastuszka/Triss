#include <algorithm>
#include "Column.h"


bool Column::compare(Column::Field* a, Column::Field* b) {
    return a -> compare(b) < 0;
}

void Column::sort() {
    std::sort(fields, fields + currentSize, compare);
}

struct Column::Position {
    int position;
    Column::Field* field;
};

bool Column::compareFieldsIdentifiedByPositions(const Position& a, const Position& b) {
    return Column::compare(a.field, b.field);
}

int* Column::getMappingFromCurrentToSortedPositions() {
    Position* positions = new Position[currentSize];
    for(int i = 0; i < currentSize; ++i) {
        positions[i].field = fields[i];
        positions[i].position = i;
    }
    std::sort(positions, positions + currentSize, compareFieldsIdentifiedByPositions);

    int* mapping = new int[currentSize];
    for(int i = 0; i < currentSize; ++i) {
        mapping[positions[i].position] = i;
    }

    delete [] positions;

    return mapping;
}

void Column::addField(Field *field) {
    fields[currentSize++] = field;
}

void Column::updateNextFieldIdsUsingMapping(int *mapping) {
    for(int i = 0; i < currentSize; ++i) {
        fields[i] -> updateNextFieldUsingMapping(mapping);
    }
}

void Column::Field::updateNextFieldUsingMapping(int *mapping) {
    nextFieldId = mapping[nextFieldId];
}

Column::~Column() {
    for (int i = 0; i < currentSize; ++i) {
        delete fields[i];
    }
    delete [] fields;
}
