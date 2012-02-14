#include <algorithm>
#include "Column.h"

Column::~Column() {
    for (int i = 0; i < currentSize; ++i) {
        delete fields[i];
    }
    delete [] fields;
}

bool Column::compare(Column::Field* a, Column::Field* b) {
    return a -> compare(b) < 0;
}

void Column::addField(Field *field) {
    fields[currentSize++] = field;
}

void Column::sort() {
    std :: sort(fields, fields + currentSize, compare);
}
