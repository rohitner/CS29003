#include <bits/stdc++.h>
using namespace std;

typedef struct node_
{
	node_ *left = NULL;
	node_ *right = NULL;
	int key;
} node;

void inorder(node *root, int *np)
{
	if (root == NULL)
		return;
	inorder(root->left, np);
	cout << left << setw(6) << root->key;
	++(*np);
	if (*np % 16 == 0)
		cout << "\n    ";
	inorder(root->right, np);
}

void preorder(node *root, int *np)
{
	if (root == NULL)
		return;
	cout << left << setw(6) << root->key;
	++(*np);
	if (*np % 16 == 0)
		cout << "\n    ";
	preorder(root->left, np);
	preorder(root->right, np);
}

void prntree(node *root)
{
	int np = 0;
	cout << "--- Preorder listing\n    ";
	preorder(root, &np);
	cout << endl;
	np = 0;
	cout << "--- Inorder listing\n    ";
	inorder(root, &np);
	cout << endl
		 << endl;
}

node *readtreeUtil(int **pre, int *idx, int n)
{
	int index = *idx;
	if (index == n)
		return NULL;

	node *temp = new node;
	temp->key = pre[index][0];
	(*idx)++;
	if (pre[index][1] != 0)
		temp->left = readtreeUtil(pre, idx, n);
	if (pre[index][2] != 0)
		temp->right = readtreeUtil(pre, idx, n);
	return temp;
}

node *readtree(int **pre, int n)
{
	int index = 0;

	node *root = new node;
	root->key = -INT_MIN;
	root->right = readtreeUtil(pre, &index, n);

	return root;
}

void swapchild(node *root)
{
	node *temp = root->left;
	root->left = root->right;
	root->right = temp;
}

node *rrotate(node *root)
{
	node *temp = root->left;
	root->left = root->left->right;
	temp->right = root;

	return temp;
}

node *lrotate(node *root)
{

	node *temp = root->right;
	root->right = root->right->left;
	temp->left = root;

	return temp;
}

node *makeskew(node *root)
{
	if (root == NULL)
		return NULL;
	while (root->left != NULL)
		root = rrotate(root);
	root->right = makeskew(root->right);

	return root;
}

void bsort(node *root, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		node *p = root;
		node *q = p->right;
		if (q == NULL)
			break;
		node *r = q->right;
		for (int j = 0; j < n - 1; j++)
		{
			if (q->key > q->right->key)
			{
				swapchild(q);
				p->right = rrotate(q);
				swapchild(q);
				p = r;
			}
			else
			{
				p = q;
				q = r;
			}

			r = q->right;
		}
	}
}

void lunskew(node *, int);
void runskew(node *, int);

void lunskew(node *root, int n)
{
	if (n <= 2)
		return;
	int nl = n / 2;
	int nr = n - 1 - nl;
	node *q = root->left;
	for (int i = 0; i < nr; i++)
		q = q->left;
	while (root->left != q)
		root->left = rrotate(root->left);
	lunskew(q, nl);
	runskew(q, nr);
}

void runskew(node *root, int n)
{
	if (n <= 1)
		return;
	int nl = n / 2;
	int nr = n - 1 - nl;
	node *q = root->right;
	for (int i = 0; i < nl; i++)
		q = q->right;
	while (root->right != q)
		root->right = lrotate(root->right);
	lunskew(q, nl);
	runskew(q, nr);
}

int main()
{
	int n;
	cout << "n = ";
	cin >> n;
	int **pre = new int *[n];
	for (int i = 0; i < n; i++)
	{
		pre[i] = new int[3];
	}
	for (int i = 0; i < n; i++)
	{
		cin >> pre[i][0] >> pre[i][1] >> pre[i][2];
	}
	node *root = readtree(pre, n);
	cout << "\n+++ Initial tree\n";
	prntree(root->right);

	root->right = makeskew(root->right);
	cout << "+++ Tree made skew\n";
	prntree(root->right);

	bsort(root, n);
	cout << "+++ Tree after sorting\n";
	prntree(root->right);

	cout << "+++ Tree after rebalancing\n";
	runskew(root, n);
	prntree(root->right);

	return 0;
}