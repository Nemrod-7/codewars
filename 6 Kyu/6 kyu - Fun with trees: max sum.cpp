class Solution {
  public:
    static int maxSum (TreeNode* root) {
        if (root == nullptr) return 0;
        return root->value + std::max (maxSum (root->left), maxSum (root->right));
    }
};
