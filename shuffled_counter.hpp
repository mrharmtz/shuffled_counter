#include <array>
#include <memory>
#include <algorithm>

template <typename BaseType, typename IndexType=BaseType, typename Allocator=std::allocator<IndexType>>
class shuffled_counter
{
private:
    BaseType _value;
    IndexType* _shuffle_order;
    IndexType _size;
    Allocator _allocator;
public:

    template <typename Container>
    static BaseType next_value(BaseType value, const Container& order, bool inc_ndec=true){
        
        BaseType mask, bit;

        for (const auto& index : order)
        {
            mask = 1 << index;
            
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

    template <typename Iterator>
    shuffled_counter(Iterator begin, Iterator end, BaseType value=0, const Allocator& allocatr = Allocator())
    :_value(value), _shuffle_order(NULL), _size(std::distance(begin, end)), _allocator(allocatr){
        _shuffle_order = allocatr.allocate(_size);

        for (; begin != end; ++begin)
        {
            /* code */
        }
        

    }

    template <typename Container>
    shuffled_counter(Container container, BaseType value=0, const Allocator& allocatr = Allocator())
    :_value(value), _shuffle_order(NULL), _size(std::size(container)), _allocator(allocatr){

    }
    ~shuffled_counter(){
        delete[] _shuffle_order;
    }
};

typedef shuffled_counter<unsigned> unsigned_shuffled_counter;