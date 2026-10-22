#include<memory>
#include<vector>
#include <iostream>
struct Node
{
    int data;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node>right;
    explicit Node(int value):data(value),left(nullptr),right(nullptr){}
};
class Solution 
{
    private:
    int count = 0;
    int result = 0;
    void inorder(const Node* node, int k)
    {
        if(node==nullptr)
        {
            return;
        }

        inorder(node->left.get(),k);
        count++;
        if (count == k)
        {
            result = node->data;
            return;
        }

        inorder(node->right.get(),k);
    }
    public:
    int kthSmallest(Node* root, int k)
    {
        count=0;
        result=0;
        inorder(root,k);
        return result;
    }
};
int main() {
    auto root = std::make_unique<Node>(5);
    root->left = std::make_unique<Node>(3);
    root->right = std::make_unique<Node>(6);
    root->left->right = std::make_unique<Node>(4);
    root->left->left = std::make_unique<Node>(2);
    root->left->left->left = std::make_unique<Node>(1);

    Solution sol;
    std::cout << sol.kthSmallest(root.get(), 3) << '\n';

    return 0;
}