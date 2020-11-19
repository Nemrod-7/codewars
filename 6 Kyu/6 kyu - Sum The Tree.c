struct node {
    int value;
    struct node* left;
    struct node* right;
};


int sumTheTreeValues(struct node* root) {

    if (root == 0)
      return 0;

  return root->value + sumTheTreeValues(root->left) + sumTheTreeValues(root->right);
}
