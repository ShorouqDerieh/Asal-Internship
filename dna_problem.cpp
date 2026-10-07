#include<string>
#include<iostream>
#include<unordered_map>
#include<vector>
/**
 * Finds all DNA sequences of length 10 that appear more than once in the given DNA string.
 * @param dna The input DNA string.
 * @return A vector containing all repeated DNA sequences of length 10.
 */
std::vector<std::string> dna_sequences(const std::string dna)
{
    std::vector<std::string>result;
    std::unordered_map<std::string, int> dna_map;
    if (dna.length() < 10)
    {
        return result; 
    }
    for (int i = 0; i <= dna.length() - 10; i++)
    {
		const std::string sequence = dna.substr(i, 10);
		++dna_map[sequence];
    }
    for (const auto &s : dna_map)
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
    std::string allowed = "ACGT";
    std::string dna;
    std::cout << "Enter a DNA sequence: ";
    std::cin >> dna;
	while (dna.find_first_not_of(allowed) != std::string::npos)
	{
        std::cout << "Invalid DNA sequence. Only A, C, G, and T are allowed." << std::endl;
        std::cout << "Please enter a valid DNA sequence: ";
		std::cin >> dna;
	}
    std::vector<std::string> sequences = dna_sequences(dna);
    for (const auto& seq : sequences) {
        std::cout << seq << std::endl;
    }
}
