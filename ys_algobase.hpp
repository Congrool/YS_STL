#ifndef _YS_ALGOBASE_H
#define _YS_ALGOBASE_H 1

#include "ys_construct.hpp"
#include <bits/stl_algobase.h>
#include <bits/stl_construct.h>
#include <bits/stl_uninitialized.h>


namespace YS_STL{

    // It has specialization version
    // _TrivialValueTypes is False
    // means invoking copy constructor
    /*
    template<bool _TrivialValueTypes>
    struct __uninitialized_copy{
        template<typename _InputIterator, typename _ForwardIterator>
        static _ForwardIterator
        __uninit_copy(_InputIterator _first, _InputIterator _last, 
                    _ForwardIterator _result)
        {
            _ForwardIterator _cur = _result;

            try{
                for(; _first != _last; ++_cur, ++_first)
                    YS_STL::_construct(&(*_cur),_first);
                return _cur;
            }
            catch(...){
                // roll back
                YS_STL::_destory(_result,_first);
                throw;
            }
        }   
    };

    template<>
    struct __uninitialized_copy<true>{
        template<typename _InputIterator, typename _ForwardIterator>
        static _ForwardIterator
        __uninit_copy(_InputIterator _first, _InputIterator _last,
                    _ForwardIterator _restut)
        // Have read implementation of std::copy.
        // Seems it's a bit of complex.
        // So let's use it first.
        {
            std::copy(_first,_last,_result);
        }
    };
    */
    template<typename _InputIterator, typename _ForwardIterator, 
            typename _Alloc>
    inline _ForwardIterator
    uninitialized_copy(_InputIterator _first, _InputIterator _last,
                    _ForwardIterator _result, _Alloc _alloc)
    {
        // I don't know the differences between true and false version of
        // struct __uninitialized_copy, so we choose false version which I
        // know more about.
        // TODO: need to learn more about std::copy
        return std::__uninitialized_copy_a(_first, _last, _result, _alloc);
    }

    /*
    template<bool _TrivialValueType>
      struct __uninitialized_fill_n
      {
          template<typename _ForwardIterator, typename _Size, typename _Tp>
            static _ForwardIterator
            __uninit_fill_n(_ForwardIterator _first, _Size _n, const _Tp& _x){
                _ForwardIterator _cur = _first;
                try{
                    for(; _n > 0; --n, ++_cur)
                        YS_STL::_construct(&(*_cur),_x);
                    return _cur;
                }
                catch(...){
                    YS_STL::_destory(_first,_cur);
                    throw;
                }
            }
      };

    template<>
      struct __uninitialized_fill_n<true>
      {
          template<typename _ForwardIterator, typename _Size, typename _Tp>
            static _ForwardIterator
            __uninit_fill_n(_ForwardIterator _first, _Size _n, const _Tp& _x){
                return std::fill_n(_first,_n,_x);
            }
      };
    */
    template<typename _ForwardIterator, typename _Size, typename _Tp,
            typename _Alloc>
    inline _ForwardIterator uninitialized_fill_n(
                _ForwardIterator _first, _Size _n, 
                const _Tp& _x, _Alloc _alloc)
    {   
        // TODO: need to learn more about std::fill_n
        return std::__uninitialized_fill_n_a(_first,_n,_x, _alloc);
    }


    /*
    template<bool _TrivialValueType>
    struct __uninitialized_default_n{
        template<typename _ForwardIterator, typename _Size>
        static _ForwardIterator
        __uninit_default_n(_ForwardIterator _first, _Size _n){
            _ForwardIterator _cur = _first;
            try{
                for(; n > 0; ++_cur, --n)
                    YS_STL::_construct(&(*_cur));
                return _cur;
            }
            catch(...){
                YS_STL::_destory(_first,_cur);
                throw;
            }
        }
    };

    template<>
    struct __uninitialized_default_n<true>{
        template<typename _ForwardIterator, typename _Size>
        static _ForwardIterator
        __uninit_default_n(_ForwardIterator _first, _Size _n){
            typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
            return std::fill_n(_first, _n, _Value_type());
        }
    };
    */
    template<typename _ForwardIterator, typename _Size,
            typename _Alloc>
    inline static _ForwardIterator
    uninitialized_default_n(_ForwardIterator _first, _Size _n, _Alloc _alloc){
        // TODO: need to learn more about std::fill_n
        return std::__uninitialized_default_n_a(_first,_n,_alloc);
    }

    // Seems to use where errors may be raised, contrast with 
    // uninitialized_move()
    template<typename _InputIterator, typename _ForwardIterator,
            typename _Allocator>
    inline _ForwardIterator
    uninitialized_move_if_noexcept(_InputIterator _first,_InputIterator _last
                            ,_ForwardIterator _result, _Allocator _alloc)
    {
        // TODO:
        return std::__uninitialized_move_if_noexcept_a(
                _first, _last, _result, _alloc
                );
    }

    template<typename _InputIterator, typename _ForwardIterator,
            typename _Allocator>
    inline _ForwardIterator
    uninitialized_move(_InputIterator _first, _InputIterator _last,
                    _ForwardIterator _result, _Allocator& _alloc)
    {
        return std::__uninitialized_move_a(_first, _last, _result, _alloc);
    }


    template<typename _Tp, typename _Compare>
    inline const _Tp&
    max(const _Tp& _a, const _Tp& _b, _Compare _comp){
        if(__comp(_a,_b))
            return _b;
        return _a;
    }

    template<typename _Tp>
    inline const _Tp&
    max(const _Tp& _a, const _Tp& _b){
        if(_a < _b)
            return _b;
        return _a;
    }

    template<typename _BT1, typename _BT2>
    inline _BT2
    copy_backward(_BT1 _first, _BT1 _last, _BT2 _result){
        return std::copy_backward(_first, _last, _result);
    }

    template<typename _BT1, typename _BT2>
    inline _BT2
    move_backward(_BT1 _first, _BT1 _last, _BT2 _result){
        return std::move_backward(_first,_last,_result);
    }

    template<typename _InputIterator, typename _OutputIterator>
    inline _OutputIterator
    copy(_InputIterator _first, _InputIterator _last,
         _OutputIterator _result)
    {
        return std::copy(_first, _last, _result);
    }

    template<typename _InputIterator, typename _OutputIterator>
    inline _OutputIterator
    move(_InputIterator _first, _InputIterator _last,
        _OutputIterator _result)
    {
        return std::move(_first, _last, _result);
    }

    template<typename _InputIterator, typename _OutputIterator>
    inline _OutputIterator
    fill(_InputIterator _first, _InputIterator _last,
        _OutputIterator _result)
    {
        return std::fill(_first, _last, _result);
    }

    template<typename _OutputIterator, typename _Size, typename _Tp>
    inline _OutputIterator
    fill_n(_OutputIterator _start, _Size _n, const _Tp& _val)
    {
        return std::fill_n(_start,_n,_val);
    }

}

#endif