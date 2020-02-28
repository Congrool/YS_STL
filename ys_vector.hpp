#ifndef _YS_VECTOR_H
#define _YS_VECTOR_H 1

#include "ys_allocator.hpp"
#include "ys_allocator_traits.hpp"
#include "ys_iterator.hpp"
#include "ys_construct.hpp"
#include "ys_initializer_list.hpp"
#include "ys_errno.hpp"
#include "ys_algobase.hpp"
// visit allocator's member or type and invoke member funtion 
// through allocator_traits

namespace YS_STL{

    template<typename _Tp, typename _Alloc>
    struct _Vector_base{
        protected:
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

                _Mem_pool(const _Tp_alloc_type&& _a) noexcept : _Tp_alloc_type(YS_STL::move(_a)),
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
                typedef typename YS_STL::__alloc_traits<_Tp_alloc_type> _Tr;
                return _n != 0 ? _Tr::allocate(_memory_pool,_n) : nullptr;
            }

            void
            _M_deallocate(pointer _p, size_t _n){
                typedef typename YS_STL::__alloc_traits<_Tp_alloc_type> _Tr;
                _Tr::deallocate(_memory_pool,_p,_n);
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
                : _memory_pool(YS_STL::move(_a))
            { }
            
            _Vector_base(_Vector_base&& _x) noexcept
                : _memory_pool(YS_STL::move(_x._M_get_Tp_allocator()))
            {
                _memory_pool._M_swap_data(_x._memory_pool);
            }

            ~_Vector_base() noexcept{
                _M_deallocate(_memory_pool._M_start,
                    _memory_pool._M_end_of_storage - _memory_pool._M_start);
            }

    };


    template<typename _Tp, typename _Alloc = allocator<_Tp> >
      class vector : protected _Vector_base<_Tp,_Alloc>{
          
          typedef _Vector_base<_Tp,_Alloc>                  _Base;
          typedef typename _Base::_Tp_alloc_type            _Tp_alloc_type;
          typedef typename YS_STL::__alloc_traits<_Tp_alloc_type>   _Alloc_traits;
          
          public:
            typedef _Tp                                     value_type;
            typedef typename _Alloc_traits::pointer         pointer;
            typedef typename _Alloc_traits::const_pointer   const_pointer;
            typedef typename _Alloc_traits::reference       reference;
            typedef typename _Alloc_traits::const_reference const_reference;
            typedef size_t                                  size_type;
            typedef ptrdiff_t                               difference_type;
            typedef _Alloc                                  allocator_type;
            typedef YS_STL::__normal_iterator<pointer,vector>       iterator;
            typedef YS_STL::__normal_iterator<const_pointer,vector> const_iterator;
            typedef YS_STL::reverse_iterator<iterator>              reverse_iterator;
            typedef YS_STL::reverse_iterator<const_iterator>        const_reverse_iterator;

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

            vector(size_type _n, const value_type& _value, 
                const allocator_type& _a = allocator_type())
            : _Base(_n,_a)
            { _M_fill_initialize(_n, _value); }

            vector(const vector& _x) 
            : _Base(_x.size(), _x._M_get_Tp_allocator()){
                _memory_pool._M_finish = uninitialized_copy(
                    _x.begin(), _x.end(), this->_memory_pool._M_start,
                    _M_get_Tp_allocator()
                );
            }

            vector(vector&& _x) noexcept
            : _Base(YS_STL::move(_x))
            { }

            vector(initializer_list<value_type> _l, const allocator_type& _a = allocator_type())
            : _Base(_a)
            {
                // Call _M_range_initialize with forward_iterator_tag parameters
                _M_range_initialize(_l.begin(), _l.end(), random_access_iterator_tag());  
            }
            
            // pay attention to this constuctor and 
            // vector(size_type _n, const value_type& _value, const allocator_type& _a = allocator_type())
            // how to distinguish one from another?

            // if _InputIterator is int , _RequireInputIterator in template will raise error
            // Maybe it's the reason.
            template<typename _InputIterator,
                typename = YS_STL::_RequireInputIter<_InputIterator>>
            vector(_InputIterator _first, _InputIterator _last,
                    const allocator_type& _a = allocator_type())
            : _Base(_a)
            { _M_initialize_dispatch(_first, _last); }

