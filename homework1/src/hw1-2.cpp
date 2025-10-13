#include <iostream>
using namespace std;

// �D���j���������J�Ҩ��
int ackermann(int m, int n) {
    while (true) {
        if (m == 0) {
            // �� m = 0 �ɡA��^ n + 1
            return n + 1;
        }
        else if (n == 0) {
            // �� n = 0 �B m > 0 �ɡA�p�� A(m-1, 1)
            m = m - 1;
            n = 1;
        }
        else {
            // �� m > 0 �B n > 0 �ɡA�p�� A(m-1, A(m, n-1))
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
    cout << "�п�J m �M n ���ȡG";
    cin >> m >> n;

    cout << "���J�Ҩ�� A(" << m << ", " << n << ") ���ȬO: " << ackermann(m, n) << endl;

    return 0;
}