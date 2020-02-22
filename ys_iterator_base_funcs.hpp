#ifndef _YS_ITERATOR_BASE_FUNCS_H
#define _YS_ITERATOR_BASE_FUNCS_H 1

#include "ys_iterator_base_types.hpp"

namespace YS_STL{
    
    /** There should be 
     * distance(InputIterator _first,InputIterator _last, input_iterator_tag)
     * distance(RandomAccessIterator _first, RandomAccessIterator _last, random_iterator_tag)
     * optional: distance(list_iterator) 
     *  
     * advance(InputIterator& _i, distance_type _n, input_iterator_tag)
     * advance(RandomAccessIterator& _i, distance_type _n, random_access_iterator_tag)
     * advance(BidirectionalIterator& _i, distance_type _n, bidirectional_iterator_tag)
     * 
     * next(ForwardIterator _x, difference_type _n = 1)
     * 
     * prev(BidirectionalIterator _x, difference_type _n = 1)
     */


    template<typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    __distance(_InputIterator _first, _InputIterator _last, input_iterator_tag){
        typename iterator_traits<_InputIterator>::difference_type _n = 0;
        while(_first != _last){
            ++_first;
            ++_n;
        }
        return _n;
    }

    template<typename _RandomAccessIterator>
    inline typename iterator_traits<_RandomAccessIterator>::difference_type
    __distance(_RandomAccessIterator _first, _RandomAccessIterator _last,random_access_iterator_tag){
        return _last - _first;
    }

    template<typename _Iterator>
    inline typename iterator_traits<_Iterator>::difference_type
    distance(_Iterator _first, _Iterator _last){
        return __distance(_first,_last,
            typename iterator_traits<_Iterator>::iterator_category());
    }

    template<typename _InputIterator, typename _Distance>
    inline void 
    __advance(_InputIterator& _i, _Distance _n, input_iterator_tag){
        while(_n--)
            ++_i;
    }

    template<typename _RandomAccessIterator, typename _Distance>
    inline void
    __advance(_RandomAccessIterator& _i, _Distance _n, random_access_iterator_tag){
        _i += _n;
    }

    template<typename _BidirectionalIterator, typename _Distance>
    inline void
    __advance(_BidirectionalIterator& _i, _Distance _n, bidirectional_iterator_tag){
        if(_n > 0)
            while(_n--)
                ++_i;
        else
            while(_n++)
                --_i;
    }

    template<typename _Iterator, typename _Distance>
    inline void
    advance(_Iterator& _i, _Distance _n){
        __advance(_i,_n,
            typename iterator_traits<_Iterator>::iterator_category());
    }

    template<typename _InputIterator>
    inline _InputIterator
    next(_InputIterator _i, 
        typename iterator_traits<_InputIterator>::difference_type _n = 1){
        advance(i,n);
        return i;
    }

    template<typename _BidirectionalIterator>
    inline _BidirectionalIterator
    prev(_BidirectionalIterator _i, 
        typename iterator_traits<_BidirectionalIterator>::difference_type _n = 1)
    {
        advance(_i,-_n);
        return _i;
    }
}

#endif