            ~vector() noexcept{
                YS_STL::_destory(_memory_pool._M_start, _memory_pool._M_finish);
                _Alloc_traits::deallocate(_memory_pool,
                    _memory_pool._M_start,
                    _memory_pool._M_end_of_storage - _memory_pool._M_start);
            }
            


            // operator override

            // It's defined outside the class;
            // It has not been completed.
            vector&
            operator=(const vector& _x);
            
            vector&
            operator=(initializer_list<value_type> _l){
                // Don't use _M_range_initialize.
                // Because it can avoid allocating new memory
                _M_assign_aux(_l.begin(),_l.end(),random_access_iterator_tag());
            }

            reference
            operator[](size_type _n) noexcept
            { return *(_memory_pool._M_start + _n); }

            const_reference
            operator[](size_type _n) const noexcept
            { return *(_memory_pool._M_start + _n); }

            void assign(size_type _n, const value_type& _val){
                _M_fill_assign(_n,_val);
            }

            template<typename _InputIterator,
                    typename = YS_STL::_RequireInputIter<_InputIterator>>
            void
            assign(_InputIterator _first, _InputIterator _last){
                _M_assign_dispatch(_first,_last);
            }

            void
            assign(initializer_list<value_type> _l){
                _M_assign_aux(_l.begin(),_l.end(),random_access_iterator_tag());
            }

            iterator
            begin() noexcept
            { return iterator(_memory_pool._M_start); }

            const_iterator
            begin() const noexcept
            { return const_iterator(_memory_pool._M_start); }

            iterator
            end() noexcept
            { return iterator(_memory_pool._M_finish); }

            const_iterator
            end() const noexcept
            { return const_iterator(_memory_pool._M_finish); }

            reverse_iterator
            rbegin() noexcept
            { return reverse_iterator(end()); }

            const_reverse_iterator
            rbegin() const noexcept
            { return const_reverse_iterator(end()); }

            reverse_iterator
            rend() noexcept
            { return reverse_iterator(begin()); }
            
            const_reverse_iterator
            rend() const noexcept
            { return const_reverse_iterator(begin()); }

            const_iterator
            cbegin() const{
                return const_iterator(_memory_pool._M_start);
            }

            const_iterator
            cend() const{
                return const_iterator(_memory_pool._M_finish);
            }

            size_type
            capacity() const noexcept
            { return size_type(_memory_pool._M_end_of_storage - _memory_pool._M_start); }

            size_type
            size() const noexcept
            { return size_type(_memory_pool._M_finish - _memory_pool._M_start); }

            bool
            empty() const noexcept
            { return begin() == end(); }

            // max_size = size_t(-1) / sizeof(_Tp)
            size_type
            max_size() const noexcept
            { return _Alloc_traits::max_size(_M_get_Tp_allocator()); }

            void
            resize(size_type _new_size)
            { 
                if(_new_size > size())
                    _M_default_append(_new_size - size());
                else if(_new_size < size())
                    _M_erase_at_end(_memory_pool._M_start + _new_size);
            }

            void
            resize(size_type _new_size, const value_type& _x)
            {
                if(_new_size > size())
                    _M_fill_insert(end(),_new_size - size(), _x); 
                else if(_new_size < size())
                    _M_erase_ad_end(_memory_pool._M_start + _new_size);
            }

            // preallocate _n size memory
            void
            reserve(size_type _n);

            reference
            at(size_type _n){
                _M_range_check(_n);
                return (*this)[_n];
            }

            const_reference
            at(size_type _n) const{
                _M_range_check(_n);
                return (*this)[_n];
            }

            reference
            front() noexcept
            { return *begin(); }

            const_reference
            front() const noexcept
            { return *begin(); }

            reference
            back() noexcept
            { return *(end()-1); }

            const_reference
            back() const noexcept
            { return *(end()-1); }
            
            void
            push_back(const value_type& _x){
                if(_memory_pool._M_finish != _memory_pool._M_end_of_storage){
                    _Alloc_traits::construct(_memory_pool,_memory_pool._M_finish,_x);
                    ++_memory_pool._M_finish;
                }
                else
                    _M_realloc_insert(end(),_x);
            }
            
