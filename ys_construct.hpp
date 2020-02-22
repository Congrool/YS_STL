#ifndef _YS_CONSTRUCT_H
#define _YS_CONSTRUCT_H 1

#include "new"
#include "type_traits"
#include "ys_iterator_base_types.hpp"

namespace YS_STL{

  using std::ptrdiff_t;
  using std::size_t;  

  template<typename _Tp, typename... _Args>
    inline void _construct(_Tp* _p,_Args&&... _args){
        ::new(static_cast<void*>(_p)) _Tp(_args...);
    }
  
  template<typename _Tp>
    inline void _destory(_Tp* _p){
        _p->~Tp();
    }

  /**
   * helper function for trivial object
   * do nothing
   */
  template<bool>
    struct _Destory_aux{
        template<typename ForwardIterator>
          static void __destory(ForwardIterator _first, ForwardIterator _last){}
    };

  /**
   * specialization
   * helper function for non-trivial object
   * invoke the destructor
   */
  template<>
    struct _Destory_aux<true>{
        template<typename ForwardIterator>
          static void __destory(ForwardIterator _first, ForwardIterator _last){
              for(; _first != _last; ++_first)
                _destory(&(*_first));
          }
    };

  // I don't know how does __has_trivial_destructor work and where is it defined 
  template<typename ForwardIterator>
    inline void _destory(ForwardIterator _first, ForwardIterator _last){
        typedef typename iterator_traits<ForwardIterator>::value_type   _value_type;
        _Destory_aux<__has_trivial_destructor(_value_type)>::__destory(_first,_last);
    }
  
  // the following two functions needn't be defined I guess
  template<>
    inline void _destory<char*>(char*, char*) {}

#ifdef _WCHAR_T
  template<>
    inline void _destory<wchar_t*>(wchar_t*, wchar_t*) {}
#endif

}

#endif