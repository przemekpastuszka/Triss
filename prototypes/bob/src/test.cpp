#include <cstdio>
#include "NumericalColumn.h"

int main() {
    NumericalColumn c;
    c.init(10);

    c.addField(5, 7);
    c.sort();

    return 0;
}
