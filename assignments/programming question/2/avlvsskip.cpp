#include <iostream>
#include <assert.h>
#include <cmath>
#include <vector>
#include <string>
#include <climits>

using namespace std;

int search_count = 0;

struct bstnode {
  int key;
  int value;
  struct bstnode *parent;
  struct bstnode *left;
  struct bstnode *right;
  int height;
};

struct bst {
  struct bstnode *root;
  int num_item;
};

struct bst *bst_create_empty(void) {
  bst* tree = new bst;
  tree->root = NULL;
  tree->num_item = 0;
  return tree;
}

static struct bstnode *new_leaf(int k, int v) {
  bstnode *leaf = new bstnode;
  leaf->key = k;
  leaf->value = v;
  leaf->height = 0;
  leaf->left = NULL;
  leaf->right = NULL;
  leaf->parent = NULL;
  return leaf;
}

void printtree(bstnode*);

void setHeightFromSubtrees(bstnode* u) {
    if(u->left == NULL && u->right == NULL){
        u->height = 0;
        return;
    }
    if(u->left == NULL) {
        u->height = 1 + u->right->height;
        return;
    }
    if(u->right == NULL) {
        u->height = 1 + u->left->height;
        return;
    }
    u->height = 1 + max(u->left->height, u->right->height);
}

bstnode* rotate_right(bstnode* z) {
    bstnode* y = z->left;
    z->left = y->right;
    y->right = z;
    if(z->left != NULL)
        z->left->parent = z;
    if(z->right != NULL)
        z->right->parent = z;
    if(y->right != NULL)
        y->right->parent = y;
    if(y->left != NULL)
        y->left->parent = y;
    setHeightFromSubtrees(z);
    setHeightFromSubtrees(y);
    return y;
}

bstnode* rotate_left(bstnode* z) {
    bstnode* y = z->right;
    z->right = y->left;
    y->left = z;
    if(z->right != NULL)
        z->right->parent = z;
    if(z->left != NULL)
        z->left->parent = z;
    if(y->left != NULL)
        y->left->parent = y;
    if(y->right != NULL)
        y->right->parent = y;
    setHeightFromSubtrees(z);
    setHeightFromSubtrees(y);
    return y;
}

bstnode* restructure(bstnode* x, bstnode* y, bstnode* z) {
    if(z->left==y && y->left==x){
//            cout << "branch 1" << endl;
        return rotate_right(z);
    }
    if(z->left==y && y->right==x){
//            cout << "branch 2" << endl;
        z->left = rotate_left(y);
        return rotate_right(z);
    }
    if(z->right==y && y->left==x) {
//            cout << "branch 3" << endl;
        z->right = rotate_right(y);
        return rotate_left(z);
    }
    if(z->right==y && y->right==x){
//            cout << "branch 4" << endl;
        return rotate_left(z);
    }
    cerr << "Restrucure case not found! ERROR" << endl;
    return NULL;
}

