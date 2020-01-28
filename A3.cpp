#include <iostream>
#include <iomanip>
#include <stack>
using namespace std;

typedef struct line_
{
    double m, c;
    int num;
} line;

typedef struct pt_
{
    double x, y;
} pt;

pt intersect(line l1, line l2)
{
    pt cross;
    cross.x = (l2.c - l1.c) / (l1.m - l2.m);
    cross.y = l1.m * cross.x + l1.c;

    return cross;
}

bool checkvisited(bool *visited, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (visited[i] == false)
        {
            return false;
        }
    }
    return true;
}

void method1(line *lines, int n)
{
    bool *visited = new bool[n];
    int min = 0, mint = 0;
    for (int i = 0; i < n; i++)
    {
        if (lines[i].m < lines[min].m)
            min = i;
    }
    pt curpt = {-100000, 0};
    int first = 1;

    // for(int ll = 0; ll < 6; ll++)
    while (checkvisited(visited, n) == false)
    {
        // for(int i = 0; i < n; i++)
        // {
        // 	cout << visited[i] << " ";
        // }
        // cout << endl;
        // cout << "min : " << min << endl;
        visited[min] = 1;
        line cur = lines[min];
        pt minpt;
        mint = min;
        int flag = 0;
        for (int i = 0; i < n; i++)
        {
            if (visited[i])
                continue;
            if (flag == 0)
            {
                for (int ii = 0; ii < n; ii++)
                {
                    if (visited[ii])
                        continue;
                    pt temp = intersect(cur, lines[ii]);
                    if (temp.x > curpt.x)
                        minpt = temp;
                }
                // cout << "ii " << minpt.x << "\n";
                flag = 1;
                // continue;
            }
            pt intr = intersect(cur, lines[i]);
            // cout << i << " " << intr.x << endl;
            if (intr.x <= curpt.x)
                visited[i] = 1;
            if (intr.x <= minpt.x && intr.x > curpt.x)
            {
                minpt = intr;
                min = i;
            }
        }
        if (first)
        {
            cout << "Line " << setw(4) << mint << ": From MINUS_INFINITY to (" << minpt.x << "," << minpt.y << ")\n";
            first = 0;
        }
        else if (checkvisited(visited, n) == true)
        {
            cout << "Line " << setw(4) << mint << ": From "
                 << "(" << minpt.x << "," << minpt.y << ") to PLUS_INFINTIY\n";
        }
        else
        {
            cout << "Line " << setw(4) << mint << ": From "
                 << "(" << curpt.x << "," << curpt.y << ") to (" << minpt.x << "," << minpt.y << ")\n";
        }
        curpt = minpt;
    }
}

void merge(line *lines, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    line L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = lines[l + i];
    for (j = 0; j < n2; j++)
        R[j] = lines[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].m <= R[j].m)
        {
            lines[k] = L[i];
            i++;
        }
        else
        {
            lines[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        lines[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        lines[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(line *lines, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        mergeSort(lines, l, m);
        mergeSort(lines, m + 1, r);

        merge(lines, l, m, r);
    }
}

void method2(line *lines, int n)
{
    stack<line> st, st1;
    line temp;
    pt prev = {-1000000, 0};

    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            st.push(lines[0]);
            continue;
        }
        // cout << intersect(st.top(), lines[i]).x << " " << prev.x << endl;
        while (intersect(st.top(), lines[i]).x < prev.x)
        {
            if (st.size() == 2)
            {
                st.pop();
                break;
            }
            st.pop();
            temp = st.top();
            st.pop();
            prev = intersect(st.top(), temp);
            st.push(temp);
        }
        prev = intersect(st.top(), lines[i]);
        st.push(lines[i]);
    }
    while (!st.empty())
    {
        st1.push(st.top());
        st.pop();
    }

    prev = {-100000, 0};
    int first = 1;
    while (!st1.empty())
    {
        if (first)
        {
            cout << "Line " << setw(4) << st1.top().num << ": From MINUS_INFINITY to ";
            first = 0;
        }
        else
            cout << "Line " << setw(4) << st1.top().num << ": From "
                 << "(" << prev.x << "," << prev.y << ") to ";
        temp = st1.top();
        st1.pop();
        if (!st1.empty())
        {
            prev = intersect(temp, st1.top());
            cout << "(" << prev.x << "," << prev.y << ")" << endl;
        }
        else
            cout << "PLUS_INFINTIY" << endl;
    }
}

int main()
{
    int n;

    cout << "n = ";
    cin >> n;

    line *lines = new line[n];
    for (int i = 0; i < n; i++)
    {
        cin >> lines[i].m >> lines[i].c;
        lines[i].num = i;
    }
    cout << "\n+++ Lines before sorting\n";
    cout.precision(10);
    cout << std::fixed;
    for (int i = 0; i < n; i++)
    {
        cout << "Line " << setw(4) << i << ": y =" << setw(18) << lines[i].m << " x " << (lines[i].c >= 0 ? "+" : "-") << setw(16) << (lines[i].c >= 0 ? lines[i].c : -1 * lines[i].c) << "\n";
    }

    cout << "\n+++ Method 1\n";
    method1(lines, n);

    mergeSort(lines, 0, n - 1);
    cout << "\n+++ Lines after sorting\n";
    for (int i = 0; i < n; i++)
    {
        cout << "Line " << setw(4) << lines[i].num << ": y =" << setw(18) << lines[i].m << " x " << (lines[i].c >= 0 ? "+" : "-") << setw(16) << (lines[i].c >= 0 ? lines[i].c : -1 * lines[i].c) << "\n";
    }
    cout << "\n+++ Method 2\n";
    method2(lines, n);

    return 0;
}
