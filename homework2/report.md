# 41343143
作業2


## 解題說明
題目要求設計一個 Polynomial（多項式）類別，支援以下功能：
1. 多項式的輸入與輸出
2. 多項式加法
3. 多項式乘法
4. 多項式值的代入運算

## 程式實作

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

class Polynomial;

struct Term { 
    int exp;   // 指數
    float coef; // 係數
};

class Polynomial {
private:
    vector<Term> termArray;

public:
    Polynomial() = default; 
    ~Polynomial() = default; 

    Polynomial Add(const Polynomial& b) const;   // 加法
    Polynomial Mult(const Polynomial& b) const;  // 乘法
    float Eval(float x) const;                   // 代入
    void newTerm(float coef, int exp);           // 插入新項目

    friend istream& operator>>(istream& input, Polynomial& Poly);
    friend ostream& operator<<(ostream& output, const Polynomial& Poly);
};
istream& operator>>(istream& is, Polynomial& poly) {
    poly.termArray.clear();
    int n;
    is >> n;
    for (int i = 0; i < n; ++i) {
        float coef;
        int exp;
        is >> coef >> exp;
        poly.newTerm(coef, exp);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& poly) {
    if (poly.termArray.empty()) {
        os << "0";
        return os;
    }

    for (size_t i = 0; i < poly.termArray.size(); ++i) {
        const auto& term = poly.termArray[i];
        if (i > 0 && term.coef >= 0)
            os << " + ";

        if (floor(term.coef) == term.coef)
            os << (int)term.coef;
        else
            os << term.coef;

        os << "X^" << term.exp;
    }
    return os;
}

void Polynomial::newTerm(float coef, int exp) {//插入新輸入，並將相同次方項合併
    if (fabs(coef) < 0) return; 


    for (auto it = termArray.begin(); it != termArray.end(); ++it) {
        if (it->exp == exp) {//相同合併
            it->coef += coef;
            if (fabs(it->coef) < 0)//係數為0的話刪除次方項
                termArray.erase(it);
            return;
        }
    }

    Term newTerm{ exp, coef };
    termArray.push_back(newTerm);
    sort(termArray.begin(), termArray.end(),
        [](const Term& a, const Term& b) { return a.exp > b.exp; });//由大排到小
}

Polynomial Polynomial::Add(const Polynomial& b) const {//多項式相加
    Polynomial c;
    size_t aPos = 0, bPos = 0;

    while (aPos < termArray.size() && bPos < b.termArray.size()) {
        if (termArray[aPos].exp == b.termArray[bPos].exp) {
            float sumCoef = termArray[aPos].coef + b.termArray[bPos].coef;
            if (fabs(sumCoef) > 0)
                c.newTerm(sumCoef, termArray[aPos].exp);
            aPos++; bPos++;
        }
        else if (termArray[aPos].exp > b.termArray[bPos].exp) {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
        else {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
    }

    while (aPos < termArray.size()) {
        c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
        aPos++;
    }
    while (bPos < b.termArray.size()) {
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
        bPos++;
    }

    return c;
}

Polynomial Polynomial::Mult(const Polynomial& b) const {//多項式相乘
    Polynomial result;
    for (const auto& termA : termArray)//用雙重迴圈來做交叉相乘
        for (const auto& termB : b.termArray)
            result.newTerm(termA.coef * termB.coef, termA.exp + termB.exp);
    return result;
}

float Polynomial::Eval(float x) const {//代入並計算值
    float result = 0;
    for (const auto& t : termArray)
        result += t.coef * pow(x, t.exp);
    return result;
}
int main() {
    Polynomial p1, p2, sum, product;

    cout << "Enter first polynomial (number of terms, coef exp): ";
    cin >> p1;
    cout << "Enter second polynomial (number of terms, coef exp): ";
    cin >> p2;

    cout << "\nP1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    sum = p1.Add(p2);
    cout << "\nP1 + P2 = " << sum << endl;

    product = p1.Mult(p2);
    cout << "P1 * P2 = " << product << endl;

    float x;
    cout << "\nEnter value of x to evaluate P1: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}

```
## 效能分析
| 函式                  | 時間複雜度    | 空間複雜度    |
| ------------------- | -------- | -------- |
| newTerm()        | O(n)     | O(1)     |
| Add()            | O(n + m) | O(n + m) | 
| Mult()           | O(n × m) | O(n + m) |
| Eval()           | O(n)     | O(1)     |
| operator         | O(n)     | O(1)     |
整體來看Mult()在時間複雜度以平方數成長

## 測試與驗證

###測試案例
| first polynomial | second polynomial | p1結果 | p2結果 | P1 + P2 | P1 * P2 | Enter value of x to evaluate P1| P1(x) |
|----------|--------------|----------|----------|----------|----------|----------|----------|
| 3 3 2 2 1 1 0 | 2 4 1 -1 0 | 3X^2 + 2X^1 + 1X^0 | 4X^1-1X^0 |3X^2 + 6X^1 | 12X^3 + 5X^2 + 2X^1-1X^0 | 2 | 17|

## 申論及開發報告

### 自動記憶體管理
我使用了#include[vector]來排除未初始化記憶體或陣列越界的問題，避免輸出變成亂碼

(EX:P1 + P2 = 2.1927e-40x^0 + -8.17911e+27x^219547592 + 2x^0)


### 程式易讀與明確

使用 >>、<<、Add()、Mult()、Eval()、newTerm() 等方法以直覺方式操作數學物件


透過這次撰寫Polynomial（多項式)類別讓我更了解到物件導向設計的概念，尤其是如何利用 class、struct 與 vector 來實作複雜的數學結構，雖然在newTerm與輸出亂碼函式中花了不少時間除錯，但讓我對 C++ 的實作與演算法效率有更深的認識。
