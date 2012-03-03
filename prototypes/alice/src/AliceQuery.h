/*
* Copyright 2012 Michał Rychlik
*/
#ifndef PROTOTYPES_ALICE_SRC_ALICE_QUERY_H_
#define PROTOTYPES_ALICE_SRC_ALICE_QUERY_H_

#include <algorithm>
#include <list>

class AliceQuery {
    private:
    std::list<int> selectedColumns;

    public:
    void selectColumns(std::list<int> columns);
    
#ifdef TRISS_TEST
    bool isColumnSelected(int i) {
        return find(selectedColumns.begin(),
                    selectedColumns.end(), i) != selectedColumns.end();
    }
#endif
    
};

#endif  // PROTOTYPES_ALICE_SRC_ALICE_QUERY_H_

