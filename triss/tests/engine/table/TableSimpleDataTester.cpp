/*
* Copyright 2012 Przemysław Pastuszka
*/
#include "AbstractTableTest.cpp"

class TableSimpleDataTester : public AbstractTableTest {
    protected:
    virtual void setUpColumns() {
        nrOfRows = 4;

        listColumn.clear();
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(2, /**/ 1.0, 2.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(1, /**/ 7.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(1, /**/ 3.0)));
        listColumn.push_back(Tools::listFrom(Tools::vector<double>(3, /**/ 0.0, 10.0, 9.0)));

        numericColumn = Tools::vector<double>(4, /**/ 7.0, 1.0, 8.0, 3.0);

        std::string strings[] = { "belial", "merihem", "astaroth", "belphegor" };
        stringColumn.assign(strings, strings + 4);
    }
};
