#include <stdexcept>


template <typename T>
class property_get_proxy
{
protected:
    const T& _value;
public:
    property_get_proxy(const T& value)
    :_value(value){ }

    virtual ~property_get_proxy(){ }

    virtual operator const T&()const{
        return _value;
    }
};

template <typename T>
class property_proxy{
    protected:
        T& _value;
    public:
        property_proxy(T& value)
        :_value(value){ }

        virtual ~property_proxy(){ }

        virtual operator const T&()const{
            return _value;
        }

        virtual property_proxy& operator=(const T& value){
            _value = value;
        }

        virtual property_proxy& operator=(T&& value){
            _value = std::move(value);
        }
};

template <typename BaseType, typename SizeType>
class array_get_proxy
{
protected:
    BaseType*& _array;
    const SizeType& _size;
public:
    array_get_proxy(BaseType*& array, const SizeType& size)
    :_array(array),_size(size){ }
    virtual ~array_get_proxy(){ }

    virtual const BaseType* cbegin() const{
        return _array;
    }

    virtual const BaseType* cend() const{
        return _array + _size;
    }

    virtual const SizeType& size() const{
        return _size;
    }

    virtual const BaseType& operator[](const SizeType& index) const {

        if(index >= _size)
            throw std::invalid_argument("index out of range");

        return _array[index];
    }
};

