#include "Result.h"

Result::Result(std::list<Row>& rows) {
    rows = rows;
    current = rows.begin();
}

bool Result::hasNext() {
    return current != rows.end();
}

Row Result::next() {
    return *current++;
}

std::list<Row> Result::fetchAll() {
    current = rows.end();
    return rows;
}