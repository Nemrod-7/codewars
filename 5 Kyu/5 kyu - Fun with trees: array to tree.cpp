class Solution {
    public:
        static TreeNode* arrayToTree(const std::vector<int> &arr, size_t index = 0) {

            if (index >= arr.size()) return nullptr;

            TreeNode *node = new TreeNode (arr[index]);

            node->m_left  = arrayToTree (arr, index * 2 + 1);
            node->m_right = arrayToTree (arr, index * 2 + 2);

            return node;
        }
};
