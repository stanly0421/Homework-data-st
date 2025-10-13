#include <iostream>
using namespace std;
unsigned long long ack(unsigned int m, unsigned int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ack(m - 1, 1);
    else
        return ack(m - 1, ack(m, n - 1));
}
int main() {
    unsigned int m, n;
    cout << "½Ð¿é¤J m »P n: ";
    cin >> m >> n;
    cout << "Ackermann(" << m << ", " << n << ") = " << ack(m, n) << endl;
    return 0;
}
