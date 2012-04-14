#include "Result.h"

Result::Result(const std::vector<ColumnDesc>& schema, std::list<Row*>* rows) {
    this -> schema = schema;
    this->rows = rows;
    current = this -> rows -> begin();
}

Result::~Result() {
    for (std::list<Row*>::iterator it = rows -> begin(); it != rows -> end(); ++it) {
        (*it) -> dispose(schema);
        delete *it;
    }
    delete rows;
}

bool Result::hasNext() {
    return current != rows -> end();
}

Row* Result::next() {
    return *current++;
}

std::list<Row*>* Result::fetchAll() {
    current = rows -> end();
    return rows;
}
