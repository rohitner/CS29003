#include <bits/stdc++.h>
using namespace std;

typedef struct _graph
{
    int n;
    char **a = NULL;
    int **c = NULL;
} graph;

graph readgraph()
{
    graph G;
    cin >> G.n;
    int n = G.n;
    G.a = new char *[n], G.c = new int *[n];
    for (int i = 0; i < n; i++)
    {
        G.a[i] = new char[n], G.c[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            G.a[i][j] = (i == j ? 's' : '-');
            G.c[i][j] = (i == j ? 0 : -1);
        }
    }
    int i, j, cij;
    char aij;
    while (1)
    {
        cin >> i;
        if (i == -1)
            break;
        cin >> j >> cij >> aij;
        G.a[i][j] = aij;
        G.c[i][j] = cij;
    }

    return G;
}

void prngraph(graph G)
{
    int n = G.n;
    for (int i = 0; i < n; i++)
    {
        cout << "    " << left << setw(3) << i << " -> ";
        for (int j = 0; j < n; j++)
        {
            if (i != j && G.a[i][j] != '-')
                cout << right << setw(3) << j << " (" << setw(4) << G.c[i][j] << ", " << G.a[i][j] << ") ";
        }
        cout << endl;
    }
}

graph getAIgraph(graph G)
{
    graph H;
    int n = G.n;
    H.n = n;
    H.a = new char *[n], H.c = new int *[n];
    for (int i = 0; i < n; i++)
    {
        H.a[i] = new char[n], H.c[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            H.a[i][j] = (i == j ? 's' : '-');
            H.c[i][j] = (i == j ? 0 : -1);
            if (G.a[i][j] == 'a')
                H.a[i][j] = 'a', H.c[i][j] = G.c[i][j];
        }
    }
    return H;
}

int **APSP(graph G)
{
    int n = G.n;
    int **C = new int *[n];
    for (int i = 0; i < n; i++)
        C[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = G.c[i][j];
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if ((C[i][k] != -1 && C[k][j] != -1) && (C[i][j] == -1 || C[i][k] + C[k][j] < C[i][j]))
                    C[i][j] = C[i][k] + C[k][j];
    return C;
}

void prnchart(int **C, int n)
{
    cout << "           ";
    for (int i = 0; i < n; i++)
    {
        cout << setw(5) << i << " ";
    }
    cout << endl
         << endl;
    for (int i = 0; i < n; i++)
    {
        cout << "    " << left << setw(3) << i << " -> " << right;
        for (int j = 0; j < n; j++)
        {
            cout << setw(5);
            if (C[i][j] == -1)
                cout << "-";
            else
                cout << C[i][j];
            cout << " ";
        }
        cout << endl;
    }
}

int **APSPone(graph G, int **C1)
{
    int n = G.n;
    int **C = new int *[n];
    for (int i = 0; i < n; i++)
        C[i] = new int[n];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = C1[i][j];
            if (C1[i][j] == -1)
            {
                for (int k = 0; k < n; k++)
                {
                    for (int l = 0; l < n; l++)
                    {
                        if (G.a[k][l] != 'n' || C1[i][k] == -1 || C1[l][j] == -1)
                            continue;
                        if (C[i][j] == -1 || C1[i][k] + G.c[k][l] + C1[l][j] < C[i][j])
                            C[i][j] = C1[i][k] + G.c[k][l] + C1[l][j];
                    }
                }
            }
        }
    }

    return C;
}

int **APSPany(graph G, int **C1)
{
    int n = G.n;
    int **C3 = APSP(G);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (C1[i][j] != -1)
                C3[i][j] = C1[i][j];
    return C3;
}

int main()
{
    graph G = readgraph();
    cout << "\n+++ Original graph\n";
    prngraph(G);
    graph H = getAIgraph(G);
    cout << "\n+++ AI subgraph\n";
    prngraph(H);
    int **C1 = APSP(H);
    cout << "\n+++ Cheapest AI prices\n";
    prnchart(C1, H.n);
    int **C2 = APSPone(G, C1);
    cout << "\n+++ Cheapest prices with at most one non-AI leg\n";
    prnchart(C2, G.n);
    int **C3 = APSPany(G, C1);
    cout << "\n+++ Cheapest prices with any number of non-AI legs\n";
    prnchart(C3, G.n);
}