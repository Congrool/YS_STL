#ifndef _ITERATOR_H
#define _ITERATOR_H 1

#include "ys_iterator_base_types.hpp"
#include "ys_iterator_base_funcs.hpp"


namespace YS_STL{

    // iterator adapters, including
    // reverse_iterator
    // insert_iterator: back_insert_iterator, front_insert_iterator, insert_iterator
    // move_iterator

    /**
     * reverse_iterator can make iterator have reversal operation
     */
    // _Iterator type should override +=, -=, +, -, ++, --, *, ==, <
    template<typename _Iterator>
    class reverse_iterator
    : public iterator_base<typename iterator_traits<_Iterator>::iterator_category,
        typename iterator_traits<_Iterator>::value_type,
        typename iterator_traits<_Iterator>::difference_type,
        typename iterator_traits<_Iterator>::pointer,
        typename iterator_traits<_Iterator>::reference>
    {
        protected:
            _Iterator current;

            typedef iterator_traits<_Iterator>  __traits_type;
        
        public:
            typedef _Iterator                               iterator_type;
            typedef typename __traits_type::difference_type difference_type;
            typedef typename __traits_type::pointer         pointer;
            typedef typename __traits_type::reference       reference;


            reverse_iterator() : current() { }

            explicit
            reverse_iterator(iterator_type _x) : current(_x) { }

            reverse_iterator(const reverse_iterator& _x) : current(_x.base()) { }

            template<typename _Iter>
            reverse_iterator(const reverse_iterator<_Iter>& _x) : current(_x.base()) { }


            // override some operators in class, including
            // * -> + ++ += - -- -= []

            // other operators like '<' are overrode out of class

            // return a new iterator same as current
            iterator_type
            base() const{
                return current;
            }

            reference
            operator*() const{
                _Iterator _tmp = current;
                return *--_tmp;             // because it's reverse
            }

            pointer
            operator->()const{
                return &(operator*());
            }

            reverse_iterator&
            operator++(){
                --current;
                return *this;
            }

            reverse_iterator
            operator++(int){
                reverse_iterator _tmp = *this;
                --current;
                return _tmp;
            }

            reverse_iterator&
            operator--(){
                ++current;
                return *this;
            }

            reverse_iterator
            operator--(int){
                iterator_type _tmp = *this;
                ++current;
                return _tmp;
            }

            reverse_iterator
            operator+(difference_type _diff) const{
                return reverse_iterator(current - _diff);
            }

            reverse_iterator&
            operator+=(difference_type _diff){
                current -= _diff;
                return *this;
            }

            reverse_iterator
            operator-(difference_type _diff) const {
                return reverse_iterator(current + _diff);
            }

            reverse_iterator&
            operator-=(difference_type _diff){
                current += _diff;
                return *this;
            }

            reference
            operator[](difference_type _diff) const{
                return *(*this + _diff);
            }
    };

    template<typename _Iterator>
        inline bool operator==(const reverse_iterator<_Iterator>&_x,
            const reverse_iterator<_Iterator>& _y)
    { return _x.base() == _y.base(); }

    template<typename _Iterator>
        inline bool operator<(const reverse_iterator<_Iterator>& _x,
            const reverse_iterator<_Iterator>& _y)
    { return _x.base() < _y.base(); }

    template<typename _Iterator>
        inline bool operator!=(const reverse_iterator<_Iterator>& _x,
            const reverse_iterator<_Iterator>& _y)
    { return !(_x == _y); }

    template<typename _Iterator>
        inline bool operator>(const reverse_iterator<_Iterator>& _x,
            const reverse_iterator<_Iterator>& _y)
    { return _y < _x; }

    template<typename _Iterator>
        inline bool operator<=(const reverse_iterator<_Iterator>& _x,
            const reverse_iterator<_Iterator>& _y)
    { return !(_y < _x); }

    template<typename _Iterator>
        inline bool operator>=(const reverse_iterator<_Iterator>& _x,
            const reverse_iterator<_Iterator>& _y)
    { return !(_x < _y); }

    /**
     * Here should be some same name functions as above, 
     * but have two template parameters.
     * I don't know where to use them.
     */