            // pop_back() how to deal with the situation
            // when _M_finish == nullptr
            // In std::vector, _M_finish will be size_type(0-sizeof(_Tp))
            // but no exception thrown out.
            void
            pop_back(){
                --_memory_pool._M_finish;
                _Alloc_traits::destory(_memory_pool, _memory_pool._M_finish);
            }

            template<typename... _Args>
            void
            emplace_back(_Args&&... _args){
                if(_memory_pool._M_finish != _memory_pool._M_end_of_storage){
                    _Alloc_traits::construct(_memory_pool, _memory_pool._M_finish,
                            std::forward<_Args>(_args)...);
                    ++_memory_pool._M_finish;
                }
                else
                    _M_realloc_insert(end(),std::forward<_Args>(_args)...);
            }

            template<typename... _Args>
            iterator
            emplace(const_iterator _pos, _Args&&... _args){
                return _M_emplace_aux(_pos, std::forward<_Args>(_args)...);
            }

            iterator
            insert(const_iterator _pos, const value_type& _x){
                const size_type _n = _pos - begin();
                if(_memory_pool._M_finish != _memory_pool._M_end_of_storage){
                    if(_pos == _memory_pool._M_finish){
                        _Alloc_traits::construct(_memory_pool,_memory_pool._M_finish,
                                _x);
                        ++_memory_pool._M_finish;
                    }
                    else{
                        _Temporary_value _x_copy(_x);
                        _M_insert_aux(_pos, move(_x_copy._M_val()));
                    }
                }
                else
                    _M_realloc_insert(begin() + (_pos-begin()), _x);
                return iterator(_memory_pool._M_start + _n);
            }

            /**
             * insert a number of copies of given data into vector
             * @brief  Inserts a number of copies of given data into the %vector.
             * @param  __position  A const_iterator into the %vector.  
             * @param  __n  Number of elements to be inserted.
             * @param  __x  Data to be inserted.
             * @return  An iterator that points to the inserted data.
             */
            iterator
            insert(const_iterator _pos, size_type _n, const value_type& _x){
                difference_type _l = _pos - begin();
                _M_fill_insert(begin() + _l, _n, _x);
                return begin() + _l;
            }

            template<typename _InputIterator, 
                typename = YS_STL::_RequireInputIter<_InputIterator>>
            iterator
            insert(const_iterator _pos, 
                    _InputIterator _first, _InputIterator _last)
            {
                difference_type _n = _pos - cbegin();
                _M_insert_dispatch(begin() + _n, _first, _last);
                return begin() + _n;
            }

            iterator
            erase(iterator _pos)
            { return _M_erase(_pos); }

            iterator
            erase(iterator _first, iterator _last){
                return _M_erase(_first, _last);
            }

            void swap(vector& _x) noexcept{
                _memory_pool._M_swap_data(_x._memory_pool);
            }

            void clear() noexcept
            { _M_erase_at_end(_memory_pool._M_start); }

            /**
             *  Vector interal functions invoked by funtions above
             */
          private:
            struct _Temporary_value;
            /**
             * Internal constructor functions follow
             * These function are about memory allocation and construction
             */
            // invoke fill_n else construct elements one by one
            // Called by the vector(n,value) constructor
            void _M_fill_initialize(size_type _n, const value_type& _value){
                _memory_pool._M_finish = uninitialized_fill_n(
                    _memory_pool._M_start, _n, _value, _M_get_Tp_allocator()
                );  
            }


            // _InputIterator uses _first != _last as loop condition and use emplace_back
            // invoke copy else construct elements one by one
            // Called by the initialized_dispatch
            template<typename _InputIterator>
            void
            _M_range_initialize(_InputIterator _first, _InputIterator _last,
                                YS_STL::input_iterator_tag)
            {
                // Only be invoked within constructor
                // So _M_finish is nullptr
                try{    
                    for(; _first != _last; ++_first)
                        emplace_back(*_first);          // this function can allocate memory
                }
                catch(...){
                    clear();
                    throw;
                }
            }

