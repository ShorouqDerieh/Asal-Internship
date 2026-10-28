#include <iostream>
#include<memory>
#include <climits>
struct Node
{
    int data;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node>right;
    explicit Node(int value) :data(value), left(nullptr), right(nullptr) {}
};
class Solution {
private:
    int count = 0;
    int biggest = 0;
    void dfs(std::unique_ptr<Node>& root)
    {
        if (root == nullptr)
        {
            return;
        }

        int pre = biggest;

        if (biggest <= root->data)
        {
            count++;
            biggest = root->data;
        }

        dfs(root->left);
        dfs(root->right);

        biggest = pre;
    }
public:
    int goodNodes(std::unique_ptr<Node>& root) {
        count = 0;
        biggest = INT_MIN;

        dfs(root);

        return count;
    }
};
int main()
{
    auto root = std::make_unique<Node>(3);

    root->left = std::make_unique<Node>(1);
    root->right = std::make_unique<Node>(4);

    root->left->left = std::make_unique<Node>(3);

    root->right->left = std::make_unique<Node>(1);
    root->right->right = std::make_unique<Node>(5);

    Solution sol;

    std::cout << "Good Nodes: "
        << sol.goodNodes(root) << '\n';

    return 0;
}
