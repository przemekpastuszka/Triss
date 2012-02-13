#ifndef PROTOTYPES_BOB_SRC_COLUMN_H_
#define PROTOTYPES_BOB_SRC_COLUMN_H_

#include <vector>
#include "ColumnField.h"

class Column {
    private:
    ColumnField** fields;
    int currentSize;

    public:
    explicit Column(int capacity) {
        fields = new ColumnField*[capacity];
        currentSize = 0;
    }
    ~Column();

    void addField(ColumnField* field);
    void sort();
};

#endif  // PROTOTYPES_BOB_SRC_COLUMN_H_