    template<typename _Iterator>
        inline auto operator-(const reverse_iterator<_Iterator>& _x,
            const reverse_iterator<_Iterator>& _y)
            ->decltype(_y.base() - _x.base())
        { return _y.base() - _x.base(); }   // If you don't know why _y - _x here, remember it's reverse

    
    // TODO: rest unfinished adapter classes
    // back_insert_iterator
    // front_insert_iterator
    // insert_iterator
}



/**
 * normal iterator
 */
namespace YS_STL{

    template<typename _Iterator, typename _Container>
      class __normal_iterator{
        protected:
          _Iterator _current;

          typedef iterator_traits<_Iterator>    _traits_type;

        public:
          typedef _Iterator                                 iterator_type;
          typedef typename _traits_type::iterator_category  iterator_category;
          typedef typename _traits_type::pointer            pointer;
          typedef typename _traits_type::reference          reference;
          typedef typename _traits_type::difference_type    difference_type;
          typedef typename _traits_type::value_type         value_type;

          constexpr __normal_iterator() noexcept : _current(_Iterator()) { }
          
          explicit __normal_iterator(const _Iterator& _x) noexcept : _current(_x) { }

          reference
          operator*() const noexcept
          { return *_current; }

          pointer
          operator->() const noexcept
          { return &_current; }

          __normal_iterator&
          operator++(){
              ++_current;
              return *this;
          }
          __normal_iterator
          operator++(int){
              return __normal_iterator(_current++);
          }

          __normal_iterator&
          operator--(){
              --_current;
              return *this;
          }

          __normal_iterator
          operator--(int){
              return __normal_iterator(_current--);
          }

          __normal_iterator
          operator+(difference_type _diff){
              return __normal_iterator(_current + _diff);
          }

          __normal_iterator&
          operator+=(difference_type _diff){
              _current += _diff;
              return *this;
          }

          __normal_iterator
          operator-(difference_type _diff){
              return __normal_iterator(_current - _diff);
          }

          __normal_iterator&
          operator-=(difference_type _diff){
              _current -= _diff;
              return *this;
          }

          const _Iterator&
          base() const noexcept
          { return _current; }
      };

      template<typename _Iterator, typename _Container>
      inline bool
      operator==(const __normal_iterator<_Iterator, _Container>& _lhs,
        const __normal_iterator<_Iterator, _Container>& _rhs){
            return _lhs.base() == _rhs.base();
        }

      template<typename _Iterator, typename _Container>
      inline bool
      operator!=(const __normal_iterator<_Iterator, _Container>& _lhs,
        const __normal_iterator<_Iterator, _Container>& _rhs){
            return _lhs.base() != _rhs.base();
        }

      template<typename _Iterator, typename _Container>
      inline bool
      operator<(const __normal_iterator<_Iterator, _Container>& _lhs,
        const __normal_iterator<_Iterator,_Container>& _rhs){
            return _lhs.base() < _rhs.base();
        }

      template<typename _Iterator, typename _Container>
      inline bool
      operator>(const __normal_iterator<_Iterator, _Container>& _lhs,
        const __normal_iterator<_Iterator,_Container>& _rhs){
            return _lhs.base() > _rhs.base();
        }
      
      template<typename _Iterator, typename _Container>
      inline bool
      operator<=(const __normal_iterator<_Iterator, _Container>& _lhs,
        const __normal_iterator<_Iterator, _Container>& _rhs){
            return _lhs.base() <= _rhs.base();
        }

      template<typename _Iterator, typename _Container>
      inline bool
      operator>=(const __normal_iterator<_Iterator, _Container>& _lhs,
        const __normal_iterator<_Iterator, _Container>& _rhs){
            return _lhs.base() >= _rhs.base();
        }

      template<typename _Iterator, typename _Container>
      inline auto
      operator-(const __normal_iterator<_Iterator,_Container>& _lhs,
        const __normal_iterator<_Iterator, _Container>& _rhs)
        ->decltype(_lhs.base() - _rhs.base())
        { return _lhs.base() - _rhs.base(); }
}



