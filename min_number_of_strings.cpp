
#include <algorithm>
#include <string>
#include <vector>
#include<iostream>
using namespace std;

class Solution
{
public:
    int minValidStrings(std::vector<std::string>& words,std::string target)
    {
        int n = static_cast<int>(target.size());
        const int INF = 1e9;

        std::vector<int> dp(n + 1, INF);
        dp[0] = 0;

        for (int i = 0; i < n; i++)
        {
            if (dp[i] == INF)
                continue;

            for (const std::string& word : words)
            {
                int maxLen = std::min(static_cast<int>(word.size()),n - i);

                for (int len = 1; len <= maxLen; len++)
                {
                    if (target[i + len - 1] != word[len - 1])
                        break;

                    dp[i + len] = std::min(dp[i + len],dp[i] + 1);
                }
            }
        }

        return dp[n] == INF ? -1 : dp[n];
    }
};

int main()
{
    Solution solution;
    std::vector<std::string> words = {"ab", "bc", "cd"};
    std::string target = "abcd";
    int result = solution.minValidStrings(words, target);
    std::cout << "Minimum number of valid strings: " << result << std::endl;
    return 0;
}