#include <iostream>
#include "shuffled_counter.hpp"

using std::cout;
using std::endl;

int main(int argc, const char* argv[]){

    std::initializer_list<unsigned> order = {0, 1, 2, 3};
    unsigned value = 0;

    for(unsigned i = 0 ; i < (1 << order.size()) ; ++i)
        cout << "i(" << i << ") = " << (value = unsigned_shuffled_counter::next_value(value, order)) << endl;

    try{
        cout << "constructing my_counter" << endl;
        unsigned_shuffled_counter my_counter(order.begin(), order.end());
        cout << "constructing my_other_counter" << endl;
        unsigned_shuffled_counter my_other_counter(order);
        cout << "constructing my_counter_copy" << endl;
        unsigned_shuffled_counter my_counter_copy(my_counter);
    }catch(...){
        cout << "oops an exception was thrown" << endl;
    }


    return 0;
}