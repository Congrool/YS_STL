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
      struct iterator_base{
          typedef _Category     iterator_category;
          typedef _Tp           value_type;
          typedef _Distance     difference_type;
          typedef _Pointer      pointer;
          typedef _Reference    reference;
      };

    // if _Iterator doesn't has iterator_category and other attributes
    // using the following template instantation
    template<typename _Iterator, typename = std::__void_t<>>
      struct __iterator_traits { };

    template<typename _Iterator>
      struct __iterator_traits<_Iterator,
                    std::__void_t<typename _Iterator::iterator_category,
                                typename _Iterator::value_type,
                                typename _Iterator::difference_type,
                                typename _Iterator::pointer,
                                typename _Iterator::reference>>
      {
          typedef typename _Iterator::iterator_category     iterator_category;
          typedef typename _Iterator::value_type            value_type;
          typedef typename _Iterator::difference_type       difference_type;
          typedef typename _Iterator::pointer               pointer;
          typedef typename _Iterator::reference             reference;
      };


    template<typename _Iterator>
      struct iterator_traits : public __iterator_traits<_Iterator> { };

    template<typename _Tp>
      struct iterator_traits<_Tp*>
      {
          typedef random_access_iterator_tag    iterator_category;
          typedef _Tp                           value_type;
          typedef ptrdiff_t                     difference_type;
          typedef const _Tp*                    pointer;
          typedef const _Tp&                    reference;
      };
    

    // if _InIter is not a InputIterator or can't convert to InputIterator
    // compile error occurs.
    // if_Inter is pod type, typename iterator_traits<_Inter>::iterator_category is void
    template<typename _InIter>
        using _RequireInputIter = typename std::enable_if<
                std::is_convertible<typename iterator_traits<_InIter>::iterator_category,
                    input_iterator_tag>::value>::type;
}

#endif