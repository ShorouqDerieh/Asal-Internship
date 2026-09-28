#include<iostream>
#include<vector>
using namespace std;
/**
 * @brief Moves all zeros to the end of the array while maintaining the relative order of non-zero elements.
 *
 * @param numbers A reference to a vector of integers that will be modified in place.

 */
void moveZeroes(vector<int>& numbers)
{
    int last = 0;
    for (int number : numbers)
    {
        if (number)
        {
            numbers[last++] = number;
        }
    }

    for (int i = last; i < numbers.size(); i++)
    {
        numbers[i] = 0;
    }
}
