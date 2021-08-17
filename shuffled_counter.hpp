#ifndef _SHUFFLE_COUNTER_HPP_
#define _SHUFFLE_COUNTER_HPP_
#include <array>
#include <memory>
#include <algorithm>

#include "range_iteration.hpp"

#define _SHUFFLE_COUNTER_DEBUG_

#ifdef _SHUFFLE_COUNTER_DEBUG_
#include <cstdio>

#define CNTR_DBG_PRINTLINE(format) printf("%s[%10d]: " format "\n", __func__, __LINE__)
#define CNTR_DBG_FORMATLINE(format, ...) printf("%s[%10d]: " format "\n", __func__, __LINE__, __VA_ARGS__)

#else

#define CNTR_DBG_PRINTLINE(format) 
#define CNTR_DBG_FORMATLINE(format, ...) 

#endif





template <typename BaseType, typename IndexType=BaseType, typename Allocator=std::allocator<IndexType>>
class shuffled_counter
{
private:
    BaseType _value;
    IndexType* _shuffle_order;
    IndexType _size;
    Allocator _allocator;

    template <typename InputIterator>
    void _allocate_shuffle_order(InputIterator cpy_order){
        _shuffle_order = _allocator.allocate(_size);

        for (IndexType* order = _shuffle_order; order != _shuffle_order + _size; ++order, ++cpy_order){
            _allocator.construct(order, *cpy_order); 
        }
    }

    void _allocate_shuffle_order(){
        _shuffle_order = _allocator.allocate(_size);

        for (IndexType* order = _shuffle_order; order != _shuffle_order + _size; ++order)
            _allocator.construct(order, 0); 
    }

public:

    template <typename Iterator>
    static BaseType next_value(BaseType value, Iterator order, Iterator end_order, bool inc_ndec=true){
        
        BaseType mask, bit;

        for (; order != end_order ; ++order)
        {
            mask = 1 << *order;
            
            if(!(value & mask)){
                value |= mask;
                if(inc_ndec) break;
            }else{
                value &= ~mask;
                if(!inc_ndec) break;
            }
        }
        
        return value;
    }

    template <typename Container>
    static BaseType next_value(BaseType value, const Container& order, bool inc_ndec=true){
        
        return next_value(value, std::cbegin(order), std::cend(order), inc_ndec);
    }

    template <typename Iterator>
    shuffled_counter(Iterator begin, Iterator end, BaseType value=0, const Allocator& allocatr = Allocator())
    :_value(value), _shuffle_order(NULL), _size(std::distance(begin, end)), _allocator(allocatr){
        
        _allocate_shuffle_order();

        CNTR_DBG_PRINTLINE("checking order");
        
        for (Iterator index = begin; index != end; ++index){

            CNTR_DBG_FORMATLINE("index = %lu, _size = %lu", *index, _size);
            
            if (*index >= _size)
                throw std::invalid_argument("index is greater than size");

            if(_shuffle_order[*index])
                throw std::invalid_argument("duplicate values in order");

            _shuffle_order[*index] = 1;
        }
            
        CNTR_DBG_PRINTLINE("order passed check, initializing _shuffle_order");

        IndexType* order = _shuffle_order;
        for (Iterator index = begin; index != end; ++index, ++order){
            *order = *index;
        }

        #ifdef _SHUFFLE_COUNTER_DEBUG_
        for (IndexType* order = _shuffle_order; order != _shuffle_order + _size; ++order)
            CNTR_DBG_FORMATLINE("shuffle_order = %lu", *order);
        #endif
    }

    template <typename Container>
    shuffled_counter(Container container, BaseType value=0, const Allocator& allocatr = Allocator())
    :_value(value), _shuffle_order(NULL), _size(std::size(container)), _allocator(allocatr){
        
        _allocate_shuffle_order();

        CNTR_DBG_PRINTLINE("checking order");
        
        for (auto index : container){

            CNTR_DBG_FORMATLINE("index = %lu, _size = %lu", index, _size);
            
            if (index >= _size)
                throw std::invalid_argument("index is greater than size");

            if(_shuffle_order[index])
                throw std::invalid_argument("duplicate values in order");

            _shuffle_order[index] = 1;
        }
            
        CNTR_DBG_PRINTLINE("order passed check, initializing _shuffle_order");

        IndexType* order = _shuffle_order;
        for (auto index : container){
            *order++ = index;
        }

        #ifdef _SHUFFLE_COUNTER_DEBUG_
        for (IndexType* order = _shuffle_order; order != _shuffle_order + _size; ++order)
            CNTR_DBG_FORMATLINE("shuffle_order = %lu", *order);
        #endif
    }

    template <typename URnd>
    shuffled_counter(IndexType size, URnd random_gen = URnd(), BaseType value=0, const Allocator& allocatr = Allocator())
    :_value(value), _shuffle_order(NULL), _size(size), _allocator(allocatr){
        _allocate_shuffle_order(range<IndexType>(_size).begin());

        std::shuffle(_shuffle_order, _shuffle_order + _size, random_gen);

        #ifdef _SHUFFLE_COUNTER_DEBUG_
        for (IndexType* order = _shuffle_order; order != _shuffle_order + _size; ++order)
            CNTR_DBG_FORMATLINE("shuffle_order = %lu", *order);
        #endif
    }

    shuffled_counter(const shuffled_counter& cpy)
    :_value(cpy._value), _shuffle_order(NULL), _size(cpy._size), _allocator(cpy._allocator){

        _allocate_shuffle_order(cpy._shuffle_order);

        #ifdef _SHUFFLE_COUNTER_DEBUG_
        for (IndexType* order = _shuffle_order; order != _shuffle_order + _size; ++order)
            CNTR_DBG_FORMATLINE("shuffle_order = %lu", *order);
        #endif
    }

    ~shuffled_counter(){
        for (IndexType* order = _shuffle_order; order != _shuffle_order + _size; ++order)
            _allocator.destroy(order);

        _allocator.deallocate(_shuffle_order, _size);
    }

    shuffled_counter& operator=(const BaseType& value){
        _value = value;
        return *this;
    }

    shuffled_counter& operator=(const shuffled_counter& value) = delete;

    operator BaseType() const{
        return _value;
    }


};

typedef shuffled_counter<unsigned> unsigned_shuffled_counter;


#endif //_SHUFFLE_COUNTER_HPP_