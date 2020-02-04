#include <bits/stdc++.h>
using namespace std;

void greedy1(int n)
{
    cout << "    Start     : " << n << endl;
    int cnt = 0;
    while (n > 1)
    {
        cnt++;
        if (n % 2 == 0)
        {
            n /= 2;
            cout << "    Divide    : " << n << endl;
        }
        else
        {
            n--;
            cout << "    Decrement : " << n << endl;
        }
    }
    cout << "--- Number of steps = " << cnt << endl;
}

int fac2(int n)
{
    int cnt = 0;
    while (n % 2 == 0)
    {
        ++cnt;
        n /= 2;
    }

    return cnt;
}

void greedy2(int n)
{
    cout << "    Start     : " << n << endl;
    int cnt = 0;
    while (n > 1)
    {
        cnt++;
        if (n % 2 == 0)
        {
            n /= 2;
            cout << "    Divide    : " << n << endl;
        }
        else
        {
            if (fac2(n + 1) < fac2(n - 1) && n != 3)
            {
                cout << "    Decrement : " << --n << endl;
            }
            else if (n != 3)
            {
                cout << "    Increment : " << ++n << endl;
            }
            else
            {
                cout << "    Decrement : " << --n << endl;
                cout << "    Decrement : " << --n << endl;
                cnt++;
                break;
            }
        }
    }
    cout << "--- Number of steps = " << cnt << endl;
}

void greedy3(int n, int k, int *a)
{
    cout << "    Start     : " << n << endl;
    int cnt = 0;
    while (n > 1)
    {
        cnt++;

        if (n == 3)
        {
            cout << "    Decrement : " << --n << endl;
            cout << "    Decrement : " << --n << endl;
            cnt++;
            break;
        }

        if (n % 2 == 0)
        {
            n /= 2;
            cout << "    Divide    : " << n << endl;
        }
        else
        {
            int max2;
            for (int i = 0; i < k; i++)
            {
                if (i == 0)
                {
                    max2 = i;
                    continue;
                }
                if ((fac2(n + a[i]) >= fac2(n + a[max2])))
                {
                    if (fac2(n + a[i]) == fac2(n + a[max2]))
                        max2 = a[max2] < a[i] ? max2 : i;
                    else
                        max2 = i;
                }
            }
            n += a[max2];
            cout << "    Add" << setw(3) << a[max2] << "    : " << n << endl;
        }
    }
    cout << "--- Number of steps = " << cnt << endl;
}

void optimal(int n, int k, int *a)
{
    cout << "    Start     : " << n << endl;
    int m = n + floor(log2(n)), improved, *opt = new int[m + 1], *parent = new int[m + 1], j, l;

    for (int i = 0; i <= m; i++)
        opt[i] = -1;

    opt[1] = 0;

    while (1)
    {
        improved = 0;
        for (int i = 2; i <= m; i++)
        {
            if (i % 2 == 0)
            {
                j = i / 2;
                if ((opt[j] + 1 < opt[i] || opt[i] == -1) && opt[j] != -1)
                {
                    opt[i] = opt[j] + 1;
                    parent[i] = j;
                    ++improved;
                }
            }
            for (j = 0; j < k; j++)
            {
                l = i + a[j];
                if ((((l > 0) && (l <= m) && (opt[l] + 1 < opt[i])) || opt[i] == -1) && opt[l] != -1)
                {
                    opt[i] = opt[l] + 1;
                    parent[i] = l;
                }
            }
        }
        if (!improved)
            break;
    }

    int cur = n;
    int cnt = 0;
    while (parent[cur] != 1)
    {
        cnt++;
        if (parent[cur] == cur / 2)
        {
            cout << "    Divide    : " << parent[cur] << endl;
        }
        else
        {
            cout << "    Add" << setw(3) << parent[cur] - cur << "    : " << parent[cur] << endl;
        }
        cur = parent[cur];
    }
    if (cur == 2)
        cout << "    Divide    : " << parent[cur] << endl;
    else
        cout << "    Add" << setw(3) << parent[cur] - cur << "    : " << parent[cur] << endl;
    cnt++;
    cout << "--- Number of steps = " << cnt << endl;
}

int main()
{
    cout << "n = ";
    int n;
    cin >> n;
    cout << "\n+++ Greedy 1\n";
    greedy1(n);
    cout << "\n+++ Greedy 2\n";
    greedy2(n);
    cout << "k = ";
    int k;
    cin >> k;
    int *a = new int[k];
    for (int i = 0; i < k; i++)
    {
        cin >> a[i];
    }
    cout << "\n+++ Greedy 3\n";
    greedy3(n, k, a);
    cout << "\n+++ Optimal\n";
    optimal(n, k, a);
}