            // _ForwardIterator
            // why ForwardIterator has greate differences from InputIterator?
            // Maybe the distance function and uninitialized_copy function.
            // If so, why input iterator can't use the same implementation instead 
            // of using emplace_back.                 
            // Called by the initialized_dispatch
            template<typename _ForwardIterator>
            void 
            _M_range_initialize(_ForwardIterator _first, _ForwardIterator _last,
                                YS_STL::forward_iterator_tag)
            {   
                // if _ForwardIterator is _RandomAccessIterator, the most effective
                // distance function will be invoked
                const size_type _n = YS_STL::distance(_first,_last);
                _memory_pool._M_start = _M_allocate(_n);
                _memory_pool._M_end_of_storage = _memory_pool._M_start + _n;
                _memory_pool._M_finish = uninitialized_copy(
                        _first, _last, _memory_pool._M_start,_M_get_Tp_allocator()
                    );
            }
            

            // invoke fill_n else construct elements one by one
            // We call it default because it called value_tpye's default constructor
            // Called by the vector(n) constructor
            void _M_default_initialize(size_type _n){
                // _M_start, _M_finish  and _M_end_of_storage have been
                // set at _M_create_storage
                _memory_pool._M_finish = 
                    uninitialized_default_n(_memory_pool._M_start, _n, _M_get_Tp_allocator());
            }
            

            
            // parameters are iterator type
            // two situations: InputIterator and ForwardIterator
            // use _M_range_initialize with different implementations
            // Called by the range constructor
            template<typename _InputIterator>
            void
            _M_initialize_dispatch(_InputIterator _first, _InputIterator _last){
                typedef typename iterator_traits<_InputIterator>::iterator_category iterator_category;
                _M_range_initialize(_first,_last,iterator_category());
            }

            /**
             * Following functions are for assign()
             */

            template<typename _InputIterator>
            void
            _M_assign_aux(_InputIterator _first, _InputIterator _last, input_iterator_tag){
                iterator _cur = begin();
                for(; _first != _last && _cur != end(); ++_first, ++_cur)
                    *_cur = *_first;
                if(_first == _last)
                    _M_erase_at_end(_cur);
                else
                    insert(cend(),_first,_last);
            }
            template<typename _ForwardIterator>
            void
            _M_assign_aux(_ForwardIterator _first, _ForwardIterator _last, forward_iterator_tag){
                const size_type _len = YS_STL::distance(_first,_last);
                if(_len < size())
                    _M_erase_at_end(YS_STL::copy(_first,_last,_memory_pool._M_start));
                else
                {
                    _ForwardIterator _mid = _first;
                    YS_STL::advance(_mid, size());
                    YS_STL::copy(_first,_mid, _memory_pool._M_start);
                    insert(cend(),_mid,_last);
                }
            }

            template<typename _InputIterator>
            void
            _M_assign_dispatch(_InputIterator _first, _InputIterator _last){
                _M_assign_aux(_first, _last, 
                    typename iterator_traits<_InputIterator>::iterator_category());
            }
            
            void
            _M_fill_assign(size_type _n, const value_type& _val){
                if(_n > capacity()){
                    // allocate new area for memory pool
                    vector _tmp(_n,_val,_M_get_Tp_allocator());
                    _tmp._memory_pool._M_swap_data(_memory_pool);
                }
                else if(_n > size()){
                    YS_STL::fill_n(_memory_pool._M_start,_n,_val);
                    _memory_pool._M_finish = uninitialized_fill_n(
                        _memory_pool._M_finish, _n - size(), _val,_M_get_Tp_allocator()
                    );
                }
                else
                    _M_erase_at_end(YS_STL::fill_n(_memory_pool._M_start,_n,_val));
            }

            void
            _M_range_check(size_type _n) const{
                // size_type can't be less than zero
                if(_n > size())
                    throw _VECTOR_M_RANGE_CHECK_ERROR;
            }

            void
            _M_erase_at_end(pointer _pos) noexcept{
                YS_STL::_destory(_pos, _memory_pool._M_finish);
                _memory_pool._M_finish = _pos;
            }

