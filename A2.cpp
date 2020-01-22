#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int checkpureprod(string s)
{
    if(s[0] != '(' || s[s.length() - 1] != ')')
        return 0;

    int i = 0, cnt = 0;

    while(i < s.length())
    {
        if(s[i] == '(')
            cnt++;
        if(s[i] == ')')
            cnt--;
        if(cnt == 0 && i != s.length() - 1)
        {
            if(s[i+1] != '*')
                return 0;
            ++i;
        }
        ++i;
    }

    return 1;
}

vector<string> findallexprn(int n, vector<vector<string> > *MEM)
{
    if ((*MEM)[n].size() != 0)
        return (*MEM)[n];

    vector<string> list, A, B;
    vector<string>::iterator a, b, c;

    for (int i = 1; i <= n - 1; i++)
    {
        if (n - i >= i)
        {
            A = findallexprn(i, MEM);
            B = findallexprn(n - i, MEM);

            for (a = A.begin(); a != A.end(); a++)
            {
                for (b = B.begin(); b != B.end(); b++)
                {
                    int present = 0;
                    for (c = (*MEM)[n].begin(); c != (*MEM)[n].end(); c++)
                    {
                        if ((*c).compare(*a + "+" + *b) == 0)
                        {
                            present = 1;
                            break;
                        }
                    }
                    if (!present)
                        (*MEM)[n].push_back(*a + "+" + *b);
                }
            }
        }

        if (n % i == 0 && i != 1 && n / i >= i)
        {
            A = findallexprn(i, MEM);
            B = findallexprn(n / i, MEM);
            for (a = A.begin(); a != A.end(); a++)
            {
                for (b = B.begin(); b != B.end(); b++)
                {
                    int present = 0;
                    string rr = (checkpureprod(*a) ? *a : "(" + *a + ")") + "*" + (checkpureprod(*b) ? *b : "(" + *b + ")");
                    for (c = (*MEM)[n].begin(); c != (*MEM)[n].end(); c++)
                    {
                        if ((*c).compare(rr) == 0)
                        {
                            present = 1;
                            break;
                        }
                    }
                    if (!present)
                        (*MEM)[n].push_back(rr);
                }
            }
        }
    }
    return (*MEM)[n];
}

int count(string s)
{
    int cnt = 0;

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '1')
            cnt++;
    }

    return cnt;
}

void printallexpr(vector<string> res)
{
    vector<string>::iterator itr;
    for (itr = res.begin(); itr != res.end(); itr++)
    {
        cout << "= " << *itr << endl << "    ";
    }
    cout << "\b\b\b\b" << "... " << res.size() << " expressions" << endl;
}

void aswap(vector<string> *arr, int x, int y)
{
    string temp = (*arr)[x];
    (*arr)[x] = (*arr)[y];
    (*arr)[y] = temp;
}

void selectsort(vector<string> *arr)
{
    int n = (*arr).size(), i, j, mi;

    for(i = 0; i < n - 1; i++)
    {
        mi = i;
        for(j = i + 1; j < n; j++)
        {
            if((*arr)[j].length() > (*arr)[mi].length())
            {
                mi = j;
            }
        }
        aswap(arr, i, mi);
    }
}

int main()
{
    int n;
    cout << "n = ";
    cin >> n;
    vector<vector<string> > MEM((n > 3 ? n : 3) + 1);
    MEM[0].push_back("");
    MEM[1].push_back("1");
    MEM[2].push_back("1+1");
    MEM[3].push_back("1+1+1");

    vector<string> res = findallexprn(n, &MEM);

    cout << "\n+++ Before sorting\n" << left << setw(4) << n;
    printallexpr(res);

    vector<vector<string> > lbin(n + 1);
    vector<string>::iterator itr;
    vector<string>::reverse_iterator it;

    // bucket sort
    for (itr = res.begin(); itr != res.end(); itr++)
    {
        lbin[count(*itr)].push_back(*itr);
    }

    res.clear();

    cout << "\n+++ After sorting\n" << left << setw(4) << n;
    for (int i = n; i > 0; i--)
    {
        if(lbin[i].empty()) continue;

        selectsort(&lbin[i]);
        // for (it = lbin[i].rbegin(); it != lbin[i].rend(); it++)
        // {
        //     cout << *it << endl;
        // }
        for(itr = lbin[i].begin(); itr != lbin[i].end(); itr++)
        {
            res.push_back(*itr);
        }
    }
    printallexpr(res);

}