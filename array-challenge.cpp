#include<iostream>
#include<vector>
using namespace std;
void moveZeroes(vector<int>& numbers)
{
    int last = 0;
    for (int i = 0; i < numbers.size(); i++)
    {
        if (numbers[i] != 0)
        {
            numbers[last] = numbers[i];
            last++;
        }
    }

    for (int i = last; i < numbers.size(); i++)
    {
        numbers[i] = 0;
    }
}
