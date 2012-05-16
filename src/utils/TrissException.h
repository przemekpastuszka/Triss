/*
 * Copyright 2012 Przemysław Pastuszka
 */
#ifndef TRISSEXCEPTION_H
#define	TRISSEXCEPTION_H

#include <sstream>
#include <string>
#include <exception>
#include <src/common/Schema.h>

class TrissException: public std::exception {
private:
    std::ostringstream oss;
    
public:
    
    TrissException() {}
    TrissException(const TrissException& ex) {
        *this = ex;
    }
    
    virtual ~TrissException() throw() {}
    
    TrissException& operator<<(const char* str) { oss << str; return *this; }
    TrissException& operator<<(std::string& str) { oss << str; return *this; }
    TrissException& operator<<(double i) { oss << i; return *this; }
    TrissException& operator<<(Schema::DataType type) { oss << static_cast<int>(type); return *this; }
    
    TrissException& operator=(const TrissException& ex) {
        oss << ex.oss.str();
        return *this;
    }
    
    virtual const char* what() const throw() {
        return oss.str().c_str();
    }
};


#endif	/* TRISSEXCEPTION_H */

