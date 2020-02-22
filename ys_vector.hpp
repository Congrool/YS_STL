#ifndef _YS_VECTOR_H
#define _YS_VECTOR_H 1

#include "allocator_traits.hpp"
#include "allocator.hpp"
#include "ys_iterator.hpp"
#include "memory.hpp"
// visit allocator's member or type and invoke member funtion 
// through allocator_traits

namespace YS_STL{

    template<typename _Tp, typename _Alloc>
    struct _Vector_base{
        private:
            typedef typename __alloc_traits<_Alloc>::template rebind<_Tp>::other _Tp_alloc_type;
            typedef typename __alloc_traits<_Tp_alloc_type>::pointer pointer;

            struct _Mem_pool : public _Tp_alloc_type{
                pointer _M_start;
                pointer _M_finish;
                pointer _M_end_of_storage;

                // There constructors
                _Mem_pool():_Tp_alloc_type(),_M_start(nullptr),_M_finish(nullptr),
                    _M_end_of_storage(nullptr)
                { }
                
                _Mem_pool(const _Tp_alloc_type& _a) noexcept : _Tp_alloc_type(_a),
                    _M_start(nullptr),_M_finish(nullptr),_M_end_of_storage(nullptr)
                { }

                _Mem_pool(const _Tp_alloc_type&& _a) noexcept : _Tp_alloc_type(std::move(_a)),
                    _M_start(nullptr),_M_finish(nullptr),_M_end_of_storage(nullptr)
                { }

                // The only one function that swap data
                // if parameter of std::swap is rvalue reference, using move
                // else using copy constructor
                void _M_swap_data(_Mem_pool& _x) noexcept{
                    std::swap(_M_start,_x._M_start);
                    std::swap(_M_finish,_x._M_finish);
                    std::swap(_M_end_of_storage,_x._M_end_of_storage);
                }
            };
            
            void
            _M_create_storage(size_t _n){
                _memory_pool._M_start = _M_allocate(_n);
                _memory_pool._M_finish = _memory_pool._M_start;
                _memory_pool._M_end_of_storage = _memory_pool._M_start + _n;
            }

        public:
            typedef _Alloc  allocator_type;

            _Mem_pool _memory_pool;

            // return the lvalue reference 
            // of its memory_pool member
            _Tp_alloc_type& 
            _M_get_Tp_allocator() noexcept{
                return *static_cast<_Tp_alloc_type*>(&this->_memory_pool);
            }

            // return the const lvalue reference 
            // of its memory_pool member
            const _Tp_alloc_type& 
            _M_get_Tp_allocator() const noexcept{
                return *static_cast<const _Tp_alloc_type*>(&this->_memory_pool);
            }

            // return a new allocator_tpye object
            // same as that of the object who invokes the function
            allocator_type 
            get_allocator()const noexcept{
                return allocator_type(_M_get_Tp_allocator());
            }

            pointer
            _M_allocate(size_t _n){
                typedef typename __alloc_traits<_Tp_alloc_type> _Tr;
                return _n != 0 ? _Tr::allocate(_memory_pool,_n) : nullptr;
            }

            void
            _M_deallocate(pointer _p, size_t _n){
                typedef typename __alloc_traits<_Tp_alloc_type> _Tr;
                _Tr::deallocate(_p,_n);
            }

            // different constructors
            // no elements in it
            _Vector_base() : _memory_pool()
            { }
            // using a customized allocator
            // and no elements in it
            _Vector_base(const allocator_type& _x) : _memory_pool(_x)
            { }

            // create an _vector_base object containing
            // a memory pool with size _n * sizeof(_Tp)
            // using its own allocator
            _Vector_base(size_t _n) : _memory_pool(){
                _M_create_storage(_n);
            }
            // create an _vector_base object containing
            // a memory pool with size _n * sizeof(_Tp)
            // using given allocator
            _Vector_base(size_t _n, const allocator_type& _x) noexcept : _memory_pool(_x){
                _M_create_storage(_n);
            }

            _Vector_base(_Tp_alloc_type&& _a) noexcept
                : _memory_pool(std::move(_a))
            { }
            
            _Vector_base(_Vector_base&& _x) noexcept
                : _memory_pool(std::move(_x._M_get_Tp_allocator()))
            {
                _memory_pool._M_swap_data(_x._memory_pool);
            }

            ~Vector_base() noexcept{
                _M_deallocate(_Mem_pool._M_start,_Mem_pool._M_end_of_storage - _Mem_pool._M_start);
            }


    };