            /**
             * append n size at end
             * if the memory left is not enough, it will allocate new memory
             * and copy or move elements in the origin vector.
             */
            void
            _M_default_append(size_type _n){
                if(_n != 0){
                    if(size_type(_memory_pool._M_end_of_storage
                        - _memory_pool._M_finish) >= _n)
                    {
                        _memory_pool._M_finish = uninitialized_default_n(
                            _memory_pool._M_finish, _n, _M_get_Tp_allocator()
                        );
                    }
                    else{
                        const size_type _len = _M_check_len(_n);
                        const size_type _size = size();
                        pointer _new_start = _M_allocate(_len);
                        pointer _destory_from = pointer(nullptr);
                        try{
                            // Why not use copy(_pos,_memory_pool._M_finish,_pos+_n)?
                            // We should know that memory whithin [_M_finish, _M_end_of_storage)
                            // is uninitialized, but functions such as copy, fill, move
                            // and so on should have to work on an initialized memory otherwise
                            // they may raise errors. Because they do not call constructor. It works
                            // well only the objects' are POD type.
                            // However, functions such as uninitialized_fill, uninitialized_move and
                            // so on can work on uninitialized memory. They will call constructor.
                            uninitialized_default_n(
                                _new_start + _size, _n, _M_get_Tp_allocator()
                            );
                            _destory_from = _new_start + _size;
                            uninitialized_move_if_noexcept(
                                _memory_pool._M_start, _memory_pool._M_finish,
                                _new_start, _M_get_Tp_allocator()
                            );
                        }
                        catch(...){
                            // if uninitialized_move_if_noexcept throw exception
                            if(_destory_from)
                                _destory(_destory_from, _destory_from + _n);
                            _M_deallocate(_new_start, _len);
                            throw _VECTOR_M_DEFAULT_APPEND_ERROR;
                        }
                        _destory(_memory_pool._M_start, _memory_pool._M_finish);
                        _M_deallocate(_memory_pool._M_start, 
                                    _memory_pool._M_end_of_storage - _memory_pool._M_start);
                        _memory_pool._M_start = _new_start;
                        _memory_pool._M_finish = _new_start + _size + _n;
                        _memory_pool._M_end_of_storage = _new_start + _len;
                    }
                }
            }

            /**
             * @brief return new size of vector, because it is not large enough to 
             * contain more n elements
             * @param _n    number of elements to insert
             * @return  new size of memory_pool
             */
            size_type
            _M_check_len(size_type _n) const{
                if(max_size() - size() < _n)
                    throw _VECTOR_M_CHECK_LEN_ERROR;
                const size_type _len = size() + YS_STL::max(size(), _n);
                // deal with overflow
                // or greater than max_size() but isn't overflow
                return (_len < size() || _len > max_size())?max_size():_len;
            }

