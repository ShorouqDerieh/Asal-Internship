#include<string>
#include<iostream>
#include<unordered_map>
#include<vector>
/**
 * Finds all DNA sequences of length 10 that appear more than once in the given DNA string.
 * @param dna The input DNA string.
 * @return A vector containing all repeated DNA sequences of length 10.
 */
std::vector<std::string> dna_sequences(std::string dna)
{
    std::vector<std::string>result;
    std::unordered_map<std::string, int> dna_map;
    if (dna.length() < 10)
    {
        return result; 
    }
    for (int i = 0; i <= dna.length() - 10; i++)
    {
        if (dna_map.find(dna.substr(i, 10)) != dna_map.end())
        {
            dna_map[dna.substr(i, 10)]++;
        }
        else
        {
            dna_map[dna.substr(i, 10)] = 1;
        }
    }
    for (auto &s : dna_map)
    {
        if (s.second > 1)
        {
            result.push_back(s.first);
        }
    }
    return result;
}
int main()
{
    std::string dna;
    std::cin >> dna;
    std::vector<std::string> sequences = dna_sequences(dna);
    for (const auto& seq : sequences) {
        std::cout << seq << std::endl;
    }
}
