/*#include <bits/stdc++.h>
using namespace std;

enum Color { RED, BLACK };

template<typename T>
class RBTree {
private:
    struct Node {
        T key;
        Color color;
        Node *left, *right, *parent;
        int size; // subtree size

        Node(T k) : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr), size(1) {}
    };

    Node* root;
    Node* NIL;

    void updateSize(Node* node) {
        if (node != NIL)
            node->size = 1 + node->left->size + node->right->size;
    }

    void fixSizeUpwards(Node* node) {
        while (node != NIL) {
            updateSize(node);
            node = node->parent;
        }
    }

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;

        if (y->left != NIL)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;

        updateSize(x);
        updateSize(y);
    }

    void rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;

        if (x->right != NIL)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;

        updateSize(y);
        updateSize(x);
    }*/

    void insertFix(Node* z) {
        while (z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    /*Node* minimum(Node* node) {
        while (node->left != NIL)
            node = node->left;
        return node;
    }*/

    void transplant(Node* u, Node* v) {
        if (u->parent == NIL)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    void deleteFix(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    /*Node* searchNode(T key) {
        Node* curr = root;
        while (curr != NIL) {
            if (key == curr->key) return curr;
            if (key < curr->key) curr = curr->left;
            else curr = curr->right;
        }
        return NIL;
    } 

public:
    RBTree() {
        NIL = new Node(T());
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;
        NIL->size = 0;
        root = NIL;
    }*/

    bool insert(T key) {
        if (searchNode(key) != NIL) return false;

        Node* z = new Node(key);
        z->left = z->right = z->parent = NIL;

        Node* y = NIL;
        Node* x = root;

        while (x != NIL) {
            y = x;
            x->size++;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;

        if (y == NIL)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        z->left = z->right = NIL;
        z->color = RED;

        insertFix(z);
        return true;
    }

    bool remove(T key) {
        Node* z = searchNode(key);
        if (z == NIL) return false;

        Node* y = z;
        Node* x;
        Color y_original = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        fixSizeUpwards(x->parent);

        if (y_original == BLACK)
            deleteFix(x);

        return true;
    }

    /*bool search(T key) {
        return searchNode(key) != NIL;
    }*/

    int countLess(T key) {
        Node* curr = root;
        int count = 0;
        while (curr != NIL) {
            if (key <= curr->key) {
                curr = curr->left;
            } else {
                count += 1 + curr->left->size;
                curr = curr->right;
            }
        }
        return count;
    }
};

/*int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int N;
    fin >> N;
    fout << N << "\n";

    RBTree<int> tree;

    while (N--) {
        int e, x;
        fin >> e >> x;
        int r = 0;

        if (e == 0)
            r = tree.remove(x);
        else if (e == 1)
            r = tree.insert(x);
        else if (e == 2)
            r = tree.search(x);
        else if (e == 3)
            r = tree.countLess(x);

        fout << e << " " << x << " " << r << "\n";
    }

    fin.close();
    fout.close();
}*/
