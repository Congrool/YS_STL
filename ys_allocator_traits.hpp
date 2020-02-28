#ifndef _YS_ALLOCATOR_TRAITS_H
#define _YS_ALLOCATOR_TRAITS_H 1

#include "ys_allocator_base.hpp"

namespace YS_STL{

    template<typename _Alloc>
    struct __alloc_traits{
        typedef typename _Alloc::size_type          size_type;
        typedef typename _Alloc::pointer            pointer;
        typedef typename _Alloc::const_pointer      const_pointer;
        typedef typename _Alloc::reference          reference;
        typedef typename _Alloc::const_reference    const_reference;
        typedef typename _Alloc::value_type         value_type;
        typedef typename _Alloc::difference_type    difference_type;
      
        template<typename _Tp1>
        using rebind = typename _Alloc::template rebind<_Tp1>;
        
        static pointer
        allocate(_Alloc& _a, size_type _n){
            return _a.allocate(_n);
        }

        static void
        deallocate(_Alloc& _a, pointer _p, size_type _n){
            _a.deallocate(_p,_n);
        }

        static size_type
        max_size(const _Alloc& _a){
            return _a.max_size();
        }

        template<typename _Up, typename... _Args>
        static void
        construct(_Alloc& _a, _Up* _p, _Args&&... _args){
            _a.construct(_p,std::forward<_Args>(_args)...);
        }

        template<typename _Up>
        static void
        destory(_Alloc& _a, _Up* _p){
            _a.destory(_p);
        }
    };
}

#endif