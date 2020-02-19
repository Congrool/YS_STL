#include "ys_allocator_base.hpp"

namespace YS_STL{

    template<typename _Tp>
    class allocator : public __allocator_base<_Tp>{
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
            typedef allocator<_Tp1> other;
        };

        allocator() noexcept {}
        ~allocator() noexcept {}

        // How does this function work?
        // Does it do nothing?
        allocator(const allocator& _a) noexcept : __allocator_base<_Tp>(_a) { };  
        
        template<typename _Tp1>
        allocator(const allocator<_Tp1>& _a) noexcept { }
    };


// allocator specialization
// void type don't have reference
    template<>
    class allocator<void>{
        public:
            typedef size_t      size_type;
            typedef void*       pointer;
            typedef const void* const_pointer;
            typedef void        value_type;
            typedef ptrdiff_t   difference_type;

            template<typename _Tp1>
            struct rebind{
                typedef allocator<_Tp1> other;
            };
    };
}