            /**
             * insert some same elements at given position
             * If there's no enough area, it will allocate new memory.
             * @param _pos  an iterator indicate the position to insert elements
             * @param _n    number of elements to insert
             * @param _x    sample of elements
             */ 
            void
            _M_fill_insert(iterator _pos, size_type _n, const value_type& _x){
                if(_n == 0) return;
                if(size_type(capacity() - end()) >= _n){
                    value_type _x_copy = _x;
                    const size_type _elements_after = end() - _pos;
                    pointer _old_finish(_memory_pool._M_finish);
                    if(_elements_after > _n){
                        YS_STL::uninitialized_move(
                            _memory_pool._M_finish - _n, _memory_pool._M_finish,
                            _memory_pool._M_finish, _M_get_Tp_allocator());
                        _memory_pool._M_finish += _n;
                        YS_STL::move_backward(_pos.base(), _old_finish - _n,
                                            _old_finish);
                        YS_STL::fill(_pos.base(), _pos.base()+_n, _x_copy);
                    }
                    else{
                        _memory_pool._M_finish = 
                        uninitialized_fill_n(_memory_pool._M_finish,
                            _n - _elements_after,_x_copy, _M_get_Tp_allocator());
                        uninitialized_move(_pos.base(), _old_finish,
                                _memory_pool._M_finish, _M_get_Tp_allocator());
                        _memory_pool._M_finish += _elements_after;
                        YS_STL::fill(_pos.base(), _old_finish, _x_copy);
                    }
                }
                else{
                    const size_type _len = _M_check_len(_n);
                    const size_type _elements_before = _pos - begin();
                    pointer _new_start = _M_allocate(_len);
                    pointer _new_finish(_new_start);

                    try{
                        // (1)
                        uninitialized_fill_n(_new_start + _elements_before,
                            _n, _x, _M_get_Tp_allocator());
                        _new_finish = pointer();

                        // (2)
                        _new_finish = uninitialized_move_if_noexcept(
                            _memory_pool._M_start, _pos.base(), _new_start, 
                            _M_get_Tp_allocator()
                        );
                        _new_finish += _n;

                        // (3)
                        _new_finish = uninitialized_move_if_noexcept(
                            _pos.base(), _memory_pool._M_finish,
                            _new_finish, _M_get_Tp_allocator()
                        );
                    }
                    catch(...){
                        // how to deal with (3)'s exception?
                        // (1) throw exception
                        if(!_new_finish)
                            _destory(_new_start + _elements_before,
                                _new_start + _elements_before + _n);
                        // (2) throw exception
                        else
                            _destory(_new_start, _new_finish);
                        
                        _M_deallocate(_new_start, _new_finish);
                        throw _VECTOR_M_FILL_INSERT_ERROR;
                    }
                    // destory objects in origin memory_pool
                    // and deallocate the memory
                    // but how does destory work if the objects were moved
                    _destory(_memory_pool._M_start, _memory_pool._M_finish);
                    _M_deallocate(_memory_pool._M_start,
                        _memory_pool._M_end_of_storage - _memory_pool._M_start);
                    _memory_pool._M_start = _new_start;
                    _memory_pool._M_finish = _new_finish;
                    _memory_pool._M_end_of_storage = _new_start + _len;
                }

            }
            
            /**
             * allocate new memory to insert one elements
             * @param _pos  an iterator indicate the position to insert the element
             * @param _args arguments for constructor
             */
            template<typename... _Args>
            void
            _M_realloc_insert(iterator _pos, _Args&&... _args){
                const size_type _len = _M_check_len(size_type(1));
                const size_type _elems_before = _pos - begin();
                pointer _new_start = _M_allocate(_len);
                pointer _new_finish = _new_start;
                try{
                    _Alloc_traits::construct(_memory_pool,
                        _new_start + _elems_before,
                        std::forward<_Args>(_args)...);

                    _new_finish = pointer();

                    _new_finish = uninitialized_move_if_noexcept(
                        _memory_pool._M_start, _pos.base(), _new_start,
                        _M_get_Tp_allocator()
                    );

                    ++_new_finish;

                    _new_finish = uninitialized_move_if_noexcept(
                        _pos.base(), _memory_pool._M_finish, _new_finish,
                        _M_get_Tp_allocator()
                    );
                }
                catch(...){
                    if(!_new_finish)
                        _Alloc_traits::destory( _memory_pool,
                            _new_start + _elems_before);
                    else
                        _destory(_new_start, _new_finish);
                    _M_deallocate(_new_start, _len);
                    throw _VECTOR_M_REALLOC_INSERT_ERROR;
                }
                _destory(_memory_pool._M_start, _memory_pool._M_finish);
                _M_deallocate(_memory_pool._M_start, 
                    _memory_pool._M_end_of_storage - _memory_pool._M_start);
                _memory_pool._M_start = _new_start;
                _memory_pool._M_finish = _new_finish;
                _memory_pool._M_end_of_storage = _new_start + _len;
            }

            template<typename... _Args>
            iterator
            _M_emplace_aux(const_iterator _pos, _Args... _args){
                const size_type _n = _pos.base() - begin();
                if(_memory_pool._M_finish != _memory_pool._M_end_of_storage)
                    if(_pos.base() == end()){
                        _Alloc_traits::construct(_memory_pool,
                            _memory_pool._M_finish, std::forward<_Args>(_args)...);
                        ++_memory_pool._M_finish;
                    }
                    else{
                        // We need to construct a temporary because something in __args...
                        // could alias one of the elements of the container and so we
                        // need to use it before _M_insert_aux moves elements around.

                        // what the words above mean ?
                        _Temporary_value _tmp(this, std::forward<_Args>(_args)...);
                        _M_insert_aux(begin() + _n, std::move(_tmp._M_val()));
                    }
                else
                    _M_realloc_insert(begin() + _n, std::forward<_Args>(_args)...);        
            }

