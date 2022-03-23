class TreeNode {
    private:
        TreeNode* left;
        TreeNode* right;

        TreeNode(): left (nullptr), right (nullptr) { }

        static int depth (TreeNode *root) {
            if (root == nullptr) return 0;
            return std::max (depth (root->left), depth (root->right)) + 1;
        }

    public:
        static bool isPerfect (TreeNode* root) {
            if (root == nullptr) return true;
            if (depth (root->left) != depth (root->right)) return false;

            return isPerfect (root->left) && isPerfect (root->right);
        }

        static TreeNode* leaf () { return new TreeNode(); }

        static TreeNode* join(TreeNode* left, TreeNode* right) {
            return (new TreeNode())->withChildren(left, right);
        }

        TreeNode* withLeft (TreeNode* left) {
            this->left = left;
            return this;
        }
        TreeNode* withRight (TreeNode* right) {
            this->right = right;
            return this;
        }

        TreeNode* withChildren (TreeNode* left, TreeNode* right) {
            this->left = left;
            this->right = right;
            return this;
        }

        TreeNode* withLeftLeaf() {
            return withLeft(leaf());
        }
        TreeNode* withRightLeaf() {
            return withRight(leaf());
        }

        TreeNode* withLeaves() {
            return withChildren (leaf(), leaf());
        }
};
