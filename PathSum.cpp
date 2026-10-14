#include<memory>
#include<vector>
#include <iostream>
struct Node
{
    int data;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node>right;
    Node(int value):data(value),left(nullptr),right(nullptr){}
};
class Solution 
{
    private:
    int sum=0;
    std::vector<std::vector<int>> result;
    std::vector<int>items;
     void dfs(std::unique_ptr<Node>& node, int targetSum)
      {
        if(node==nullptr)
            {
            return;
              }
        items.push_back(node->data);
        sum+=node->data;
    if(node->left==nullptr && node->right==nullptr && sum==targetSum)
    {
        result.push_back(items);
    }
    dfs(node->left,targetSum);
    dfs(node->right,targetSum);
    items.pop_back();
    sum-=node->data;
     }
public:
    std::vector<std::vector<int>> pathSum(std::unique_ptr<Node>& root, int targetSum) 
    {
        dfs(root,targetSum);
        return result;
}
};
