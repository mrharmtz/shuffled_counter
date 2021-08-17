#include <iostream>
#include <random>
#include "shuffled_counter.hpp"
#include "range_iteration.hpp"

using std::cout;
using std::endl;

int main(int argc, const char* argv[]){

    std::initializer_list<unsigned> order = {0, 1, 2, 3};
    unsigned value = 0;

    for(const auto val : range<int>(10))
        cout << val << endl;

    try{
        for(unsigned i = 0 ; i < (1 << order.size()) ; ++i)
        cout << "i(" << i << ") = " << (value = unsigned_shuffled_counter::next_value(value, order)) << endl;
        cout << "constructing my_counter" << endl;
        unsigned_shuffled_counter my_counter(order.begin(), order.end());
        cout << "constructing my_other_counter" << endl;
        unsigned_shuffled_counter my_other_counter(order);
        cout << "constructing my_counter_copy" << endl;
        unsigned_shuffled_counter my_counter_copy(my_counter);
        cout << "constructing random_counter" << endl;
        unsigned_shuffled_counter random_counter(10, std::default_random_engine());

        cout << "my_counter value = " << (unsigned)my_counter << endl;
        my_counter = (unsigned)1;
        cout << "my_counter value = " << (unsigned)my_counter << endl;

        my_other_counter = (unsigned)my_counter;

    }catch(...){
        cout << "oops an exception was thrown" << endl;
    }


    return 0;
}