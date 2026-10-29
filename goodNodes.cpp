#include <iostream>
#include <memory>
#include <climits>
#include <vector>
#include<queue>
#include <optional>
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
    std::unique_ptr<Node> build_tree(const std::vector<std::optional<int>>&elements)
    {
        if (elements.empty() || !elements[0])
            return nullptr;

        auto root = std::make_unique<Node>(*elements[0]);

        std::queue<Node*> q;
        q.push(root.get());

         int i = 1;

        while (!q.empty() && i < elements.size())
        {
            Node* current = q.front();
            q.pop();

            if (i < elements.size() && elements[i])
            {
                current->left = std::make_unique<Node>(*elements[i]);
                q.push(current->left.get());
            }
            ++i;

            if (i < elements.size() && elements[i])
            {
                current->right = std::make_unique<Node>(*elements[i]);
                q.push(current->right.get());
            }
            ++i;
        }

        return root;
    }
int main()
{
    auto root=build_tree({2,1,1,3, std::nullopt,1,5});
    Solution sol;
    std::cout << "Good Nodes: "
        << sol.goodNodes(root) << '\n';
    return 0;
}
