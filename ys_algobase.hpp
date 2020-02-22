#ifndef _YS_ALGOBASE_H
#define _YS_ALGOBASE_H 1

#include "ys_construct.hpp"

namespace YS_STL{

    // It has specialization version
    // _TrivialValueTypes is False
    // means invoking copy constructor
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

    template<typename _InputIterator, typename _ForwardIterator>
    inline _ForwardIterator
    uninitialized_copy(_InputIterator _first, _InputIterator _last,
                    _ForwardIterator _result)
    {
        // I don't know the differences between true and false version of
        // struct __uninitialized_copy, so we choose false version which I
        // know more about.
        __uninitialized_copy<false>::__uninit_copy(_first,_last,_result);
    }


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
      
    template<typename _ForwardIterator, typename _Size, typename _Tp>
    inline _ForwardIterator uninitialized_fill_n(
                _ForwardIterator _first, _Size _n, const _Tp& _x)
    {   
        return __uninitialized_fill_n<false>::__uninit_fill_n(_first,_n,_x);
    }
}

#endif