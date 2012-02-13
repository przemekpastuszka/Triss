#include <cstdio>
#include "Column.h"
#include "StringColumnField.h"

int main() {
    Column c(10);
    c.addField(new StringColumnField("baba", NULL));
    c.addField(new StringColumnField("barbarian", NULL));
    c.addField(new StringColumnField("ala has a half-dead cat", NULL));

    c.sort();

    return 0;
}
