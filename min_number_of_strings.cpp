#include <algorithm>
#include <string>
#include <vector>
#include<iostream>
#include <format>
#include <cctype>

class Solution
{
public:
    int minValidStrings(const std::vector<std::string>& words,const std::string target)
    {
        const int n = static_cast<int>(target.size());
        const int INF = 1e9;

        std::vector<int> minStringsCount(n + 1, INF);
        minStringsCount[0] = 0;

        for (int i = 0; i < n; i++)
        {
            if (minStringsCount[i] == INF)
            {
                continue;
            }
            for (const std::string& word : words)
            {
                const int maxLen = std::min(static_cast<int>(word.size()),n - i);

                for (int len = 1; len <= maxLen; len++)
                {
                    if (target[i + len - 1] != word[len - 1])
                    {
                        break;
                    }
                    minStringsCount[i + len] = std::min(minStringsCount[i + len],minStringsCount[i] + 1);
                }
            }
        }

        return minStringsCount[n] == INF ? -1 : minStringsCount[n];
    }
};

void toLowerCase(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(),[](unsigned char ch)
        {
            return static_cast<char>(std::tolower(ch));
        });
}

int main()
{
    Solution solution;
    std::vector<std::string> words;
    std::cout << "Enter number of words: ";
    int numWords;
    std::cin >> numWords;
    std::cout << "Enter the words:\n";
    for (int i = 0; i < numWords; i++)
    {
        std::string word;
        std::cin >> word;
        toLowerCase(word);
        words.push_back(word);
    }
    std::cout << "Enter the target string: ";
    std::string target;
    std::cin >> target;
    toLowerCase(target);
    int result = solution.minValidStrings(words, target);
    std::cout << std::format("Minimum number of valid strings: {}\n", result);
    return 0;
}