    template<typename _Tp, typename _Alloc = YS_STL::allocator<_Tp> >
      class vector : protected _Vector_base<_Tp,_Alloc>{
          
          typedef _Vector_base<_Tp,_Alloc>                  _Base;
          typedef typename _Base::_Tp_alloc_tpye            _Tp_alloc_type;
          typedef typename __alloc_traits<_Tp_alloc_type>   _Alloc_traits;
          
          public:
            typedef _Tp                                     value_type;
            typedef typename _Alloc_traits::pointer         pointer;
            typedef typename _Alloc_traits::const_pointer   const_pointer;
            typedef typename _Alloc_traits::reference       reference;
            typedef typename _Alloc_traits::const_reference const_reference;
            typedef size_t                                  size_type;
            typedef ptrdiff_t                               difference_type;
            typedef _Alloc                                  allocator_type;
            typedef __normal_iterator<pointer,vector>       iterator;
            typedef __normal_iterator<const_pointer,vector> const_iterator;
            typedef reverse_iterator<iterator>              reverse_iterator;
            typedef reverse_iterator<const_iterator>        const_reverse_iterator;

          protected:
            using _Base::_M_allocate;
            using _Base::_M_deallocate;
            using _Base::_memory_pool;
            using _Base::_M_get_Tp_allocator;

          public:
            /**
             * constructors
             */
            vector() noexcept : _Base() { }

            explicit
            vector(const allocator_type& _a) noexcept
            : _Base(_a)
            { }

            explicit
            vector(size_type _n, const allocator_type& _a = allocator_type())
            : _Base(_n,_a)
            { _M_default_initialize(_n); }

            explicit
            vector(size_type _n, const value_type& _value, 
                const allocator_type& _a = allocator_type())
            : _Base(_n,_a)
            { _M_fill_initialize(_n, _value); }

            vector(const vector& _x) 
            : _Base(_x.size(), _x._M_get_Tp_allocator()){
                _memory_pool._M_finish = uninitialized_copy(
                    _x.begin(), _x.end(), this->_memory_pool._M_start
                )
            }

            vector(vector&& _x) noexcept
            : _Base(std::move(_x))
            { }


            // __false_type indicate that parameters are iterator not integer
            template<typename _InputIterator,
                typename = YS_STL::_RequireInputIter<_InputIterator>>
            vector(_InputIterator _first, _InputIterator _last,
                    const allocator_type& _a = allocator_type())
            : base(_a)
            { _M_initilize_dispatch(_first, _last, std::__false_type()); }



            /**
             * Internal constructor functions follow
             * These function are about memory allocation and construction
             */
            // private
            // TODO:
            void _M_default_initialize(size_type _n){
                
            }

            void _M_fill_initialize(size_type _n, const value_type& _value){
                _memory_pool._M_finish = uninitialized_fill_n(
                    _memory_pool._M_start, _n, _value, _M_get_Tp_allocator()
                );  
            }
            
            // parameters are integer type
            // Is this more effective than __false_type one ? 
            // __true_type uses uninitialized_fill_n which uses n > 0 as loop condition 
            // __false_type uses _M_range_initialize
            template<typename _Integer>
            void
            _M_initialize_dispatch(_Integer _n, _Integer _value, std::__true_type){
                _memory_pool._M_start = _M_allocate(static_cast<size_type>(_n));
                _memory_pool._M_end_of_storage = 
                    _memory_pool._M_start + static_cast<size_type>(_n);
                _memory_pool._M_finish = _memory_pool._M_end_of_storage;
                _M_fill_initialize(static_cast<size_type>(_n),_value);
            }
            // parameters are iterator type
            // two situations: InputIterator and ForwardIterator
            // use _M_range_initialize with different implementations
            template<typename _InputIterator>
            void
            _M_initialize_dispatch(_InputIterator _n, _InputIterator _value, std::__false_type){
                typedef typename iterator_traits<_InputIterator>::iterator_category iterator_category;
                _M_range_initialize(_first,_last,iterator_category());
            }



            // _InputIterator uses _first != _last as loop condition and use emplace_back
            template<typename _InputIterator>
            void
            _M_range_initialize(_InputIterator _first, _InputIterator _last,
                                YS_STL::input_iterator_tag)
            {
                try{    
                    for(; _first != _last; ++_first)
                        emplace_bace(*_first);
                }
                catch(...){
                    clear();
                    throw;
                }
            }

            template<typename _ForwardIterator>
            void _M_range_initialize(_InputIterator _first, _InputIterator _last,
                                YS_STL::forward_iterator_tag)
            {   
                
            }


            // TODO:
            size_type size() const{

            }

      };
}

#endif