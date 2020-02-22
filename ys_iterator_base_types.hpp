#ifndef _YS_ITERATOR_BASE_TYPES_H
#define _YS_ITERATOR_BASE_TYPES_H


#include<type_traits>
#include<bits/c++config.h>

namespace YS_STL{


    /**
     * STL contains five different kinds of iterator as follows
     */
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};


    /**
     * Iterator should define following 5 types.
     * It's required by STL Standard.
     */
    template<typename _Category, typename _Tp, typename _Distance = std::ptrdiff_t,
             typename _Pointer = _Tp*, typename _Reference = _Tp&>
      struct iterator{
          typedef _Category     iterator_category;
          typedef _Tp           value_type;
          typedef _Distance     difference_type;
          typedef _Pointer      pointer;
          typedef _Reference    reference;
      };

    template<typename _Iterator>
      struct iterator_traits{
          typedef typename _Iterator::iterator_category     iterator_category;
          typedef typename _Iterator::value_type            value_type;
          typedef typename _Iterator::difference_type       difference_type;
          typedef typename _Iterator::pointer               pointer;
          typedef typename _Iterator::reference             reference;
      };
    

    // if _InIter is not a InputIterator or can't convert to InputIterator
    // compile error occurs.
    template<typename _InIter>
        using _RequireInputIter = typename std::enable_if<
                std::is_convertible<typename iterator_traits<_Inter>::iterator_category,
                    input_iterator_tag>::value>::type;
}

#endif