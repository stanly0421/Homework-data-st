# 41343143
# 作業1


## 解題說明
題目要求實作抽象類別 MinPQ和繼承抽象類別MinHeap，並保持與 MaxHeap 相同的時間複雜度。

## 效能分析
抽象類別內定義及時間複雜度
1. IsEmpty():判斷空堆，時間複雜度 $O(1)$
2. Top():直接存取堆頂，時間複雜度  $O(1)$
3. Push():插入後上浮，時間複雜度 $O(\log n)$
4. Pop():移除堆頂後下沉，時間複雜度 $O(\log n)$


## 程式實作


```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};
template <class T>
class MinHeap : public MinPQ<T> {
private:
    vector<T> heap;
    void HeapifyUp(int index) {
        while (index > 1) {
            int parent = index / 2;
            if (heap[index] < heap[parent])
            {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else break;
        }
    }

    void HeapifyDown(int index) {
        int size = heap.size() - 1;
        while (true) {
            int left = 2 * index;
            int right = 2 * index + 1;
            int smallest = index;

            if (left <= size && heap[left] < heap[smallest])
                smallest = left;
            if (right <= size && heap[right] < heap[smallest])
                smallest = right;

            if (smallest != index) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else break;
        }
    }

public:
    MinHeap() {
        heap.push_back(T());
    }

    bool IsEmpty() const override {
        return heap.size() == 1;
    }

    const T& Top() const override {
        if (IsEmpty()) throw runtime_error("Heap is empty");
        return heap[1];
    }

    void Push(const T& value) override {
        heap.push_back(value);
        HeapifyUp(heap.size() - 1);
    }

    void Pop() override {
        if (IsEmpty()) throw runtime_error("Heap is empty");
        heap[1] = heap.back();
        heap.pop_back();
        if (!IsEmpty()) HeapifyDown(1);
    }

    void PrintHeap() const {
        for (size_t i = 1; i < heap.size(); i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
};
int main() {
    MinHeap<int> pq;
    int n;
    cout << "elements:";
    cin >> n;

    cout << "input " << n << " int:" << endl;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        pq.Push(x);
    }

    cout << "output:" << endl;
    pq.PrintHeap();

    return 0;
}
```
## 測試與驗證

| test | input | 原本output | 實際output |
|----------|--------------|----------|--------|
| test 1 | 5,9 4 7 6 3 | 3 4 7 9 6 | 3 4 7 9 6 |
| test 2 | 7,5 1 9 7 3 6 | 1 3 6 7 5 9 | 1 3 6 7 5 9 |
| test 3 | 9,8 4 9 3 5 7 6 1 2 | 1 2 6 3 5 9 7 8 4 |1 2 6 3 5 9 7 8 4 |

## 申論及開發報告
題目要求撰寫抽象類別MinPQ最小優先權佇列,先使用堆的功能( push , pop , top , empty ),最後再用具體類別min-heap來完成虛擬函式。


### 優點
1. 抽象化設計：介面與實作分離，方便擴充其他優先佇列。
2. 效能佳：符合理論最佳解，適合大量資料處理。
3. 簡潔實作：利用 *#include"vector"* ，避免手動記憶體管理。


### 缺點
1. 如果需要大量資料合併效率會低很多
2. 遇到重複值，順序可能亂掉


## 結論
1. 完整實作 MinPQ 的抽象介面。
2. 使用二元堆確保效能最佳化。
3. 與 MaxHeap 的複雜度一致。





# 作業2


## 解題說明
題目要求實作
- (a)建立隨機插入的二元搜尋樹 (BST)，量測樹高並與 $(\log 2n)$ 比較，觀察比例是否接近常數
- (b)撰寫刪除節點的 C++ 函式，並分析時間複雜度

##解題策略
1. 將rand()所產生的隨機數字插入BST
2. 計算高度後除以 $(\log 2n)$，觀察是否趨近常數
3. 刪除節點
4. 找要刪除的節點:
  - 如果 key 比目前節點小，就往左子樹找
  - 如果 key 比目前節點大，就往右子樹找
  - 如果剛好等於目前節點的值，代表找到要刪除的節點
5. 刪除的三種情況
  - 沒有左子樹：直接用右子樹取代，刪掉自己
  - 沒有右子樹：直接用左子樹取代，刪掉自己
  - 同時有左右子樹：
    1. 找右子樹裡最小的節點 (中序後繼)
    2. 把它的值搬到目前節點
    3. 再去右子樹刪掉那個最小值
6. 最後回傳更新後的樹根


## 效能分析
(a) 插入與高度
- 插入時間複雜度：平均 $O(\log n)$ ，最壞 $O(n)$
- 高度期望值：隨機插入的 BST 高度約為 $O(\log n)$ ，但常數因子約 2
- 實驗結果：輸出 ratio ≈ 2，符合題目預期


(b) 刪除節點
- 時間複雜度：
1. 搜尋目標節點：平均 $O(\log n)$ ，最壞 $O(n)$
2. 找中序後繼：平均 $O(\log n)$
3. 總複雜度：平均 $O(\log n)$ ，最壞 $O(n)$


## 程式實作


```cpp
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
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}
Node* deleteNode(Node* root, int key) {
    if (!root) return nullptr;
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
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
            Node* succ = root->right;
            while (succ->left) succ = succ->left;
            root->key = succ->key;
            root->right = deleteNode(root->right, succ->key);
        }
    }
    return root;
}
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

```
## 測試與驗證

| 測資 | n | 預期output(H, H/ $\log_2 n$) | 實際output(H, H/ $\log_2 n$) |
|----------|--------------|----------|----------|
| 測試一   |100  |14,2.10721|14,2.10721|
| 測試二   |500  |17,1.8961 |17,1.8961 |
| 測試三   |1000 |21,2.10721|21,2.10721|
| 測試四   |2000 |24,=2.18863|24,=2.18863|
| 測試五   |3000 |27,2.33751|27,2.33751|
| 測試六   |4000 |26,2.17286|26,2.17286|
| 測試七   |5000 |32,2.60423|32,2.60423|
| 測試八   |6000 |30,2.42697|30,2.42697|
| 測試九   |7000 |31,2.42697|31,2.42697|
| 測試十   |8000 |34,2.62229|34,2.62229|
| 測試十一 |9000 |31,2.35998|31,2.35998|
| 測試十二 |10000|32,2.40824|32,2.40824|
## 申論及開發報告
用隨機數字建 BST，量高度跟 $(\log n)$ 的比例，並使用刪除節點的函式來完成，並且觀察 $(\log _2n)$ 的比例大概在 2 左右，代表平均高度就是差不多兩倍的 $(\log n )$

## 優點
程式結構清晰，模組化設計(插入、刪除、計算高度)
程式容易擴充
刪除函式的三種情況都處理到

## 缺點
1. 樹沒有平衡機制，最壞情況下會退化成鏈結串列，效能變差
2. 使用 rand() 可能產生重複值，未處理重複插入
## 結論
題目使用不同n來隨機插入來比對在不同n的情況來建立BST並繪製高度 $(\log₂n)$ 的比值隨 n 變化的曲線，實驗顯示比值約為 2，且諾刪除操作正確，時間複雜度平均 $O(\log n)$ ，最壞情況退化為 $O(n)$