bool bst_search(int k, const bst *tree) {
  assert(tree);
  struct bstnode *node = tree->root;
  while (node) {
    search_count++;
    if (node->key == k) {
      cout << node->value << " " << search_count << endl;
      return true;
    }
    search_count++;
    if (k < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  cout << "ERROR " << search_count << endl;
  return false;
}

bstnode* bst_insert(int k, int v, struct bst *tree) {
  assert(tree);
  struct bstnode *node = tree->root;
  struct bstnode *parent = NULL;
  while (node) {
    search_count++;
    if (node->key == k) {
      return node;
    }
    parent = node;
    search_count++;
    if (k < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  tree->num_item++;
  if (parent == NULL) {  // tree was empty
    tree->root = new_leaf(k, v);
    return tree->root;
  } else if (k < parent->key) {
    parent->left = new_leaf(k, v);
    parent->left->parent = parent;
    return parent->left;
  } else {
    parent->right = new_leaf(k, v);
    parent->right->parent = parent;
    return parent->right;
  }
}

bstnode* bst_delete(int k, struct bst *tree) {
  assert(tree);
  struct bstnode *target = tree->root;
  struct bstnode *target_parent = NULL;
  struct bstnode *returning_val = NULL;
  // find the target (and its parent)
  while (target && target->key != k) {
    search_count++;
    target_parent = target;
    search_count++;
    if (k < target->key) {
      target = target->left;
    } else {
      target = target->right;
    }
  }
  search_count++;
  if (target == NULL) {
    cerr << "Key not found during deleting!" << endl;
    return target; // key not found
  }
  // find the node to "replace" the target
  struct bstnode *replacement = NULL;
  if(target->left == NULL && target->right == NULL){
    replacement = NULL;
    returning_val = target->parent;
  } else if (target->left == NULL) {
    replacement = target->right;
    replacement->parent = target_parent;
    returning_val = replacement;
//    cout << "returning value changes!" << endl;             //debug use
  } else if (target->right == NULL) {
    replacement = target->left;
    replacement->parent = target_parent;
    returning_val = replacement;
//    cout << "returning value changes! no1" << endl;         //debug use
  } else {
    // neither child is NULL:
    // find the replacement node and its parent
    replacement = target->right;
    struct bstnode *replacement_parent = target;
    while (replacement->left) {
      replacement_parent = replacement;
      replacement = replacement->left;
    }
    returning_val = replacement;
//    cout << "returning value changes! no2" << endl;         //debug use
    // update the child links for the replacement and its parent
    replacement->left = target->left;
    replacement->left->parent = replacement;
    if (replacement_parent != target) {
      replacement_parent->left = replacement->right;
      replacement_parent->left->parent = replacement_parent;
      replacement->right = target->right;
      replacement->right->parent = replacement;
    }
    replacement->parent = target->parent;
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
  tree->num_item--;
  return returning_val;
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

void avl_insert(int k,int v, bst* tree) {
    bstnode *z = bst_insert(k,v,tree);
//    printtree(tree->root);                            //debug use
//        cout << "z: " << z->key << endl;
    bstnode *y=NULL;
    bstnode *x=NULL;
    bstnode *z_parent = NULL;
    while(z) {
        bool flag = false;
        bool isleft = false;
        if(z->left == NULL && z->right == NULL){
//                cout << "branch1" << endl;
            setHeightFromSubtrees(z);
            z = z->parent;
            continue;
        }
        else if(z->left == NULL && z->right->height == 0) {
//                cout << "branch2" << endl;
            setHeightFromSubtrees(z);
            z = z->parent;
            continue;
        }
        else if(z->right == NULL && z->left->height == 0) {
//                cout << "branch3" << endl;
            setHeightFromSubtrees(z);
            z = z->parent;
            continue;
        }
        else if(z->left == NULL && z->right->height > 0) {
//                cout << "branch4" << endl;
            y = z->right;
            if(y->left == NULL) {
                x = y->right;
            } else if(y->right == NULL) {
                x = y->left;
            } else {
                if(y->left->height > y->right->height)
                    x = y->left;
                else
                    x = y->right;
            }
            if(tree->root == z) flag = true;
            z_parent = z->parent;
            if(!flag && z_parent->left == z) isleft = true;
//            printtree(tree->root);                      //debug use
            z = restructure(x,y,z);
//            printtree(tree->root);                      //debug use
            z->parent = z_parent;
            if(flag) {
                tree->root = z;
                setHeightFromSubtrees(z);
                z = z->parent;
                continue;
            }
            if(isleft)
                z_parent->left = z;
            else
                z_parent->right = z;
            break;
        }
        else if(z->right == NULL && z->left->height > 0) {
//                cout << "branch5" << endl;
            y = z->left;
            if(y->left == NULL) {
                x = y->right;
            } else if(y->right == NULL) {
                x = y->left;
            } else {
                if(y->left->height > y->right->height)
                    x = y->left;
                else
                    x = y->right;
            }
            if(tree->root == z) flag = true;
            z_parent = z->parent;
            if(!flag && z_parent->left == z) isleft = true;
            z = restructure(x,y,z);
//                printtree(tree->root);
            z->parent = z_parent;
            if(flag) {
                tree->root = z;
                setHeightFromSubtrees(z);
                z = z->parent;
                continue;
            }
            if(isleft)
                z_parent->left = z;
            else
                z_parent->right = z;
            break;
        }
        else if(abs(z->left->height - z->right->height) > 1){
//                cout << "branch6" << endl;
            if(z->left->height > z->right->height) {
                y = z->left;
            } else {
                y = z->right;
            }
            if(y->left->height > y->right->height) {
                x = y->left;
            } else {
                x = y->right;
            }
            if(tree->root == z) flag = true;
            z_parent = z->parent;
            if(!flag && z_parent->left == z) isleft = true;
            z = restructure(x,y,z);
            z->parent = z_parent;
            if(flag) {
                tree->root = z;
                setHeightFromSubtrees(z);
                z = z->parent;
                continue;
            }
            if(isleft)
                z_parent->left = z;
            else
                z_parent->right = z;
            break;
        }
        setHeightFromSubtrees(z);
        z = z->parent;
    }
}

void avl_delete(int k, bst* tree) {
    bstnode *z = bst_delete(k, tree);
//    cout << "z: " << z << endl;                         //debug use
    bstnode *y = NULL;
    bstnode *x = NULL;
    bstnode *z_parent = NULL;
    while(z) {
//        cout << "debug1" << endl;                       //debug use
        bool flag = false;
        bool isleft = false;
        if(z->left == NULL && z->right == NULL){
            setHeightFromSubtrees(z);
            z = z->parent;
            continue;
        }
        else if(z->left == NULL && z->right->height == 0) {
            setHeightFromSubtrees(z);
            z = z->parent;
            continue;
        }
        else if(z->right == NULL && z->left->height == 0) {
            setHeightFromSubtrees(z);
            z = z->parent;
            continue;
        }
        else if(z->left == NULL && z->right->height > 0) {
            y = z->right;
            if(y->left == NULL) {
                x = y->right;
            } else if(y->right == NULL) {
                x = y->left;
            } else {
                if(y->left->height > y->right->height)
                    x = y->left;
                else
                    x = y->right;
            }
            if(tree->root == z) flag = true;
            z_parent = z->parent;
            if(!flag && z_parent->left == z) isleft = true;
            z = restructure(x,y,z);
            z->parent = z_parent;
            if(flag) {
                tree->root = z;
                setHeightFromSubtrees(z);
                z = z->parent;
                continue;
            }
            if(isleft)
                z_parent->left = z;
            else
                z_parent->right = z;
        }
        else if(z->right == NULL && z->left->height > 0) {
//            cout << "debug" << endl;                        //debug use
            y = z->left;
            if(y->left == NULL) {
                x = y->right;
            } else if(y->right == NULL) {
                x = y->left;
            } else {
                if(y->left->height > y->right->height)
                    x = y->left;
                else
                    x = y->right;
            }
            if(tree->root == z) flag = true;
//            cout << "debug2" << endl;                   //debug use
            z_parent = z->parent;
            if(!flag && z_parent->left == z) isleft = true;
            z = restructure(x,y,z);
            z->parent = z_parent;
            if(flag) {
                tree->root = z;
                setHeightFromSubtrees(z);
                z = z->parent;
                continue;
            }
            if(isleft)
                z_parent->left = z;
            else
                z_parent->right = z;
//            cout << "debug3" << endl;               //debug use
        }
        else if(abs(z->left->height - z->right->height) > 1){
            if(z->left->height > z->right->height) {
                y = z->left;
            } else {
                y = z->right;
            }
            if(y->left->height > y->right->height) {
                x = y->left;
            } else {
                x = y->right;
            }
            if(tree->root == z) flag = true;
            z_parent = z->parent;
            if(!flag && z_parent->left == z) isleft = true;
            z = restructure(x,y,z);
            z->parent = z_parent;
            if(flag) {
                tree->root = z;
                setHeightFromSubtrees(z);
                z = z->parent;
                continue;
            }
            if(isleft)
                z_parent->left = z;
            else
                z_parent->right = z;
        }
        setHeightFromSubtrees(z);
        z = z->parent;
    }
}



struct skplstnode {
  int key;
  int value;
  struct skplstnode *after;
  struct skplstnode *below;
};

struct skplst {
  struct skplstnode *root;
  int num_item;
  int num_node;
};

struct skplst *sl_create_empty(void) {
  skplst* sl = new skplst;
  skplstnode* neg_infty = new skplstnode;
  skplstnode* pos_infty = new skplstnode;
  neg_infty->key = INT_MIN;
  neg_infty->value = 0;
  neg_infty->after = pos_infty;
  neg_infty->below = NULL;
  pos_infty->key = INT_MAX;
  pos_infty->value = 0;
  pos_infty->after = NULL;
  pos_infty->below = NULL;
  sl->root = neg_infty;
  sl->num_item = 0;
  sl->num_node = 0;
  return sl;
}



vector<skplstnode*>* getPredecessor(int k, skplstnode* root) {
    skplstnode *p = root;
    vector<skplstnode*> *pred = new vector<skplstnode*>;
    pred->push_back(p);
    while(p->below) {
        p = p->below;
        while(p->after->key < k) {
            search_count++;
            p = p->after;
        }
        search_count++;
        pred->push_back(p);
    }
    return pred;
}

skplstnode* sl_search(int k, skplst* sl) {
    vector<skplstnode*> *pred = getPredecessor(k, sl->root);
    skplstnode* top = pred->back();
    search_count++;
    if(top->after->key == k) {
        delete pred;
        return top->after;
    }
    delete pred;
    return top;
}

void sl_insert(int k, int v, int coin, skplst* sl) {
    skplstnode* p = sl->root->below;
    sl->num_item++;
    sl->num_node = sl->num_node + coin + 1;
    int h;
    for(h = 0; p != NULL; p = p->below)  {h++;}
    while(coin >= h) {
        skplstnode* neg_infty = new skplstnode;
        skplstnode* pos_infty = new skplstnode;
        neg_infty->key = INT_MIN;
        neg_infty->value = 0;
        neg_infty->after = pos_infty;
        neg_infty->below = sl->root;
        pos_infty->key = INT_MAX;
        pos_infty->value = 0;
        pos_infty->after = NULL;
        pos_infty->below = sl->root->after;
        sl->root = neg_infty;
        h++;
    }
    vector<skplstnode*> *pred = getPredecessor(k, sl->root);
    p = pred->back();
    pred->pop_back();
    skplstnode* zBellow = new skplstnode;
    zBellow->key = k;
    zBellow->value = v;
    zBellow->after = p->after;
    zBellow->below = NULL;
    p->after = zBellow;
    while(coin > 0) {
        p = pred->back();
        pred->pop_back();
        skplstnode* z = new skplstnode;
        z->key = k;
        z->value = v;
        z->after = p->after;
        p->after = z;
        z->below = zBellow;
        zBellow = z;
        coin--;
    }
    delete pred;
}

void sl_delete(int k, skplst* sl) {
    vector<skplstnode*> *pred = getPredecessor(k, sl->root);
    skplstnode* p;
    bool flag = false;
    while(!pred->empty()) {
        p = pred->back();
        pred->pop_back();
        if(p->after->key == k) {
            p->after = p->after->after;
            sl->num_node--;
            if(flag == false) {
                flag = true;
                sl->num_item--;
            }
        }
        else break;
    }
    p = sl->root;
    while(p->below->after->key == INT_MAX) {
        p->below = p->below->below;
        p->after->below = p->after->below->below;
    }
    delete pred;
}

void sl_destroy(skplst *sl) {
    assert(sl);
    skplstnode *line = sl->root;
    skplstnode *col = sl->root;
    skplstnode *nxt_col;
    while(line) {
        col = line;
        line = line->below;
        while(col) {
            nxt_col = col->after;
            delete col;
            col = nxt_col;
        }
    }
    delete sl;
}

void printtree(bstnode* n) {            // debug use
    if(n==NULL) return;
    cout << n->key << " " << n->height << " parent: ";
    if(n->parent == NULL) cout << "NULL" << endl;
    else cout << n->parent->key << endl;
    printtree(n->left);
    printtree(n->right);
}

int main() {
    skplst* sl = NULL;
    bst* tree = NULL;
    while(1){
        string command;
        cin >> command;
        if(command == "x") {
            if(tree) bst_destroy(tree);
            if(sl) sl_destroy(sl);
            return 0;
        } else if (command == "r") {
            if(tree) bst_destroy(tree);
            if(sl) sl_destroy(sl);
            tree = bst_create_empty();
            sl = sl_create_empty();
        } else if (command == "iavl") {
            search_count = 0;
            int k, v;
            cin >> k >> v;
            avl_insert(k,v,tree);
            cout << search_count << endl;
        } else if (command == "isl") {
            search_count = 0;
            int k, v, coin;
            cin >> k >> v >> coin;
            sl_insert(k,v,coin,sl);
            cout << search_count << endl;
        } else if (command == "davl") {
            search_count = 0;
            int k;
            cin >> k;
            avl_delete(k,tree);
            cout << search_count << endl;
        } else if (command == "dsl") {
            search_count = 0;
            int k;
            cin >> k;
            sl_delete(k,sl);
            cout << search_count << endl;
        } else if (command == "savl") {
            int k;
            cin >> k;
            search_count = 0;
            bst_search(k,tree);
        } else if (command == "ssl") {
            int k;
            cin >> k;
            search_count = 0;
            skplstnode* return_val = sl_search(k,sl);
            if(return_val->key == k) {
                cout << return_val->value << " " << search_count << endl;
            } else {
                cout << "ERROR " << search_count << endl;
            }
        } else if (command == "statsavl") {
            cout << tree->num_item << " " << tree->root->height << endl;
        } else if (command == "statssl") {
            cout << sl->num_item << " " << sl->num_node << endl;
        } else {
            cerr << "Undefined command!" << endl;
            continue;
        }
    }
    return 0;
}
