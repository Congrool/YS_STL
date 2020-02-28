#ifndef _YS_ALLOCATOR_BASE_H
#define _YS_ALLOCATOR_BASE_H 1

#include<bits/c++config.h>
#include<iostream>

namespace YS_STL{

  using std::size_t;
  using std::ptrdiff_t;
  
  void __throw_bad_alloc(){
      std::cerr<<"out of memory" << std::endl; 
      exit(1);
  }

  template<typename _Tp>
  class __allocator_base{
    public:
        typedef size_t      size_type;
        typedef _Tp*        pointer;
        typedef const _Tp*  const_pointer;
        typedef _Tp&        reference;
        typedef const _Tp&  const_reference;
        typedef _Tp         value_type;
        typedef ptrdiff_t   difference_type;

        template<typename _Tp1>
        struct rebind{
            typedef __allocator_base<_Tp1> other;
        };

        // normal constructor
        __allocator_base() noexcept { };

        // normal copy constructor
        __allocator_base(const __allocator_base<_Tp>&) noexcept { };

        // copy assignment constructor
        __allocator_base& operator=(const __allocator_base<_Tp>&) noexcept { };

        // deallocator
        ~__allocator_base() noexcept { };


        pointer address(reference _x) noexcept{
            return &_x;
        }
        const_pointer address(reference _x) const noexcept{
            return &_x;
        }

        // allocate uninitialized memory
        pointer allocate(size_type _n, const void* = 0){
            if(_n > max_size())
                YS_STL::__throw_bad_alloc();
            return static_cast<_Tp*>(::operator new(_n * sizeof(_Tp)));
        }

        void deallocate(pointer _p, size_type){
            ::operator delete(_p);
        }

        // max memory the machine has
        size_type max_size() const noexcept{
            return size_t(-1) / sizeof(_Tp);
        }

        template<typename _Up, typename... _Args>
        void construct(_Up* _p, _Args&&... _args){
            ::new((void*)_p) _Up(std::forward<_Args>(_args)...);
        }

        template<typename _Up>
        void destory(_Up* _p){
            _p->~_Up();
        }

  };
  template<typename _Tp>
  inline bool operator==(const __allocator_base<_Tp>&, const __allocator_base<_Tp>&){
      return true;
  }
  template<typename _Tp>
  inline bool operator!=(const __allocator_base<_Tp>&, const __allocator_base<_Tp>&){
      return false;
  }
  template<typename _Tp1, typename _Tp2>
  inline bool operator==(const __allocator_base<_Tp1>&, const __allocator_base<_Tp2>&){
      return true;
  }
  template<typename _Tp1, typename _Tp2>
  inline bool operator!=(const __allocator_base<_Tp1>&, const __allocator_base<_Tp2>&){
      return false;
  }
}

#endif
