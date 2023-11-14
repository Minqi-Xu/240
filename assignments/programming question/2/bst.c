// The following code is based on the BST code from CS 136
// You may use it in CS 240 but do not share it with other students

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct bstnode {
  int key;
  int value;
  struct bstnode *left;
  struct bstnode *right;
};

struct bst {
  struct bstnode *root;
};


struct bst *bst_create_empty(void) {
  struct bst *tree = malloc(sizeof(struct bst));
  tree->root = NULL;
  return tree;
}


static struct bstnode *new_leaf(int k, int v) {
  struct bstnode *leaf = malloc(sizeof(struct bstnode));
  leaf->key = k;
  leaf->value = v;
  leaf->left = NULL;
  leaf->right = NULL;
  return leaf;
}

void printtree();

bool bst_search(int k, const struct bst *tree) {
  assert(tree);
  struct bstnode *node = tree->root;
  while (node) {
    if (node->key == k) {
	  printf("%d <# comparisons>\n", node->value);
      return true;
    }
    if (k < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  printf("ERROR\n");
  return false;
}


void bst_insert(int k, int v, struct bst *tree) {
  assert(tree);
  struct bstnode *node = tree->root;
  struct bstnode *parent = NULL;
  while (node) {
    if (node->key == k) {
      return;
    }
    parent = node;
    if (k < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  if (parent == NULL) {  // tree was empty
    tree->root = new_leaf(k, v);
  } else if (k < parent->key) {
    parent->left = new_leaf(k, v);
  } else {
    parent->right = new_leaf(k, v);
  }
}


void bst_delete(int k, struct bst *tree) {
  assert(tree);
  struct bstnode *target = tree->root;
  struct bstnode *target_parent = NULL;
  // find the target (and its parent)
  while (target && target->key != k) {
    target_parent = target;
    if (k < target->key) {
      target = target->left;
    } else {
      target = target->right;
    }
  }
  if (target == NULL) {
    return; // key not found
  }
  // find the node to "replace" the target
  struct bstnode *replacement = NULL;
  if (target->left == NULL) {
    replacement = target->right;
  } else if (target->right == NULL) {
    replacement = target->left;
  } else {
    // neither child is NULL:
    // find the replacement node and its parent
    replacement = target->right;
    struct bstnode *replacement_parent = target;
    while (replacement->left) {
      replacement_parent = replacement;
      replacement = replacement->left;
    }
    // update the child links for the replacement and its parent
    replacement->left = target->left;
    if (replacement_parent != target) {
      replacement_parent->left = replacement->right;
      replacement->right = target->right;
    }
  }
  // free the target, and update the target's parent
  free(target);
  if (target_parent == NULL) {
    tree->root = replacement;
// Note: the following is an implementation detail that could be removed
// by using a double pointer or implementing delete recursively.
  } else if (k > target_parent->key) {  // don't count this comparison
    target_parent->right = replacement;
  } else {
    target_parent->left = replacement;
  }
}


static void free_bstnode(struct bstnode *node) {
  if (node) {
    free_bstnode(node->left);
    free_bstnode(node->right);
    free(node);
  }
}


void bst_destroy(struct bst *tree) {
  assert(tree);
  free_bstnode(tree->root);
  free(tree);
}

int main(void) {
  struct bst *tree = bst_create_empty();
  bst_insert(7, 14, tree);
  bst_insert(9, 18, tree);
  bst_insert(11, 22, tree);
  bst_search(13, tree);
  bst_search(9, tree);
  bst_delete(7, tree);
  bst_destroy(tree);
}
