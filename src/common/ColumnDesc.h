/*
* Copyright 2012 Przemysław Pastuszka
*/
#ifndef TRISS_SRC_COMMON_COLUMNDESC_H_
#define TRISS_SRC_COMMON_COLUMNDESC_H_

#include <string>
#include "Schema.h"

class ColumnDesc {
    public:
        std::string name;
        Schema::DataType type;
        
        ColumnDesc(const std::string& n, Schema::DataType t) {
            name = n;
            type = t;
        }
        ~ColumnDesc() {}
        template <typename Archive> void serialize(
            Archive& ar, const unsigned int version
        ) {
            ar & name;
            ar & type;
        }
};


#endif /* TRISS_SRC_COMMON_COLUMNDESC_H_ */
