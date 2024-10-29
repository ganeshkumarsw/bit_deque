#include <iostream>
#include <thread>
#include "bit_deque.hpp"

int main()
{
    bitset_guarded_deque<52> bit_deque_instance; // Create a bit_deque with 8 bits
    bit_deque_instance.push_back(2);             // Set bit at position 2
    bit_deque_instance.push_front(3);            // Set bit at position 3
    bit_deque_instance.push_front(49);           // Set bit at position 5

    std::thread t1{[&]()
                   {
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                   }};

    std::thread t2{[&]()
                   {
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                       std::cout << "poped bit: " << bit_deque_instance.front() << std::endl;
                       bit_deque_instance.pop_front();
                   }};

    t1.join();
    t2.join();

    return 0;
}
