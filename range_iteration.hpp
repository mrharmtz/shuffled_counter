#ifndef _RANGE_ITERATOR_HPP_
#define _RANGE_ITERATOR_HPP_

//#define _RNG_ITR_DEBUG_

#ifdef _RNG_ITR_DEBUG_
#include <cstdio>

#define RNG_DBG_PRINTLINE(format) printf("%s[%10d]: " format "\n", __func__, __LINE__)
#define RNG_DBG_FORMATLINE(format, ...) printf("%s[%10d]: " format "\n", __func__, __LINE__, __VA_ARGS__)

#else

#define RNG_DBG_PRINTLINE(format) 
#define RNG_DBG_FORMATLINE(format, ...) 

#endif

template <typename BaseType, BaseType default_start=0, BaseType default_step=1>
class range_iterator{

    private:
        BaseType _start;
        BaseType _step;
        BaseType _stop;

        BaseType _value;

    public:

        range_iterator(BaseType start, BaseType step, BaseType stop)
        :_start(start), _step(step), _stop(stop), _value(_start){ 
            RNG_DBG_FORMATLINE("start = %d, step = %d, stop = %d, value = %d",_start, _step, _stop, _value);
        }

        range_iterator(const range_iterator& cpy)
        :_start(cpy._start), _step(cpy._step), _stop(cpy._stop), _value(cpy._value){
            RNG_DBG_FORMATLINE("start = %d, step = %d, stop = %d, value = %d",_start, _step, _stop, _value);
        }

        range_iterator()
        :_start(default_start), _step(default_step), _stop(default_start), _value(_start){ 
            RNG_DBG_FORMATLINE("start = %d, step = %d, stop = %d, value = %d",_start, _step, _stop, _value);
        }

        range_iterator& operator= (const range_iterator& other){
            _start = other._start;
            _step = other._step;
            _stop = other._stop;
            _value = other._value;

            return *this;
        }

        bool is_at_end() const{
            if (_step > 0)
                return (_value >= _stop);
            else
                return (_value <= _stop);
        }

        range_iterator& operator++(){
            
            if (!is_at_end())
                _value += _step;
            
            return *this;
        }

        range_iterator operator++(int){
            range_iterator ret_val(*this);
            this->operator++();
            return ret_val;
        }

        BaseType operator*()const{
            return _value;
        }

        const BaseType* operator->()const{
            return &_value;
        }

        bool operator==(const range_iterator& other)const{
            
            if(is_at_end() && other.is_at_end()){
                RNG_DBG_PRINTLINE("return true");
                return true;
            }

            if(is_at_end() ^ other.is_at_end()){
                RNG_DBG_PRINTLINE("return false");
                return false;
            }

            RNG_DBG_FORMATLINE("return %d", _value == other._value);
            return (_value == other._value);
        }

        bool operator!=(const range_iterator& other)const{
            if(is_at_end() && other.is_at_end()){
                RNG_DBG_PRINTLINE("return false");
                return false;
            }

            if(is_at_end() ^ other.is_at_end()){
                RNG_DBG_PRINTLINE("return true");
                return true;
            }
            
            RNG_DBG_FORMATLINE("return %d", _value != other._value);
            return (_value != other._value);
        }
};

template <typename BaseType = int, BaseType default_start=0, BaseType default_step=1>
class range
{
private:
    range_iterator<BaseType, default_start, default_step> _iterator_mold;
public:
    range(BaseType start, BaseType step, BaseType stop)
    :_iterator_mold(start,step,stop){ }

    range(BaseType start, BaseType stop)
    :_iterator_mold(start, default_step, stop){ }

    range(BaseType stop)
    :_iterator_mold(default_start, default_step, stop){ }

    range(const range& cpy)
    :_iterator_mold(cpy._iterator_mold){ }

    ~range(){}

    range_iterator<BaseType, default_start, default_step> begin()const {
        return _iterator_mold;
    }

    range_iterator<BaseType, default_start, default_step> end()const {
        return range_iterator<BaseType, default_start, default_step>();
    }
};


#endif