namespace YS_STL{
    template<typename _Iterator>
    class move_iterator
    {
      protected:
        _Iterator _M_current;  
        typedef iterator_traits<_Iterator> _traits_type;
        typedef typename _traits_type::reference _base_ref;

      public:
        typedef _Iterator                                   iterator_type;
        typedef typename _traits_type::iterator_category    iterator_category;
        typedef typename _traits_type::value_type           value_type;
        typedef typename _traits_type::difference_type      difference_type;
        typedef typename _traits_type::pointer              pointer;

        // If _base_ref is l-reference or r-reference, reference is r-reference.
        // Otherwise reference is _base_ref. ?? How can be otherwise ?
        // It's the key of move_iterator class design.
        typedef typename std::conditional<std::is_reference<_base_ref>::value,
                        typename std::remove_reference<_base_ref>::type&&,
                        _base_ref>::type                    reference;
        
        move_iterator() : _M_current() { }

        explicit
        move_iterator(iterator_type _i) : _M_current(_i) { }

        template<typename _Iter>
        move_iterator(const move_iterator<_Iter>& _i) : _M_current(_i.base()) { }

        iterator_type
        base() const
        { return _M_current; }

        reference
        operator*() const
        { return static_cast<reference>(*_M_current); }

        pointer
        operator->() const
        { return _M_current; }
        
        move_iterator&
        operator++(){ 
            ++_M_current; 
            return *this;
        }

        move_iterator
        operator++(int){
            move_iterator _tmp(*this);
            ++_M_current;
            return _tmp;
        }

        move_iterator&
        operator--(){
            --_M_current;
            return *this;
        }

        move_iterator
        operator--(int){
            move_iterator _tmp;
            --_M_current;
            return _tmp;
        }

        move_iterator
        operator+(difference_type _n) const{
            return move_iterator(_M_current + _n);
        }

        move_iterator&
        operator+=(difference_type _n) {
            _M_current += _n;
            return *this;
        }

        move_iterator
        operator-(difference_type _n) const{
            return move_iterator(_M_current - _n);
        }

        move_iterator&
        operator-=(difference_type _n){
            _M_current -= _n;
            return *this;
        }

        reference
        operator[](difference_type _n) const{
            return move(_M_current[_n]);
        }
    };

    template<typename _Iterator>
    inline bool
    operator==(const move_iterator<_Iterator>& _x,
            const move_iterator<_Iterator>& _y){
        return _x.base() == _y.base();
    }

    template<typename _Iterator>
    inline bool
    operator!=(const move_iterator<_Iterator>& _x,
            const move_iterator<_Iterator>& _y){
        return !(_x == _y);
    }

    template<typename _Iterator>
    inline bool
    operator<(const move_iterator<_Iterator>& _x,
            const move_iterator<_Iterator>& _y){
        return _x.base() < _y.base();
    }

    template<typename _Iterator>
    inline bool
    operator>(const move_iterator<_Iterator>& _x,
            const move_iterator<_Iterator>& _y){
        return _y < _x;
    }

    template<typename _Iterator>
    inline bool
    operator<=(const move_iterator<_Iterator>& _x,
            const move_iterator<_Iterator>& _y){
        return !(_x > _y);
    }

    template<typename _Iterator>
    inline bool
    operator>=(const move_iterator<_Iterator>& _x,
            const move_iterator<_Iterator>& _y){
        return !(_x < _y);
    }

    template<typename _Iterator>
    inline move_iterator<_Iterator>
    make_move_iterator(_Iterator _i){
        return move_iterator<_Iterator>(_i);
    }

    // If iterator_traits<_Iterator>::value_type has copy constructor
    // but don't have move constructor, return _Iterator
    // else return move_iterator<_Iterator>
    template<typename _Iterator,typename _ReturnType = 
      typename std::conditional<std::__and_<
          std::__not_<std::is_nothrow_move_constructible<typename iterator_traits<_Iterator>::value_type>>,
          std::is_copy_constructible<typename iterator_traits<_Iterator>::value_type>>::value,
      _Iterator, move_iterator<_Iterator>>::type>
    _ReturnType inline
    _make_move_if_noexcept_iterator(_Iterator _i) 
    {
        return _ReturnType(_i);
    }
}   

#endif