/*
 * Copyright 2012 Przemysław Pastuszka
 */
#ifndef TOOLS_H_
#define TOOLS_H_

#include <cstdarg>
#include <vector>

class Tools {
    public:
    template <class T>
    static std::vector<T> vector(int count, ...){
        std::vector<T> elements;
        elements.reserve(count);

        va_list argument_list;
        va_start(argument_list, count);
        for(int i = 0; i < count; ++i) {
            elements.push_back(va_arg(argument_list, T));
        }
        va_end(argument_list);
        return elements;
    }
};

#endif /* TOOLS_H_ */
