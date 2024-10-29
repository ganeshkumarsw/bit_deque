/******************************************************************************
The MIT License(MIT)

Bits Deque.

Copyright(c) 2024 Ganeshkumar Swaminathan
https://github.com/ganeshkumarsw/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/
#pragma once

#include <stdint.h>
#include <etl/bitset.h>
#include <etl/mutex.h>

template <const size_t NUM_BITS_>
class bitset_deque
{
public:
    // Constructor
    bitset_deque() : current_index_(0) {}

    bool push_front(std::size_t bit_pos)
    {
        if (bit_pos >= NUM_BITS_)
        {
            return false; // Return false if the bit position is out of range
        }

        if (!bitset_[bit_pos])
        {
            bitset_[bit_pos] = 1; // Set the bit
        }

        // Adjust the current index to prioritize bit popping
        current_index_ = bit_pos;

        return true; // Return true if the bit was set successfully
    }

    // Push a bit position and set the corresponding bit
    bool push_back(std::size_t bit_pos)
    {
        if (bit_pos >= NUM_BITS_)
        {
            return false; // Return false if the bit position is out of range
        }

        if (!bitset_[bit_pos])
        {
            bitset_[bit_pos] = 1; // Set the bit
        }

        return true; // Return true if the bit was set successfully
    }

    // Is bit set at the corresponding position
    bool is_set(std::size_t bit_pos)
    {
        bool ret{false};

        if (bit_pos >= NUM_BITS_)
        {
            return false; // Return false if the bit position is out of range
        }

        ret = bitset_[bit_pos];

        return ret; // Return true if the bit was set successfully
    }

    // Pop the next set bit position and return it, return -1 if no bit is set
    int front()
    {
        int peeked_bit{-1};
        size_t start_index{current_index_};

        // Continue searching for a set bit
        while (true)
        {
            // If the current_index exceeds the limit, reset it to 0
            if (current_index_ >= NUM_BITS_)
            {
                current_index_ = 0;
            }

            // Check if there's a bit set at the current_index
            if (bitset_[current_index_])
            {
                peeked_bit = current_index_;
                break;
            }

            // Move to the next bit
            ++current_index_;

            // If no bits are set and we have looped back around, return -1
            if (current_index_ == start_index)
            {
                peeked_bit = -1;
                break;
            }
        }

        // If no bits are set, return -1
        return peeked_bit;
    }

    // Pop the next set bit position and return it, return -1 if no bit is set
    bool pop_front()
    {
        bool status{false};

        size_t start_index{current_index_};

        // Continue searching for a set bit
        while (true)
        {
            // If the current_index exceeds the limit, reset it to 0
            if (current_index_ >= NUM_BITS_)
            {
                current_index_ = 0;
            }

            // Check if there's a bit set at the current_index
            if (bitset_[current_index_])
            {
                bitset_[current_index_] = 0; // Clear the bit
                status = true;
                ++current_index_; // Move to the next bit index for the next call
                break;
            }

            // Move to the next bit
            ++current_index_;

            // If no bits are set and we have looped back around, return -1
            if (current_index_ == start_index)
            {
                status = false;
                break;
            }
        }

        // If no bits are set, return -1
        return status;
    }

    // Check if there are any bits set
    bool empty() const
    {
        return bitset_.none();
    }

    // Check if there are any bits set
    constexpr bool clear()
    {
        return bitset_.reset();
    }

    // Get the number of bits currently set in the BitDeque
    constexpr size_t size() const
    {
        return bitset_.count();
    }

private:
    etl::bitset<NUM_BITS_> bitset_; // Store the bitset with num_bits length
    size_t current_index_;          // Tracks the next valid bit position
};

template <const size_t NUM_BITS_>
class bitset_guarded_deque
{
public:
    // Constructor
    bitset_guarded_deque() : current_index_(0) {}

    bool push_front(std::size_t bit_pos)
    {
        if (bit_pos >= NUM_BITS_)
        {
            return false; // Return false if the bit position is out of range
        }

        {
            etl::lock_guard<etl::mutex> lock_guard(lock_);
            if (!bitset_[bit_pos])
            {
                bitset_[bit_pos] = 1; // Set the bit
            }

            // Adjust the current index to prioritize bit popping
            current_index_ = bit_pos;
        }

        return true; // Return true if the bit was set successfully
    }

    // Push a bit position and set the corresponding bit
    bool push_back(std::size_t bit_pos)
    {
        if (bit_pos >= NUM_BITS_)
        {
            return false; // Return false if the bit position is out of range
        }

        {
            etl::lock_guard<etl::mutex> lock_guard(lock_);
            if (!bitset_[bit_pos])
            {
                bitset_[bit_pos] = 1; // Set the bit
            }
        }

        return true; // Return true if the bit was set successfully
    }

    // Is bit set at the corresponding position
    bool is_set(std::size_t bit_pos)
    {
        bool ret{false};

        if (bit_pos >= NUM_BITS_)
        {
            return false; // Return false if the bit position is out of range
        }

        {
            etl::lock_guard<etl::mutex> lock_guard(lock_);
            ret = bitset_[bit_pos];
        }

        return ret; // Return true if the bit was set successfully
    }

    // Pop the next set bit position and return it, return -1 if no bit is set
    int front()
    {
        int peeked_bit{-1};
        size_t start_index{current_index_};

        etl::lock_guard<etl::mutex> lock_guard(lock_);
        // Continue searching for a set bit
        while (true)
        {
            // If the current_index exceeds the limit, reset it to 0
            if (current_index_ >= NUM_BITS_)
            {
                current_index_ = 0;
            }

            // Check if there's a bit set at the current_index
            if (bitset_[current_index_])
            {
                peeked_bit = current_index_;
                break;
            }

            // Move to the next bit
            ++current_index_;

            // If no bits are set and we have looped back around, return -1
            if (current_index_ == start_index)
            {
                peeked_bit = -1;
                break;
            }
        }

        // If no bits are set, return -1
        return peeked_bit;
    }

    // Pop the next set bit position and return it, return -1 if no bit is set
    bool pop_front()
    {
        bool status{false};

        size_t start_index{current_index_};

        etl::lock_guard<etl::mutex> lock_guard(lock_);
        // Continue searching for a set bit
        while (true)
        {
            // If the current_index exceeds the limit, reset it to 0
            if (current_index_ >= NUM_BITS_)
            {
                current_index_ = 0;
            }

            // Check if there's a bit set at the current_index
            if (bitset_[current_index_])
            {
                bitset_[current_index_] = 0; // Clear the bit
                status = true;
                ++current_index_; // Move to the next bit index for the next call
                break;
            }

            // Move to the next bit
            ++current_index_;

            // If no bits are set and we have looped back around, return -1
            if (current_index_ == start_index)
            {
                status = false;
                break;
            }
        }

        // If no bits are set, return -1
        return status;
    }

    // Check if there are any bits set
    bool empty() const
    {
        return bitset_.none();
    }

    // Check if there are any bits set
    constexpr bool clear()
    {
        return bitset_.reset();
    }

    // Get the number of bits currently set in the BitDeque
    constexpr size_t size() const
    {
        return bitset_.count();
    }

private:
    etl::bitset<NUM_BITS_> bitset_; // Store the bitset with num_bits length
    size_t current_index_;          // Tracks the next valid bit position
    etl::mutex lock_;
};
