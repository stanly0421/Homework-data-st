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