#include <iostream>
using namespace std;

// 非遞迴版本的阿克曼函數
int ackermann(int m, int n) {
    while (true) {
        if (m == 0) {
            // 當 m = 0 時，返回 n + 1
            return n + 1;
        }
        else if (n == 0) {
            // 當 n = 0 且 m > 0 時，計算 A(m-1, 1)
            m = m - 1;
            n = 1;
        }
        else {
            // 當 m > 0 且 n > 0 時，計算 A(m-1, A(m, n-1))
            n = n - 1;
            int temp = m;
            m = m - 1;
            n = n + 1;
            return ackermann(m, n);
        }
    }
}

int main() {
    int m, n;
    cout << "請輸入 m 和 n 的值：";
    cin >> m >> n;

    cout << "阿克曼函數 A(" << m << ", " << n << ") 的值是: " << ackermann(m, n) << endl;

    return 0;
}