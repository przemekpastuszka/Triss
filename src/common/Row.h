/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_SRC_COMMON_ROW_H_
#define TRISS_SRC_COMMON_ROW_H_

#include <cstdlib>
#include <vector>
#include <string>
#include <list>
#include "ColumnDesc.h"

class Row {
    protected:
    void** values;
    
    Row(int size);
    
    virtual void dispose(const std::vector<ColumnDesc>& schema);
    void deleteFieldAt(int index, const std::vector<ColumnDesc>& schema);
    
    public:
    virtual ~Row();
        
    template <class T>
    void set(int i, const T& value, const std::vector<ColumnDesc>& schema) {
        deleteFieldAt(i, schema);
        values[i] = new T(value);
    }

    template <class T>
    T& get(int i) const {
        return *static_cast<T*>(values[i]);
    }

    void setNull(int i, const std::vector<ColumnDesc>& schema) {
        deleteFieldAt(i, schema);
    }

    bool isNull(int i) const {
        return values[i] == NULL;
    }
    
    friend class Result;
    friend class Table;
};

class TableRow : public Row {
private:
    std::vector<ColumnDesc> schema;
    
    TableRow(const std::vector<ColumnDesc>& schema) : Row(schema.size()) {
        this -> schema = schema;
    }
    
    void dispose(const std::vector<ColumnDesc>& schema) {}
    
public:
    ~TableRow() { Row::dispose(schema); }
    
    template <class T>
    void set(int i, const T& value) {
        deleteFieldAt(i, schema);
        values[i] = new T(value);
    }
    
    friend class Table;
};

#endif /* TRISS_SRC_COMMON_ROW_H_ */
