#include <algorithm>
#include "Column.h"

Column::~Column() {
    for (int i = 0; i < currentSize; ++i) {
        delete fields[i];
    }
    delete [] fields;
}

void Column::addField(ColumnField *field) {
    fields[currentSize++] = field;
}

bool columnFieldsComparision(ColumnField* a, ColumnField* b) {
    return a -> compare(b) < 0;
}

void Column::sort() {
    std :: sort(fields, fields + currentSize, columnFieldsComparision);
}
