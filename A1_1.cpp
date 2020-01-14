#include <iostream>
#include <math.h>
using namespace std;

double lorec(int n);

double hirec(int n)
{
    if (n == 0)
        return 1;
    return 2 * hirec(n - 1) + lorec(n - 1);
}

double lorec(int n)
{
    if (n == 0)
        return 0;
    return hirec(n - 1) + lorec(n - 1);
}

double *hilorec(int n)
{
    double *HL = new double[2], temp;
    HL[0] = 1;
    HL[1] = 0;

    for (int i = 0; i < n; i++)
    {
        temp = 2 * HL[0] + HL[1];
        HL[1] = HL[0] + HL[1];
        HL[0] = temp;
    }

    return HL;
}

double *hiloformula(int n)
{
    double *HL = new double[2];
    HL[0] = ((5 + sqrt(5)) / 10) * pow((3 - sqrt(5)) / 2, n + 1) + ((5 - sqrt(5)) / 10) * pow((3 + sqrt(5)) / 2, n + 1);
    HL[1] = ((-5 - 3 * sqrt(5)) / 10) * pow((3 - sqrt(5)) / 2, n + 1) + ((-5 + 3 * sqrt(5)) / 10) * pow((3 + sqrt(5)) / 2, n + 1);
    return HL;
}

int main()
{
    int n;
    double *a;

    cin >> n;
    cout << "n = " << n << endl
         << endl;
    cout.precision(10);
    cout << std::scientific;
    cout << "+++ Method 0\n    "
         << "hi(" << n << ") = " << hirec(n) << ", lo(" << n << ") = " << lorec(n) << endl
         << endl;
    a = hilorec(n);
    cout << "+++ Method 1\n    "
         << "hi(" << n << ") = " << a[0] << ", lo(" << n << ") = " << a[1] << endl
         << endl;
    a = hiloformula(n);
    cout << "+++ Method 2\n    "
         << "hi(" << n << ") = " << a[0] << ", lo(" << n << ") = " << a[1] << endl;

    return 0;
}