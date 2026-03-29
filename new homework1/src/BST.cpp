#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// 插入節點
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}

// 計算高度
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}

// 刪除節點
Node* deleteNode(Node* root, int key) {
    if (!root) return nullptr;
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
        // 找到要刪除的節點
        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        else {
            // 找右子樹最小值替代
            Node* succ = root->right;
            while (succ->left) succ = succ->left;
            root->key = succ->key;
            root->right = deleteNode(root->right, succ->key);
        }
    }
    return root;
}

// 測試 (a)
void experiment() {
    srand(static_cast<unsigned int>(time(0)));
    vector<int> ns = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
    for (int n : ns) {
        Node* root = nullptr;
        for (int i = 0; i < n; i++) {
            int val = rand();
            root = insert(root, val);
        }
        int h = height(root);
        double ratio = static_cast<double>(h) / log2(n);
        cout << "n=" << n << " height=" << h << " ratio=" << ratio << endl;
    }
}

int main() {
    experiment();

    // 測試 (b)
    Node* root = nullptr;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);


    return 0;
}
