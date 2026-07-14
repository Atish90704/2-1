#include <bits/stdc++.h>
using namespace std;

// AVL Tree Node
struct AVLNode {
    int key;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(int k) {
        key = k;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

//Functions
int height(AVLNode* node) {
    return node ? node->height : 0;
}

int getBalance(AVLNode* node) {
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

// Right Rotation
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left Rotation
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Insert
AVLNode* insert(AVLNode* node, int key, bool &success) {
    if (!node) {
        success = true;
        return new AVLNode(key);
    }

    if (key < node->key)
        node->left = insert(node->left, key, success);
    else if (key > node->key)
        node->right = insert(node->right, key, success);
    else {
        success = false; // duplicate key
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // LL
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // RR
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // LR
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // RL
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Find min
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left)
        current = current->left;
    return current;
}

// Delete
AVLNode* deleteNode(AVLNode* root, int key, bool &success) {
    if (!root) {
        success = false;
        return root;
    }

    if (key < root->key)
        root->left = deleteNode(root->left, key, success);
    else if (key > root->key)
        root->right = deleteNode(root->right, key, success);
    else {
        success = true;

        // One child or no child
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;

            if (!temp) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }

            delete temp;
        }
        else {
            // Two child
            AVLNode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key, success);
        }
    }

    if (!root)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    // LL
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // LR
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // RL
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Traversal
void preorder(AVLNode* root, vector<int>& res) {
    if (!root) return;
    res.push_back(root->key);
    preorder(root->left, res);
    preorder(root->right, res);
}

void inorder(AVLNode* root, vector<int>& res) {
    if (!root) return;
    inorder(root->left, res);
    res.push_back(root->key);
    inorder(root->right, res);
}

void postorder(AVLNode* root, vector<int>& res) {
    if (!root) return;
    postorder(root->left, res);
    postorder(root->right, res);
    res.push_back(root->key);
}

void levelorder(AVLNode* root, vector<int>& res) {
    if (!root) return;
    queue<AVLNode*> q;
    q.push(root);

    while (!q.empty()) {
        AVLNode* curr = q.front();
        q.pop();
        res.push_back(curr->key);

        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int N;
    fin >> N;
    fout << N << "\n";

    AVLNode* root = nullptr;

    for (int i = 0; i < N; i++) {
        int e, x;
        fin >> e >> x;

        if (e == 1) {  // Insert
            bool success = false;
            root = insert(root, x, success);
            fout << "1 " << x << " " << (success ? 1 : 0) << "\n";
        }
        else if (e == 0) {  // Delete
            bool success = false;
            root = deleteNode(root, x, success);
            fout << "0 " << x << " " << (success ? 1 : 0) << "\n";
        }
        else if (e == 2) {  // Traversal
            vector<int> res;

            if (x == 1) preorder(root, res);
            else if (x == 2) inorder(root, res);
            else if (x == 3) postorder(root, res);
            else if (x == 4) levelorder(root, res);

            for (size_t j = 0; j < res.size(); j++) {
                fout << res[j];
                if (j + 1 != res.size()) fout << " ";
            }
            fout << "\n";
        }
    }

    fin.close();
    fout.close();
    return 0;
}