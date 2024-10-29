#include <iostream>
#include "bit_deque.hpp"

int main()
{
    bitset_guarded_deque<52> bit_deque_instance; // Create a bit_deque with 8 bits
    bit_deque_instance.push_back(2); // Set bit at position 2
    bit_deque_instance.push_front(3); // Set bit at position 3
    bit_deque_instance.push_front(49); // Set bit at position 5


   std::cout << "poped bit: " << std:cout << bit_deque_instance.pop() << std::cout::endl;
   bit_deque_instance.pop_front();

    return 0;
}
