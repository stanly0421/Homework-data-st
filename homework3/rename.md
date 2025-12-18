# 41343143
作業2


## 解題說明
題目要求設計一個Polynomial程式，能夠表示並操作一元多項式。每個多項式由多個項數組成，每項包含係數與指數。程式需支援以下功能：
1. 使用環狀鏈結串列，並使用header節點
2. 多項式的輸入與輸出
3. 多項式的加減乘法運算
4. Evaluate(x)計算

## 程式實作

```cpp
#include<iostream>
#include<sstream>
#include<string>
#include<cmath>
#include<algorithm>
using namespace std;

struct Term {
    int coef;
    int exp;
    Term(int c = 0, int e = 0) :coef(c), exp(e) {}
};

struct Node {
    Term data;
    Node* link;
    Node(Term t = Term(), Node* l = nullptr) :data(t), link(l) {}
};

class Polynomial {
private:
    Node* header;
    static Node* avail;

    static Node* getNode(int c, int e, Node* l = nullptr) {
        if (avail) {
            Node* p = avail;
            avail = avail->link;
            p->data.coef = c;
            p->data.exp = e;
            p->link = l;
            return p;
        }
        return new Node(Term(c, e), l);
    }

    static void retNode(Node* p) {
        p->link = avail;
        avail = p;
    }

    void clear() {
        Node* p = header->link;
        while (p != header) {
            Node* temp = p;
            p = p->link;
            retNode(temp);
        }
        header->link = header;
    }

    void insertTerm(int c, int e) {
        if (c == 0)return;
        Node* prev = header;
        Node* curr = header->link;
        while (curr != header && curr->data.exp > e) {
            prev = curr;
            curr = curr->link;
        }
        if (curr != header && curr->data.exp == e) {
            curr->data.coef += c;
            if (curr->data.coef == 0) {
                prev->link = curr->link;
                retNode(curr);
            }
        }
        else {
            Node* newNode = getNode(c, e, curr);
            prev->link = newNode;
        }
    }

public:
    Polynomial() {
        header = new Node(Term(0, -1));
        header->link = header;
    }

    Polynomial(const Polynomial& rhs) {
        header = new Node(Term(0, -1));
        header->link = header;
        Node* p = rhs.header->link;
        while (p != rhs.header) {
            insertTerm(p->data.coef, p->data.exp);
            p = p->link;
        }
    }

    Polynomial& operator=(const Polynomial& rhs) {
        if (this == &rhs)return *this;
        clear();
        Node* p = rhs.header->link;
        while (p != rhs.header) {
            insertTerm(p->data.coef, p->data.exp);
            p = p->link;
        }
        return *this;
    }

    ~Polynomial() {
        clear();
        delete header;
    }

    float Evaluate(float x)const {
        float sum = 0;
        Node* p = header->link;
        while (p != header) {
            sum += p->data.coef * pow(x, p->data.exp);
            p = p->link;
        }
        return sum;
    }

    bool operator==(const Polynomial& b)const {
        Node* p = header->link;
        Node* q = b.header->link;
        while (p != header && q != b.header) {
            if (p->data.exp != q->data.exp || p->data.coef != q->data.coef)return false;
            p = p->link;
            q = q->link;
        }
        return p == header && q == b.header;
    }

    friend istream& operator>>(istream& is, Polynomial& p) {
        string line;
        getline(is, line);
        if (line.empty())getline(is, line);
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        for (char& ch : line)if (ch == '+')ch = ' ';
        string fixed;
        for (char ch : line) {
            if (ch == '-')fixed += " -";
            else fixed += ch;
        }
        stringstream ss(fixed);
        string token;
        p.clear();
        while (ss >> token) {
            int c = 0, e = 0;
            size_t posX = token.find('X');
            if (posX == string::npos) {
                c = stoi(token);
                e = 0;
            }
            else {
                c = stoi(token.substr(0, posX));
                size_t posCaret = token.find('^');
                e = (posCaret != string::npos) ? stoi(token.substr(posCaret + 1)) : 1;
            }
            p.insertTerm(c, e);
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        Node* node = p.header->link;
        if (node == p.header) { os << "0"; return os; }
        bool first = true;
        while (node != p.header) {
            int c = node->data.coef;
            int e = node->data.exp;
            if (!first)os << (c >= 0 ? "+" : "-");
            else if (c < 0)os << "-";
            first = false;
            int absC = abs(c);
            if (e == 0)os << absC;
            else if (e == 1)os << absC << "X";
            else os << absC << "X^" << e;
            node = node->link;
        }
        return os;
    }

    Polynomial operator+(const Polynomial& b)const {
        Polynomial result;
        Node* p = header->link;
        Node* q = b.header->link;
        while (p != header && q != b.header) {
            if (p->data.exp == q->data.exp) {
                result.insertTerm(p->data.coef + q->data.coef, p->data.exp);
                p = p->link; q = q->link;
            }
            else if (p->data.exp > q->data.exp) {
                result.insertTerm(p->data.coef, p->data.exp);
                p = p->link;
            }
            else {
                result.insertTerm(q->data.coef, q->data.exp);
                q = q->link;
            }
        }
        while (p != header) { result.insertTerm(p->data.coef, p->data.exp); p = p->link; }
        while (q != b.header) { result.insertTerm(q->data.coef, q->data.exp); q = q->link; }
        return result;
    }

    Polynomial operator-(const Polynomial& b)const {
        Polynomial result;
        Node* p = header->link;
        Node* q = b.header->link;
        while (p != header && q != b.header) {
            if (p->data.exp == q->data.exp) {
                result.insertTerm(p->data.coef - q->data.coef, p->data.exp);
                p = p->link; q = q->link;
            }
            else if (p->data.exp > q->data.exp) {
                result.insertTerm(p->data.coef, p->data.exp);
                p = p->link;
            }
            else {
                result.insertTerm(-q->data.coef, q->data.exp);
                q = q->link;
            }
        }
        while (p != header) { result.insertTerm(p->data.coef, p->data.exp); p = p->link; }
        while (q != b.header) { result.insertTerm(-q->data.coef, q->data.exp); q = q->link; }
        return result;
    }

    Polynomial operator*(const Polynomial& b)const {
        Polynomial result;
        for (Node* p = header->link; p != header; p = p->link) {
            for (Node* q = b.header->link; q != b.header; q = q->link) {
                result.insertTerm(p->data.coef * q->data.coef, p->data.exp + q->data.exp);
            }
        }
        return result;
    }
};

Node* Polynomial::avail = nullptr;

int main() {
    int x;
    Polynomial poly1, poly2;
    cout << "poly1:";
    cin >> poly1;
    cout << "poly2:";
    cin >> poly2;
    Polynomial sum = poly1 + poly2;
    cout << "(" << poly1 << ")+(" << poly2 << ")=" << sum << "\n";
    Polynomial diff = poly1 - poly2;
    cout << "(" << poly1 << ")-(" << poly2 << ")=" << diff << "\n";
    Polynomial prod = poly1 * poly2;
    cout << "(" << poly1 << ")*(" << poly2 << ")=" << prod << "\n";
    cout << "Evaluate(x),x=:";
    cin >> x;
    cout << "Evaluate poly1:" << poly1.Evaluate(x) << "\n";
    cout << "Evaluate poly2:" << poly2.Evaluate(x) << "\n";
    cout << "Evaluate(poly1+poly2):" << sum.Evaluate(x) << "\n";
    cout << "Evaluate(poly1-poly2):" << diff.Evaluate(x) << "\n";
    cout << "Evaluate(poly1*poly2):" << prod.Evaluate(x) << "\n";
    return 0;
}

```
## 效能分析
| 函式            | 時間複雜度| 空間複雜度|
| --------------- | -------- | -------- |
| insertTerm      | O(n)     | O(1)     |
| 加減法運算       | O(n + m) | O(n + m) | 
| 乘法運算         | O(n * m) | 𝑂(n * m) |
| Evaluate(x)     | O(n)     | O(1)     |

整體來看[乘法運算]在時間複雜度以平方數成長

## 測試與驗證

###測試案例

| poly1| poly2 | poly1 + poly2 | poly1 - poly2 | poly1 * poly2 |x|Evaluate(poly1) |Evaluate(poly2) |Evaluate(poly1 + poly2) |Evaluate(poly1 - poly2) |Evaluate(poly1 * poly2) |
|-----------|------------|-----------|------------|-----------|------------|-----------|------------|-----------|------------|-----------|
| 2X^2 + 3X^1 + 10X^0 | 3X^5 + 3X^2 + 2X^1 + 5X^0 | 3X^5 + 5X^2 + 5X + 15 | -3X^5 - 1X^2 + 1X + 5 |6X^7 + 9X^6 + 30X^5 + 6X^4 + 13X^3 + 46X^2 + 35X + 50 |2|24|117|141|-93|2808|

## 申論及開發報告

### 環狀鏈結串列
使用環狀鏈結串列搭配 header 節點，可以簡化插入與刪除操作，避免處理空指標問題


### available-space list

提供節點回收機制，減少記憶體配置與釋放的成本，提高效能
