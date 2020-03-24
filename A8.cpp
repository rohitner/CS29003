#include <iostream>
#include <math.h>
#include <algorithm>
#include <iomanip>
using namespace std;

typedef struct hnode_
{
    int n;
    int *k = NULL;
} hnode;

typedef struct mheap_
{
    int cap;
    int n;
    int N;
    hnode *arr = NULL;
} mheap;

mheap *initheap(int p, int nmax)
{
    int m = (int)ceil(nmax * 1.0 / p);
    mheap *H = new mheap;
    H->cap = p, H->n = 0, H->N = 0;
    H->arr = new hnode[m];
    for (int i = 0; i < m; i++)
        H->arr[i].k = new int[p], H->arr[i].n = 0;
    return H;
}

void insert(mheap *H, int x)
{
    if (H->n == H->N * H->cap)
    {
        H->arr[H->N].k[H->arr[H->N].n++] = x;
        H->n++;
        H->N = (int)ceil(H->n * 1.0 / H->cap);
    }
    else
    {
        H->arr[H->N - 1].k[H->arr[H->N - 1].n++] = x;
        H->n++;
    }
    if (H->N == 1)
        return;
    int q = H->N - 1;
    while (q > 0)
    {
        int r = (q - 1) / 2;
        int rmin = H->arr[r].k[0];
        for (int i = 0; i < H->arr[r].n; i++)
            rmin = min(H->arr[r].k[i], rmin);
        int qmax = H->arr[q].k[0];
        for (int i = 0; i < H->arr[q].n; i++)
            qmax = max(H->arr[q].k[i], qmax);
        if (rmin >= qmax)
            return;
        int m = H->arr[r].n + H->arr[q].n;
        int *temp = new int[m];
        for (int i = 0; i < H->arr[r].n; i++)
            temp[i] = H->arr[r].k[i];
        for (int i = H->arr[r].n; i < m; i++)
            temp[i] = H->arr[q].k[i - H->arr[r].n];
        sort(temp, temp + m, greater<int>());
        for (int i = 0; i < H->arr[r].n; i++)
            H->arr[r].k[i] = temp[i];
        for (int i = H->arr[r].n; i < m; i++)
            H->arr[q].k[i - H->arr[r].n] = temp[i];
        q = r;
        free(temp);
    }
}

int findmax(mheap *H)
{
    if (H->n == 0)
        return -1;
    int mx = H->arr[0].k[0];
    for (int i = 0; i < H->arr[0].n; i++)
        mx = max(H->arr[0].k[i], mx);
    return mx;
}

void heapify(mheap *H, int i)
{
    if (H->arr[i].n < H->cap || (i + 1) * H->cap == H->n || (2 * i + 1) > H->N)
        return;
    int qmin = H->arr[i].k[0], lmax, rmax, r;
    for (int j = 0; j < H->arr[i].n; j++)
        qmin = min(H->arr[i].k[j], qmin);
    if (H->arr[2 * i + 1].n == 0)
        r = 2 * i + 2;
    else if (H->arr[2 * i + 2].n == 0)
        r = 2 * i + 1;
    else
    {
        lmax = H->arr[2 * i + 1].k[0];
        for (int j = 0; j < H->arr[2 * i + 1].n; j++)
            lmax = max(H->arr[2 * i + 1].k[j], lmax);
        rmax = H->arr[2 * i + 2].k[0];
        for (int j = 0; j < H->arr[2 * i + 2].n; j++)
            rmax = max(H->arr[2 * i + 2].k[j], rmax);
        r = (rmax > lmax) ? (2 * i + 2) : (2 * i + 1);
        int m = H->arr[i].n + H->arr[r].n;
        int *temp = new int[m];
        for (int j = 0; j < H->arr[i].n; j++)
            temp[j] = H->arr[i].k[j];
        for (int j = H->arr[i].n; j < m; j++)
            temp[j] = H->arr[r].k[j - H->arr[i].n];
        sort(temp, temp + m, greater<int>());
        for (int j = 0; j < H->arr[i].n; j++)
            H->arr[i].k[j] = temp[j];
        for (int j = H->arr[i].n; j < m; j++)
            H->arr[r].k[j - H->arr[i].n] = temp[j];
        free(temp);
    }
    heapify(H, r);
}

void delmax(mheap *H)
{
    if (H->n == 0)
        return;
    if (H->n == 1)
    {
        H->n = 0;
        H->N = 0;
        return;
    }
    int m = H->arr[0].n;
    sort(H->arr[0].k, H->arr[0].k + m);
    if (H->N == 1)
        H->arr[0].n--;
    else
        H->arr[0].k[m - 1] = H->arr[H->N - 1].k[H->arr[H->N - 1].n-- - 1];
    H->n--;
    H->N = (int)ceil(H->n * 1.0 / H->cap);
    heapify(H, 0);
}

void prnheap(mheap *H)
{
    for (int i = 0; i < H->N; i++)
    {
        cout << "    [";
        for (int j = 0; j < H->arr[i].n; j++)
            cout << right << setw(5) << H->arr[i].k[j] << " ";
        for (int j = H->arr[i].n; j < H->cap; j++)
            cout << "      ";
        cout << " ]\n";
    }
}

int main()
{
    int p, nmax;
    cin >> p >> nmax;
    int *A = new int[nmax];
    for (int i = 0; i < nmax; i++)
        cin >> A[i];
    mheap *H = initheap(p, nmax);
    for (int i = 0; i < nmax; i++)
        insert(H, A[i]);
    cout << "\n+++ " << nmax << " insertions made\n";
    prnheap(H);
    for (int i = nmax - 1; i >= 0; i--)
    {
        A[i] = findmax(H);
        delmax(H);
    }
    cout << "\n+++ " << nmax << " deletions made\n\n+++ Input array sorted";
    for (int i = 0; i < nmax; i++)
    {
        if (i % 20 == 0)
            cout << endl;
        cout << left << setw(5) << A[i];
    }
    cout << endl;
}