#include <bits/stdc++.h>
using namespace std;

int nCr(int n, int k)
{
    int res = 1;

    if (k > n - k)
        k = n - k;

    for (int i = 0; i < k; ++i)
    {
        res *= (n - i);
        res /= (i + 1);
    }

    return res;
}

int countseq(int *A, int n)
{
    if (n <= 2)
        return 1;
    int l = 0, r = 0, *A_L, *A_R, lt = 0, rt = 0, cnt;
    for (int i = 1; i < n; i++)
        (A[i] < A[0]) ? l++ : r++;
    A_L = new int[l], A_R = new int[r];
    for (int i = 1; i < n; i++)
        (A[i] < A[0]) ? A_L[lt++] = A[i] : A_R[rt++] = A[i];
    cnt = countseq(A_L, l) * countseq(A_R, r) * nCr(n - 1, l);
    free(A_L), free(A_R);
    return cnt;
}

int **findallseq(int *A, int n)
{
    int l = 0, r = 0, *A_L, *A_R, **B_L, **B_R, lt = 0, rt = 0, nseq = countseq(A, n), nseql, nseqr, **SEQ = new int *[nseq], idx;
    long long t1;
    for (int i = 0; i < nseq; i++)
        SEQ[i] = new int[n];
    if (nseq == 1)
    {
        for (int i = 0; i < n; i++)
            SEQ[0][i] = A[i];
        return SEQ;
    }
    for (int i = 1; i < n; i++)
        (A[i] < A[0]) ? l++ : r++;
    A_L = new int[l], A_R = new int[r];
    for (int i = 1; i < n; i++)
        (A[i] < A[0]) ? A_L[lt++] = A[i] : A_R[rt++] = A[i];
    B_L = findallseq(A_L, l), B_R = findallseq(A_R, r);
    for (long long int t = 0, nseq = 0; t < pow(2, l + r); t++)
    {
        if (__builtin_popcount(t) != r)
            continue;
        nseql = countseq(A_L, l), nseqr = countseq(A_R, r);
        for (int i = 0; i < nseql; i++)
        {
            for (int j = 0; j < nseqr; j++)
            {
                idx = n - 1, lt = l - 1, rt = r - 1, t1 = t;
                while (t || idx)
                {
                    (t & 1) ? SEQ[nseq][idx--] = B_R[j][rt--] : SEQ[nseq][idx--] = B_L[i][lt--];
                    t >>= 1;
                }
                SEQ[nseq][idx] = A[0];
                t = t1;
                ++nseq;
            }
        }
    }
    for (int i = 0; i < nseql; i++)
        free(B_L[i]);
    for (int i = 0; i < nseqr; i++)
        free(B_R[i]);
    free(A_L), free(A_R), free(B_L), free(B_R);
    return SEQ;
}

typedef struct node_
{
    int key;
    struct node_ *l = NULL, *r = NULL;
} node;

node *BSTins(node *root, int x)
{
    if (root == NULL)
    {
        root = new node;
        root->key = x;
        return root;
    }
    if (x < root->key)
        root->l = BSTins(root->l, x);
    else
        root->r = BSTins(root->r, x);
    return root;
}

node *BSTcons(int *A, int n)
{
    node *root = NULL;
    for (int i = 0; i < n; i++)
    {
        root = BSTins(root, A[i]);
    }
    return root;
}

void inorder(node *root, int *s, int *trav)
{
    if (root != NULL)
    {
        inorder(root->l, s, trav);
        trav[(*s)++] = root->key;
        inorder(root->r, s, trav);
    }
}

void preorder(node *root, int *s, int *trav)
{
    if (root != NULL)
    {
        trav[(*s)++] = root->key;
        preorder(root->l, s, trav);
        preorder(root->r, s, trav);
    }
}

void BSTprn(node *root, int n)
{
    int *trav = new int[n], s = 0;
    preorder(root, &s, trav);
    cout << "    Preorder       : ";
    for (int i = 0; i < n; i++)
        cout << left << setw(5) << trav[i];
    cout << endl;
    s = 0;
    inorder(root, &s, trav);
    cout << "    Inorder        : ";
    for (int i = 0; i < n; i++)
        cout << left << setw(5) << trav[i];
    cout << endl;
    free(trav);
}

bool BSTsame(node *T1, node *T2, int n) // trivially non-identical if number of nodes are different
{
    int *trav1 = new int[n], *trav2 = new int[n], s = 0;
    preorder(T1, &s, trav1);
    s = 0;
    preorder(T2, &s, trav2);
    for (int i = 0; i < n; i++)
        if (trav1[i] != trav2[i])
            return false;
    s = 0;
    inorder(T1, &s, trav1);
    s = 0;
    inorder(T2, &s, trav2);
    for (int i = 0; i < n; i++)
        if (trav1[i] != trav2[i])
            return false;
    return true;
}

void BSTfree(node *root)
{
    if (root != NULL)
    {
        BSTfree(root->l);
        BSTfree(root->r);
        free(root);
    }
}

void checkall(node *T, int n, int **SEQ, int nseq)
{
    for (int i = 0; i < nseq; i++)
    {
        node *T1 = BSTcons(SEQ[i], n);
        if (!BSTsame(T, T1, n))
        {
            cout << "    All trees do no match\n";
            return;
        }
        BSTfree(T1);
    }
    cout << "    All trees match\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    int *A = new int[n];
    for (int i = 0; i < n; i++)
        cin >> A[i];
    int nseq = countseq(A, n);
    cout << "+++ Sequence count\n    Total number of sequences = " << nseq << endl
         << endl;
    cout << "+++ All sequences\n";
    int **SEQ = findallseq(A, n);
    for (int i = 1; i <= nseq; i++)
    {
        cout << "    Sequence " << left << setw(5) << i << " : ";
        for (int j = 0; j < n; j++)
            cout << left << setw(5) << SEQ[i - 1][j];
        cout << endl;
    }
    node *T = BSTcons(A, n);
    cout << "\n+++ BST constructed from input array\n";
    BSTprn(T, n);
    cout << "\n+++ Checking all sequences\n";
    checkall(T, n, SEQ, nseq);
}