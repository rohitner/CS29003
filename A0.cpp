#include <iostream>
#include <iomanip>
using namespace std;

typedef struct node_
{
    int date;
    struct node_ *hor;
    struct node_ *ver;
} node;

node *initcal()
{
    node *mesh = new node;
    mesh->date = 0;
    mesh->hor = NULL;
    mesh->ver = NULL;
    node *p = mesh;
    node *s = mesh;

    /* create first row */
    for (int i = 0; i < 20; i++)
    {
        node *h = new node;
        h->date = 0;
        h->ver = NULL;
        h->hor = NULL;
        p->hor = h;
        p = h;
    }

    p = mesh;

    node *prev;
    for (int i = 0; i < 19; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            node *v = new node;
            v->date = 0;
            p->ver = v;
            if (j != 0)
            {
                prev->ver->hor = v;
            }
            prev = p;
            p = p->hor;
        }
        s = s->ver;
        p = s;
    }
    return mesh;
}

int storemonth(node *mesh, int mnth, int fday)
{
    int row = (mnth / 3) * 5;
    int col = (mnth % 3) * 7;
    // cout << row << " " << col << endl;

    node *p = mesh;
    for (int j = 0; j < col; j++)
    {
        p = p->hor;
    }
    for (int i = 0; i < row; i++)
    {
        p = p->ver;
    }
    node *pp = p;

    // for(int i = 0; i < 5; i++)
    // {
    // 	node *prev = p;
    // 	for(int j = 0; j < 7; j++)
    // 	{
    // 		p->date = 1;
    // 		p = p->hor;
    // 	}
    // 	p = prev->ver;
    // }
    int mnthcnt[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int cnt = 1, jsaved;
    for (int i = 0; i < 5; i++)
    {
        node *prev = p;
        for (int j = 0; j < 7; j++)
        {
            if ((j >= fday || i > 0) && cnt <= mnthcnt[mnth])
            {
                p->date = cnt++;
                jsaved = j;
            }
            p = p->hor;
        }
        p = prev->ver;
    }

    int offset;
    if (cnt <= mnthcnt[mnth])
    {
        offset = cnt;
        p = pp;
        while (cnt <= mnthcnt[mnth])
        {
            p->date = cnt++;

            p = p->hor;
        }
        offset = cnt - offset;
    }
    else
    {
        offset = jsaved + 1;
    }

    offset %= 7;

    return offset;
}

void storeyear(node *mesh, int fday)
{
    for (int i = 0; i < 12; i++)
    {
        fday = storemonth(mesh, i, fday);
    }
}

void neatprint(node *mesh)
{
    node *p = mesh;
    node *s = mesh;
    for (int i = 0; i < 20; i++)
    {
        if (i % 5 == 0)
        {
            cout << endl;
            for (int j = 0; j < 3; j++)
            {
                cout << setw(5) << "Su" << setw(3) << "Mo" << setw(3) << "Tu" << setw(3) << "We" << setw(3) << "Th" << setw(3) << "Fr" << setw(3) << "Sa";
            }
            cout << endl
                 << setw(3);
        }
        for (int j = 0; j < 21; j++)
        {
            if (j % 7)
                cout << setw(3) << p->date;
            else
                cout << setw(5) << p->date;
            p = p->hor;
        }
        cout << endl;
        s = s->ver;
        p = s;
    }
}

int main()
{
    node *m1 = initcal();
    storeyear(m1, 3);
    neatprint(m1);
    return 0;
}