            template<typename _InputIterator>
            void
            _M_insert_dispatch(iterator _pos, _InputIterator _first,
                        _InputIterator _last)
            {
                _M_range_insert(_pos, _first, _last, 
                    typename iterator_traits<_InputIterator>::iterator_category());
            }

            template<typename _InputIterator>
            void
            _M_range_insert(iterator _pos, _InputIterator _first,
                    _InputIterator _last, input_iterator_tag)
            {
                for(; _first != _last; ++_first){
                    _pos = insert(_pos, *_first);
                    ++_pos;
                }
            }

            template<typename _ForwardIterator>
            void
            _M_range_insert(iterator _pos, _ForwardIterator _first,
                    _ForwardIterator _last, forward_iterator_tag)
            {   
                if(_first == _last) return;
                const size_type _n = distance(_first,_last);
                if(size_type(_memory_pool._M_end_of_storage 
                    - _memory_pool._M_finish) >= _n){
                    const size_type _elems_after = end() - _pos;
                    pointer _old_finish(_memory_pool._M_finish);
                    if(_elems_after > _n){
                        uninitialized_move(_memory_pool._M_finish - _n,
                            _memory_pool._M_finish, _memory_pool._M_finish,
                            _M_get_Tp_allocator());
                        _memory_pool._M_finish += _n;
                        move_backward(_pos.base(), _old_finish - _n, _old_finish);
                        copy(_first,_last,_pos.base());
                    }
                    else{
                        _ForwardIterator _mid = _first;
                        advance(_mid,_elems_after);
                        uninitialized_copy(_mid,_last,_memory_pool._M_finish,
                            _M_get_Tp_allocator());
                        _memory_pool._M_finish += _n - _elems_after;
                        uninitialized_move(_pos.base(),_old_finish, _memory_pool._M_finish,
                            _M_get_Tp_allocator());
                        _memory_pool._M_finish += _elems_after;
                        copy(_first,_mid,_pos.base());
                    }
                }
                else{
                    const size_type _len = _M_check_len(_n);
                    pointer _new_start(_M_allocate(_len));
                    pointer _new_finish(_new_start);
                    try{
                        _new_finish = uninitialized_move_if_noexcept(
                            _memory_pool._M_start, _pos.base(),_new_start,
                            _M_get_Tp_allocator()
                        );
                        _new_finish = uninitialized_copy(_first, _last,
                            _new_finish, _M_get_Tp_allocator());
                        _new_finish = uninitialized_move(_pos,end(),
                            _new_finish, _M_get_Tp_allocator());
                    }
                    catch(...){
                        _destory(_new_start,_new_finish);
                        _M_deallocate(_new_start,_len);
                        throw _VECTOR_M_RANGE_INSERT_ERRER;
                    }
                    _destory(_memory_pool._M_start,_memory_pool._M_finish);
                    _M_deallocate(_memory_pool._M_start, 
                            _memory_pool._M_end_of_storage - _memory_pool._M_start);
                    _memory_pool._M_start = _new_start;
                    _memory_pool._M_finish = _new_finish;
                    _memory_pool._M_end_of_storage = _new_start + _len;
                }
            }

            iterator
            _M_erase(iterator _pos){
                if(_pos + 1 != end())
                    move(_pos+1, end(), _pos.base());
                --_memory_pool._M_finish;
                _Alloc_traits::destory(_memory_pool,_memory_pool._M_finish);
                return _pos;
            }

            iterator
            _M_erase(iterator _first, iterator _last){
                if(_first == _last) return _first;
                if(_last != end())
                    move(_last,end(),_first.base());
                _M_erase_at_end(_first.base() + (end() - _last));
            }

            iterator
            _M_insert_rval(const_iterator _pos,value_type&& _v){
                const size_type _n = _pos.base() - begin();
                if(_memory_pool._M_finish != _memory_pool._M_end_of_storage){
                    if(_pos == end()){
                        _Alloc_traits::construct(_memory_pool,
                                _memory_pool._M_finish, move(_v));\
                        ++_memory_pool._M_finish;
                    }
                    else
                        _M_insert_aux(begin() + _n, move(_v));
                }
                else{
                    _M_realloc_insert(begin() + _n, move(_v));
                }
                return iterator(_memory_pool._M_start + _n);
            }

