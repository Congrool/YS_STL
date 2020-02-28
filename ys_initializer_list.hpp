#ifndef _YS_INITIALIZER_LIST_H
#define _YS_INITIALIZER_LIST_H 1

#include <initializer_list>

namespace YS_STL{
    template<typename _E>
    using initializer_list = std::initializer_list<_E>;
}

#endif