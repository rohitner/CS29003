#include <iostream>
using namespace std;

void m0(int *arr, int n)
{
    int i, j, k;

    for (int a = 1; a <= n; a++)
    {
        for (int b = a + 1; b <= n; b++)
        {
            for (int c = b + 1; c <= n; c++)
            {
                for (int l = 0; l < n; l++)
                {
                    if (arr[l] == a)
                        j = l;
                    if (arr[l] == b)
                        k = l;
                    if (arr[l] == c)
                        i = l;
                }
                if (i < j && j < k)
                {
                    cout << "    Method 0: Unalgolicious\n";
                    return;
                }
            }
        }
    }
    cout << "    Method 0: Algolicious\n";
    return;
}

void m1(int *arr, int n)
{
    int a, b, c;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            for (int k = j + 1; k < n; k++)
            {
                if (i == j || j == k || i == k)
                    continue;
                c = arr[i];
                a = arr[j];
                b = arr[k];
                if (a < b && b < c)
                {
                    cout << "    Method 1: Unalgolicious\n";
                    return;
                }
            }
        }
    }
    cout << "    Method 1: Algolicious\n";
    return;
}

void m2(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        int c = arr[i], lc, f = 0;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < c)
            {
                if (f == 0)
                {
                    lc = arr[j];
                    f = 1;
                    continue;
                }
                if (arr[j] >= lc)
                {
                    cout << "    Method 2: Unalgolicious\n";
                    return;
                }
                else
                    lc = arr[j];
            }
        }
    }
    cout << "    Method 2: Algolicious\n";
    return;
}

void m3(int *arr, int n)
{
    int hsf = arr[0], lsf = arr[0], hsg = arr[0];

    for (int i = 1; i < n; i++)
    {
        if (arr[i] > lsf && arr[i] < hsg)
        {
            cout << "    Method 3: Unalgolicious\n";
            return;
        }
        if (arr[i] > hsf)
            hsf = arr[i];
        if (arr[i] < lsf)
        {
            lsf = arr[i];
            hsg = hsf;
        }
    }
    cout << "    Method 3: Algolicious\n";
    return;
}

int main()
{
    int n, *a;

    cin >> n;
    a = new int[n];

    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }
    cout << "n = " << n << "\n\n+++ Sequence: ";
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
    m0(a, n);
    m1(a, n);
    m2(a, n);
    m3(a, n);

    return 0;
}