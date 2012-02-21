/*
 * Copyright 2012 Przemysław Pastuszka
 */
#ifndef TOOLS_H_
#define TOOLS_H_

#include <cstdarg>

#define FETCH_VAR_ARGS(T, count, ...) Tools::fetchVarArgs<T>(count, __VA_ARGS__ )
#define INVOKE_WITH_VAR_ARGS(T, function, count, ...) function(count, FETCH_VAR_ARGS(T, count, __VA_ARGS__))

class Tools {
    public:
    template <class T>
    static T* fetchVarArgs(int count, ...){
        va_list argument_list;
        va_start(argument_list, count);
        T* elements = new T[count];
        for(int i = 0; i < count; ++i) {
            elements[i] = va_arg(argument_list, T);
        }
        va_end(argument_list);
        return elements;
    }
};

#endif /* TOOLS_H_ */
