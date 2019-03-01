#include <iostream>

using namespace std;

const int N = 10;

int main()
{
    int x,y;
    char b[N];
    int a[N];
    int z,t;

    cout << "Dia chi 3 phan tu mang a : ";
    for (int i=1; i<=3; i++)
    {
        cout<<&a[i]<<' ';
    }
    cout << endl << "Dia chi 3 phan tu mang b : ";
    for (int i=1; i<=3; i++)
    {
        cout<<(int*)&b[i]<<' ';
    }

    cout << endl << "Dia chi x, y, z, t la : " << &x <<' '<< &y <<' '<< &z <<' '<< &t<<endl;
}
    /*
        Địa chỉ 3 phần từ liên tiếp của mảng a là dãy 3 giá trị tăng dần cách đều nhau 4 chính là kích thước kiểu int 4 bytes
        Tương tự b với kích thước kiểu char 1 bytes
    */