            template<typename _Arg>
            void
            _M_insert_aux(iterator _pos, _Arg&& _arg){
                _Alloc_traits::construct(_memory_pool,_memory_pool._M_finish,
                        move(*(_memory_pool._M_finish - 1)));
                ++_memory_pool._M_finish;
                move_backward(_pos.base(),_memory_pool._M_finish - 2,
                    _memory_pool._M_finish - 1);
                *_pos = std::forward<_Arg>(_arg);
            }

            template<typename _ForwardIterator>
            pointer
            _M_allocate_and_copy(size_type _n,
                    _ForwardIterator _first, _ForwardIterator _last)
            {
                pointer _result = _M_allocate(_n);
                try{
                    uninitialized_copy(_first,_last,_result,_M_get_Tp_allocator());
                    return _result;
                }            
                catch(...)
                {
                    _M_deallocate(_result,_n);
                    throw _VECTOR_M_ALLOCATE_AND_COPY_ERROR;
                }
            }
      };   
        template<typename _Tp, typename _Alloc>
        struct vector<_Tp,_Alloc>::_Temporary_value
        {
            public:
            template<typename... _Args>
            explicit
            _Temporary_value(_Args&&... _args){
                _Alloc_traits::construct(_memory_pool, _M_ptr(),
                    std::forward<_Args>(_args)...);
            }
            ~_Temporary_value(){
                _Alloc_traits::destory(_memory_pool, _M_ptr());
            }

            value_type&
            _M_val() { return *reinterpret_cast<_Tp*>(&__buf); }

            private:
            pointer
            _M_ptr() { return &_M_val(); }

            typename std::aligned_storage<sizeof(_Tp),alignof(_Tp)>::type __buf;
        };

        template<typename _Tp, typename _Alloc>
        void vector<_Tp, _Alloc>::
        reserve(size_type _n){
            if(_n > max_size())
                throw _VECTOR_RESERVE_ERROR;
            if(capacity() < _n){
                const size_type _old_size = size();
                pointer _tmp = _M_allocate_and_copy(_n,
                    _make_move_if_noexcept_iterator(_memory_pool._M_start),
                    _make_move_if_noexcept_iterator(_memory_pool._M_finish)
                );
                _destory(_memory_pool._M_start, _memory_pool._M_end_of_storage);
                _M_deallocate(_memory_pool._M_start,
                            _memory_pool._M_end_of_storage - _memory_pool._M_start);
                _memory_pool._M_start = _tmp;
                _memory_pool._M_finish = _tmp + _old_size;
                _memory_pool._M_end_of_storage = _memory_pool._M_start + _n;
            }
        }

        template<typename _Tp, typename _Alloc>
          vector<_Tp,_Alloc>&
          vector<_Tp,_Alloc>::
          operator=(const vector<_Tp, _Alloc>& _x){
              if(&_x == this) return (*this);
              const size_type _xlen = _x.size();
              if(_xlen > capacity()){
                pointer _tmp = _M_allocate_and_copy(_xlen,_x.begin(),_x.end());
                _destory(_memory_pool._M_start, _memory_pool._M_finish);
                _M_deallocate(_memory_pool._M_start,
                        _memory_pool._M_end_of_storage, _memory_pool._M_start);
                _memory_pool._M_start = _tmp;
                _memory_pool._M_end_of_storage = _memory_pool._M_start + _xlen;
              }
              else if(size() >= _xlen){
                  _destory(copy(_x.begin(),_x.end(),begin()),end());
              }
              else{
                  copy(_x._memory_pool._M_start,_x._memory_pool._M_start + size(),
                        _memory_pool._M_start);
                  uninitialized_copy(_x._memory_pool._M_start + size(),
                                _x._memory_pool._M_finish,
                                _memory_pool._M_finish,
                                _M_get_Tp_allocator());
              }
              _memory_pool._M_finish = _memory_pool._M_start + _xlen;
              return *this;
          }
}

#endif