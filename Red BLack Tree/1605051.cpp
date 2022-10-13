#include <iostream>
#include <algorithm>
using namespace std;

///Insert, Insert_fixup, Delete, Delete_fixup, rotate_left, rotate_right
///functions are taken from Teacher's slide <- (Introduction to Algorithms - CLRS)

#define BLACK 1
#define RED 0
#define INF 1000000000

struct node {
    int key, height, size, color;
    node * left, *right, *parent;
    bool isNULL;

    node(int key, node * left, node * right, node * parent) {
        this->key = key;
        this->left = left;
        this->right = right;
        this->parent = parent;
        size = 1, height = 0, color = RED;
        isNULL = false;

    }

    node() {/// null Black node
        this->left = this->right = this->parent = NULL;
        size = 0, height = -1, color = BLACK;
        this->key = -INF;
        isNULL = true;
    }
};

class RB_Tree {
    node * root;
    node * nullNode;
    void Insert_fixup(node *x);
    void Delete_fixup(node *x);
    void rotate_left(node * x);
    void rotate_right(node * x);
    void print(node *x);
    node * successor(node * x);
    void update_node(node * x);
    void update_ancestor(node * x);
    void testInorder(node *x);
    void delete_sub_tree(node * x);
public:
    RB_Tree() {
        root = new node();
        nullNode = new node();
    }
    void Insert(int val);
    void Delete(node * x);
    node * Search(int val);
    void print(){ print(root); }
    void testInorder() { testInorder(root); }
    ~RB_Tree() {
        delete_sub_tree(root);
    }
};

void RB_Tree::delete_sub_tree(node *x) {
    if (x == nullNode) return;
    delete_sub_tree(x->left);
    delete_sub_tree(x->right);
    delete x;
}

void RB_Tree::testInorder(node *x) {
    if (x->isNULL)
        return;
    if (!x->left->isNULL)
        testInorder(x->left);

    if (!x->isNULL) {
        cout << "(K: " << x->key << "; H: " << x->height << "; S: " << x->size << "; C: " << (x->color == RED? 'R' : 'B') << ")" << endl;
    }

    if (!x->right->isNULL)
        testInorder(x->right);
}

void RB_Tree::Insert_fixup(node *x) {
    while (x->parent->color == RED) {
        if (x->parent == x->parent->parent->left) {
            node * y = x->parent->parent->right;
            if (y->color == RED) {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->right) {
                    x = x->parent;
                    rotate_left(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotate_right(x->parent->parent);
            }
        }
        else {
            node * y = x->parent->parent->left;
            if (y->color == RED) {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rotate_right(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotate_left(x->parent->parent);
            }
        }
    }

    update_ancestor(x);

    root->color = BLACK;
}

void RB_Tree::Delete_fixup(node *x) {
    while (x->color == BLACK and x != root) {
        if (x == x->parent->left) {
            node * w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK and w->right->color == BLACK) {
                w->color = RED;
                update_node(x);
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(x->parent);
                update_ancestor(x);
                x = root;
            }
        }
        else {
            node * w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK and w->left->color == BLACK) {
                w->color = RED;
                update_node(x);
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(x->parent);
                update_ancestor(x);
                x = root;
            }
        }
    }

    update_ancestor(x);

    x->color = BLACK;
}

void RB_Tree::rotate_left(node *x) {
    node * y = x->right;
    x->right = y->left;
    y->left->parent = x;
    y->parent = x->parent;

    if (x->parent->isNULL) root = y;
    else if (x->parent->left == x) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;

    update_node(x);
    update_node(y);
}

void RB_Tree::rotate_right(node *x) {
    node * y = x->left;
    x->left = y->right;
    y->right->parent = x;
    y->parent = x->parent;

    if (x->parent->isNULL) root = y;
    else if (x->parent->right == x) x->parent->right = y;
    else x->parent->left = y;

    y->right = x;
    x->parent = y;

    update_node(x);
    update_node(y);
}

void RB_Tree::update_node(node *x) {
    if (x->isNULL)
        return;
    x->height = max(x->left->height, x->right->height) + 1;
    x->size = x->left->size + x->right->size + 1;
}

void RB_Tree::update_ancestor(node *x) {
    while (x != root) {
        update_node(x);
        x = x->parent;
    }
    update_node(x);
}

void RB_Tree::Insert(int val) {
    node * y = nullNode;
    node * x = root;

    while (!x->isNULL) {
        y = x;

        if (x->key > val) x = x->left;
        else if (x->key < val) x = x->right;
        else return;///duplicate values are not allowed
    }

    node *z = new node(val, nullNode, nullNode, y);

    if (y->isNULL) root = z;
    else if (y->key > val) y->left = z;
    else y->right = z;

    update_node(y);

    Insert_fixup(z);
}

node* RB_Tree::successor(node *x) {///considering it has both non-null left and right child
    if (!x->right->isNULL) {///always true
        x = x->right;
        while (!x->left->isNULL) x = x->left;
        return x;
    }
}

void RB_Tree::Delete(node *x) {
    node *y, *z;
    if (x->left->isNULL or x->right->isNULL) {
        y = x;
    }
    else  y = successor(x);

    if (!y->left->isNULL) {
        z = y->left;
    }
    else z = y->right;

    z->parent = y->parent;
    if (y->parent->isNULL) {
        root = z;
    }
    else if (y->parent->left == y) {
        y->parent->left = z;
    }
    else y->parent->right = z;

    if (y != x) {
        x->key = y->key;
    }

    if (y->color == BLACK) {
        Delete_fixup(z);
    }
    else {
        update_ancestor(z);
    }

    delete y;
}

node* RB_Tree::Search(int val) {
    node * x = root;
    while (!x->isNULL) {
        if (x->key > val) x = x->left;
        else if (x->key < val) x = x->right;
        else return x;
    }
    return nullNode;
}

void RB_Tree::print(node *x)  {

    if (x->isNULL) {
        cout << "()";
        return;
    }

    if (x != root) {
        cout << "(";
    }

    if (!x->isNULL) {
        cout << x->key << ":" << (x->color == BLACK ? 'b' : 'r');
    }

    if (!x->left->isNULL or !x->right->isNULL) {
        print(x->left);
        print(x->right);
    }

    if (x != root) {
        cout << ")";
    }
}


int main() {

    freopen("in.txt", "r", stdin);

    char type;
    int key;
    RB_Tree rb_tree;
    while (cin >> type >> key) {
        if (type == 'I') {
            rb_tree.Insert(key);
            rb_tree.print();
            cout << endl;
        }
        else if (type == 'D') {
            node * now = rb_tree.Search(key);
            if (!now->isNULL)
                rb_tree.Delete(now);
            rb_tree.print();
            cout << endl;
        }
        else {
            if (rb_tree.Search(key)->isNULL)
                cout << "False" << endl;
            else cout << "True" << endl;
        }
    }

    return 0;
}
