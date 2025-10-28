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

void Polynomial::newTerm(float coef, int exp) {
    if (fabs(coef) < 0) return; 


    for (auto it = termArray.begin(); it != termArray.end(); ++it) {
        if (it->exp == exp) {
            it->coef += coef;
            if (fabs(it->coef) < 0)
                termArray.erase(it);
            return;
        }
    }

    Term newTerm{ exp, coef };
    termArray.push_back(newTerm);
    sort(termArray.begin(), termArray.end(),
        [](const Term& a, const Term& b) { return a.exp > b.exp; });
}

Polynomial Polynomial::Add(const Polynomial& b) const {
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

Polynomial Polynomial::Mult(const Polynomial& b) const {
    Polynomial result;
    for (const auto& termA : termArray)
        for (const auto& termB : b.termArray)
            result.newTerm(termA.coef * termB.coef, termA.exp + termB.exp);
    return result;
}

float Polynomial::Eval(float x) const {
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
