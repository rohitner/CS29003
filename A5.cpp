#include <bits/stdc++.h>
using namespace std;

void optimalbuy1(int n, int C, int *c, double *s, double *t, double *p)
{
    double **T = new double *[n];
    int **M = new int *[n];
    for (int i = 0; i < n; i++)
    {
        T[i] = new double[C + 1];
        M[i] = new int[C + 1];
    }

    for (int j = 0; j <= C; j++)
    {
        if (j < c[0])
            T[0][j] = -DBL_MAX;
        else
        {
            int mi = j / c[0];
            M[0][j] = mi;
            T[0][j] = (1 - pow(p[0], mi)) * s[0] + pow(p[0], mi) * t[0];
        }
    }

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j <= C; j++)
        {
            if (j < c[i])
                T[i][j] = -1;
            else
            {
                double max = -DBL_MAX;
                int f = 0;
                int maxmi;
                for (int mi = 1; mi <= j / c[i]; mi++)
                {
                    if (f == 0)
                    {
                        maxmi = mi;
                        max = T[i - 1][j - mi * c[i]] + ((1 - pow(p[i], mi)) * s[i] + pow(p[i], mi) * t[i]);
                        f = 1;
                        continue;
                    }
                    if (T[i - 1][j - mi * c[i]] + ((1 - pow(p[i], mi)) * s[i] + pow(p[i], mi) * t[i]) > max)
                    {

                        max = T[i - 1][j - mi * c[i]] + ((1 - pow(p[i], mi)) * s[i] + pow(p[i], mi) * t[i]);
                        maxmi = mi;
                    }
                }
                M[i][j] = maxmi;
                T[i][j] = max;
            }
        }
    }

    int tC = C;
    int cost = 0;
    int *marr = new int[n];
    for (int i = 0; i < n; i++)
    {
        // cout << M[n-1 - i][tC] << " ";
        marr[n - 1 - i] = M[n - 1 - i][tC];
        cost += (M[n - 1 - i][tC] * c[n - 1 - i]);
        tC -= (M[n - 1 - i][tC] * c[n - 1 - i]);
    }
    for (int i = 0; i < n; i++)
    {
        cout << "    Machine " << setw(3) << i + 1 << ": " << setw(3) << marr[i] << " copies, cost = " << setw(6) << marr[i] * c[i] << endl;
    }
    cout << "--- Total buying cost             = " << setw(6) << cost << endl;
    printf("--- Expected total profit         = %15.8lf\n", T[n - 1][C]);
    for (int i = 0; i < n; i++)
    {
        free(T[i]);
        free(M[i]);
    }
    free(T);
    free(M);
}

void optimalbuy2(int n, int C, int *c, double *s, double *t, int *r, double *p, double *q)
{
    double **T = new double *[n];
    int **M = new int *[n];
    int **R = new int *[n];
    for (int i = 0; i < n; i++)
    {
        T[i] = new double[C + 1];
        M[i] = new int[C + 1];
        R[i] = new int[C + 1];
    }

    for (int j = 0; j <= C; j++)
    {
        if (j < c[0])
            T[0][j] = -DBL_MAX;
        else
        {
            int mi = j / c[0];
            M[0][j] = mi;
            double profit = (1 - pow(p[0], mi)) * s[0] + pow(p[0], mi) * t[0];
            double prorep = (1 - pow(q[0], mi)) * s[0] + pow(q[0], mi) * t[0] - mi * r[0];
            T[0][j] = profit > prorep ? profit : prorep;
            if (profit < prorep)
                R[0][j] = 1;
        }
    }

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j <= C; j++)
        {
            if (j < c[i])
                T[i][j] = -1;
            else
            {
                double max = -DBL_MAX;
                int maxmi;
                for (int mi = 1; mi <= j / c[i]; mi++)
                {
                    if (T[i - 1][j - mi * c[i]] + ((1 - pow(p[i], mi)) * s[i] + pow(p[i], mi) * t[i]) > max)
                    {

                        max = T[i - 1][j - mi * c[i]] + ((1 - pow(p[i], mi)) * s[i] + pow(p[i], mi) * t[i]);
                        maxmi = mi;
                        if (R[i][j] == 1)
                            R[i][j] = 0;
                    }
                    if (T[i - 1][j - mi * c[i]] + ((1 - pow(q[i], mi)) * s[i] + pow(q[i], mi) * t[i]) - mi * r[i] > max)
                    {
                        max = T[i - 1][j - mi * c[i]] + ((1 - pow(q[i], mi)) * s[i] + pow(q[i], mi) * t[i]) - mi * r[i];
                        maxmi = mi;
                        R[i][j] = 1;
                    }
                }
                M[i][j] = maxmi;
                T[i][j] = max;
            }
        }
    }

    int tC = C;
    int cost = 0;
    int *marr = new int[n];
    for (int i = 0; i < n; i++)
    {
        // cout << M[n-1 - i][tC] << " ";
        marr[n - 1 - i] = M[n - 1 - i][tC];
        cost += (M[n - 1 - i][tC] * c[n - 1 - i]);
        tC -= (M[n - 1 - i][tC] * c[n - 1 - i]);
    }
    int tcost = 0;
    for (int i = 0; i < n; i++)
    {
        tcost += marr[i] * c[i];
        cout << "    Machine " << setw(3) << i + 1 << ": " << setw(3) << marr[i] << " copies, cost = " << setw(6) << marr[i] * c[i] << "  [Maintenance" << (R[i][tcost] == 1 ? " " : " not ") << "needed]" << endl;
    }
    cout << "--- Total buying cost             = " << setw(6) << cost << endl;
    printf("--- Expected total profit         = %15.8lf\n", T[n - 1][C]);
    for (int i = 0; i < n; i++)
    {
        free(T[i]);
        free(M[i]);
        free(R[i]);
    }
    free(T);
    free(M);
    free(R);
}

int main()
{
    cout.precision(8);
    cout << std::fixed;
    int n;
    cout << "n                    : ";
    cin >> n;
    int C, *c = new int[n], *r = new int[n];
    double *s = new double[n], *t = new double[n], *p = new double[n], *q = new double[n];
    cout << "Capital          (C) : ";
    cin >> C;
    cout << "Costs            (c) : ";
    for (int i = 0; i < n; i++)
        cin >> c[i];
    cout << "Selling price    (s) : ";
    for (int i = 0; i < n; i++)
        cin >> s[i];
    cout << "Selling price    (t) : ";
    for (int i = 0; i < n; i++)
        cin >> t[i];
    cout << "Maintenance cost (r) : ";
    for (int i = 0; i < n; i++)
        cin >> r[i];
    cout << "Probabilities    (p) : ";
    for (int i = 0; i < n; i++)
        cin >> p[i];
    cout << "Probabilities    (q) : ";
    for (int i = 0; i < n; i++)
        cin >> q[i];

    cout << "\n+++ Best buying option\n";
    optimalbuy1(n, C, c, s, t, p);
    cout << "+++ Best buying option\n";
    optimalbuy2(n, C, c, s, t, r, p, q);
}