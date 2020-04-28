/* 16MA20053 NER ROHIT */
#include <iostream>
#include <iomanip>
using namespace std;

bool ischessboard(int **F, int x, int y, int lx, int ly)
{
    int col = F[x][y];
    for (int i = x; i < x + lx; i++)
    {
        for (int j = y; j < y + ly; j++)
        {
            if (i == x && j == y)
                continue;
            if (F[i][j] == col)
                return false;
            col = F[i][j];
        }
        if (ly % 2 == 0)
            col = 1 - col;
    }
    return true;
}

void prnchess(int **F, int m, int n, int x, int y, int lx, int ly)
{
    for (int i = 0; i < m; i++)
    {
        cout << "    ";
        for (int j = 0; j < n; j++)
        {
            if (i >= x && i < x + lx && j >= y && j < y + ly)
                cout << (F[i][j] == 1 ? "B " : "W ");
            else
                cout << ". ";
        }
        cout << endl;
    }
}

int essquare(int **F, int m, int n)
{
    int maxi = 0, maxj = 0, maxl = 1;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int L = min(m - i, n - j);
            for (int l = L; l > 0; l--)
            {
                if (ischessboard(F, i, j, l, l))
                {
                    if (l >= maxl)
                        maxi = i, maxj = j, maxl = l;
                    break;
                }
            }
        }
    }
    prnchess(F, m, n, maxi, maxj, maxl, maxl);
    return maxl * maxl;
}

int esrectangle(int **F, int m, int n)
{
    int maxi = 0, maxj = 0, maxl = 1, maxk = 1;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int L = m - i;
            int K = n - j;
            for (int l = L; l > 0; l--)
            {
                for (int k = K; k > 0; k--)
                {
                    if (ischessboard(F, i, j, l, k))
                    {
                        if (l * k >= maxl * maxk)
                            maxi = i, maxj = j, maxl = l, maxk = k;
                        break;
                    }
                }
            }
        }
    }
    prnchess(F, m, n, maxi, maxj, maxl, maxk);
    return maxl * maxk;
}

int dpsquare(int **F, int m, int n)
{
    int **T = new int *[m], maxi = 0, maxj = 0, maxl = 1;
    for (int i = 0; i < m; i++)
        T[i] = new int[n];
    for (int i = m - 1; i >= 0; i--)
    {
        for (int j = n - 1; j >= 0; j--)
        {
            if (i == m - 1 || j == n - 1 || (i + 1 < m && F[i][j] == F[i + 1][j]) || (j + 1 < n && F[i][j] == F[i][j + 1]))
            {
                T[i][j] = 1;
                continue;
            }
            int l1 = T[i + 1][j], l2 = T[i][j + 1];
            if (l1 > l2)
                T[i][j] = l2 + 1;
            else if (l1 < l2)
                T[i][j] = l1 + 1;
            else if (F[i + l1][j + l2] == F[i + l1 - 1][j + l2 - 1])
                T[i][j] = l1 + 1;
            else
                T[i][j] = l1;
            if (T[i][j] > maxl)
                maxi = i, maxj = j, maxl = T[i][j];
        }
    }
    prnchess(F, m, n, maxi, maxj, maxl, maxl);
    for (int i = 0; i < m; i++)
        delete (T[i]);
    delete (T);
    return maxl * maxl;
}

int dprectangle(int **F, int m, int n)
{
    int **TX = new int *[m], **TY = new int *[m], maxi = 0, maxj = 0, maxl = 1, maxk = 1;
    for (int i = 0; i < m; i++)
        TX[i] = new int[n], TY[i] = new int[n];
    for (int i = m - 1; i >= 0; i--) // TX(i, j) and TY(i, j) store length of longest valid alternating sequence starting at (i, j) in vertical and horizontal directions respectively
    {
        for (int j = n - 1; j >= 0; j--)
        {
            if (i == m - 1 || (i + 1 < m && F[i][j] == F[i + 1][j]))
                TX[i][j] = 1;
            else
                TX[i][j] = TX[i + 1][j] + 1;
            if (j == n - 1 || (j + 1 < n && F[i][j] == F[i][j + 1]))
                TY[i][j] = 1;
            else
                TY[i][j] = TY[i][j + 1] + 1;
            int l = TX[i][j], k = 1;
            int mintx = l;
            for (int k1 = 1; k1 <= TY[i][j]; k1++)
            {
                mintx = min(mintx, TX[i][j + k1 - 1]);
                if (mintx * k1 > l * k)
                {
                    l = mintx, k = k1;
                }
            }
            if (k * l > maxl * maxk)
                maxi = i, maxj = j, maxl = l, maxk = k;
        }
    }

    prnchess(F, m, n, maxi, maxj, maxl, maxk);
    for (int i = 0; i < m; i++)
        delete (TX[i]), delete (TY[i]);
    delete (TX), delete (TY);
    return maxl * maxk;
}

int main()
{
    int m, n;
    cin >> m >> n;
    int **F = new int *[m];
    for (int i = 0; i < m; i++)
        F[i] = new int[n];
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            cin >> F[i][j];
    cout << "\n+++ Exhaustive search: Square\n";
    cout << "    Area = " << essquare(F, m, n) << endl;
    cout << "\n+++ Exhaustive search: Rectangle\n";
    cout << "    Area = " << esrectangle(F, m, n) << endl;
    cout << "\n+++ Dynamic Programming: Square\n";
    cout << "    Area = " << dpsquare(F, m, n) << endl;
    cout << "\n+++ Dynamic Programming: Rectangle\n";
    cout << "    Area = " << dprectangle(F, m, n) << endl;
}
