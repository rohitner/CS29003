#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EDGE_PROB 0.1

typedef struct _node {
   int nbr;
   struct _node *next;
} node;

typedef struct {
   int n;
   int *V;
   char *C;
   node **E;
} graph;

node *insertend ( node *E, node *p )
{
   node *q;

   if (E == NULL) return p;
   q = E;
   while (q -> next) q = q -> next;
   q -> next = p;
   return E;
}

graph gengraph ( int n )
{
   graph G;
   node *p;
   int u, v;

   G.n = n;

   G.V = (int *)malloc(n * sizeof(int));
   for (u=0; u<n; ++u) G.V[u] = u;

   G.C = (char *)malloc(n * sizeof(char));
   for (u=0; u<n; ++u) G.C[u] = (rand() % 2) ? 'r' : 'b';

   G.E = (node **)malloc(n * sizeof(node *));
   for (u=0; u<n; ++u) G.E[u] = NULL;
   for (u=0; u<=n-2; ++u) {
      for (v=u+1; v<=n-1; ++v) {
         if (((double)rand() / (double)RAND_MAX) <= EDGE_PROB) {
            p = (node *)malloc(sizeof(node));
            p -> nbr = v;
            p -> next = NULL;
            G.E[u] = insertend(G.E[u],p);

            p = (node *)malloc(sizeof(node));
            p -> nbr = u;
            p -> next = NULL;
            G.E[v] = insertend(G.E[v],p);
         }
      }
   }

   printf("%d\n", n);
   for (u=0; u<n; ++u) printf("%c ", G.C[u]);
   printf("\n");
   for (u=0; u<n; ++u) {
      p = G.E[u];
      while (p) {
         if (u < p -> nbr) printf("%-3d %-3d\n", u, p -> nbr);
         p = p -> next;
      }
   }
   printf("-1\n");

   return G;
}

void prngraph ( graph G )
{
   int u;
   node *p;

   for (u=0; u<G.n; ++u) {
      printf("    [%c] %3d -> ", G.C[u], G.V[u]);
      p = G.E[u];
      while (p) {
         printf("%3d", G.V[p -> nbr]);
         p = p -> next;
         if (p) printf(",");
      }
      printf("\n");
   }
}

graph getcolgraph ( graph G, char color )
{
   graph H;
   node *p, *q;
   int n, u, v, w;
   int *I;

   I = (int *)malloc(G.n * sizeof(int));
   for (u=0; u<G.n; ++u) I[u] = -1;
   n = 0;
   for (u=0; u<G.n; ++u) if (G.C[u] == color) {
      I[u] = n;
      ++n;
   }
   H.n = n;

   if (n == 0) {
      H.C = NULL;
      H.V = NULL;
      H.E = NULL;
      free(I);
      return H;
   }

   H.V = (int *)malloc(n * sizeof(int));
   H.C = (char *)malloc(n * sizeof(char));
   H.E = (node **)malloc(n * sizeof(node *));
   for (w=0; w<n; ++w) H.E[w] = NULL;
   w = 0;

   for (u=0; u<G.n; ++u) if (G.C[u] == color) {
      H.C[w] = color;
      H.V[w] = u;
      p = G.E[u];
      while (p) {
         v = p -> nbr;
         if (G.C[v] == color) {
            q = (node *)malloc(sizeof(node));
            q -> nbr = I[v];
            q -> next = NULL;
            H.E[w] = insertend(H.E[w],q);
         }
         p = p -> next;
      }
      ++w;
   }

   free(I);
   return H;
}

void DFS ( graph G, int u, int *visited, int *parent, int *level )
{
   node *p;
   int v, w;

   visited[u] = 1;
   p = G.E[u];
   while (p) {
      v = p -> nbr;
      if (visited[v] == 0) {
         parent[v] = u;
         level[v] = level[u] + 1;
         DFS(G,v,visited,parent,level);
      } else if (level[v] <= level[u]-2) {
         printf("    (");
         w = u;
         while (1) {
            printf("%d", G.V[w]);
            w = parent[w];
            if (w != v) {
               printf(", ");
            } else {
               printf(", %d), Color: (", G.V[w]);
               break;
            }
         }
         w = u;
         while (1) {
            printf("%c", G.C[w]);
            w = parent[w];
            if (w != v) {
               printf(", ");
            } else {
               printf(", %c)\n", G.C[w]);
               break;
            }
         }
      }
      p = p -> next;
   }
}

int *multiDFS ( graph G )
{
   int u, *visited, *parent, *level;

   visited = (int *)malloc(G.n * sizeof(int));
   parent = (int *)malloc(G.n * sizeof(int));
   level = (int *)malloc(G.n * sizeof(int));
   for (u=0; u<G.n; ++u) {
      visited[u] = 0;
      parent[u] = -1;
      level[u] = -1;
   }
   for (u=0; u<G.n; ++u) {
      if (visited[u] == 0) {
         level[u] = 0;
         DFS(G,u,visited,parent,level);
      }
   }
   free(visited);
   free(level);
   return parent;
}

graph getrbgraph ( graph G, graph GR, graph GB, int *FR, int *FB )
{
   graph H;
   node *p, *q;
   int n, u, v;

   n = G.n;
   H.n = n;
   H.V = (int *)malloc(n * sizeof(int));
   H.C = (char *)malloc(n * sizeof(char));
   H.E = (node **)malloc(n * sizeof(node *));
   for (u=0; u<n; ++u) {
      H.V[u] = u;
      H.C[u] = G.C[u];
      H.E[u] = NULL;
   }

   for (u=0; u<GR.n; ++u) {
      v = FR[u];
      if (v != -1) {
         p = (node *)malloc(sizeof(node));
         p -> nbr = GR.V[v];
         p -> next = H.E[GR.V[u]];
         H.E[GR.V[u]] = p;

         p = (node *)malloc(sizeof(node));
         p -> nbr = GR.V[u];
         p -> next = H.E[GR.V[v]];
         H.E[GR.V[v]] = p;
      }
   }

   for (u=0; u<GB.n; ++u) {
      v = FB[u];
      if (v != -1) {
         p = (node *)malloc(sizeof(node));
         p -> nbr = GB.V[v];
         p -> next = H.E[GB.V[u]];
         H.E[GB.V[u]] = p;
         
         p = (node *)malloc(sizeof(node));
         p -> nbr = GB.V[u];
         p -> next = H.E[GB.V[v]];
         H.E[GB.V[v]] = p;
      }
   }

   for (u=0; u<n; ++u) {
      p = G.E[u];
      while (p) {
         v = p -> nbr;
         if ((u < v) && (G.C[u] != G.C[v])) {
            q = (node *)malloc(sizeof(node));
            q -> nbr = v;
            q -> next = H.E[u];
            H.E[u] = q;

            q = (node *)malloc(sizeof(node));
            q -> nbr = u;
            q -> next = H.E[v];
            H.E[v] = q;
         }
         p = p -> next;
      }
   }

   return H;
}

int main ( int argc, char *argv[] )
{
   int n;
   graph G, GR, GB, GRB;
   int *FR, *FB;

   srand((unsigned int)time(NULL));
   n = (argc > 1) ? atoi(argv[1]) : 20;

   G = gengraph(n);
   printf("\n+++ Original graph (G)\n");
   prngraph(G);

   GR = getcolgraph(G,'r');
   printf("\n+++ Red subgraph (GR)\n");
   prngraph(GR);

   GB = getcolgraph(G,'b');
   printf("\n+++ Blue subgraph (GB)\n");
   prngraph(GB);

   printf("\n+++ Red cycles\n");
   FR = multiDFS(GR);

   printf("\n+++ Blue cycles\n");
   FB = multiDFS(GB);

   GRB = getrbgraph(G,GR,GB,FR,FB);
   printf("\n+++ Nonmonochromatic graph (GRB)\n");
   prngraph(GRB);

   free(FR); free(FB);

   printf("\n+++ Multi-color cycles\n");
   free(multiDFS(GRB));

   exit(0);
}

// #include <bits/stdc++.h>
// using namespace std;

// typedef struct graph_
// {
//     int n, nr, nb;
//     char *col = NULL;
//     vector<int> Vr;
//     vector<int> Vb;
//     vector<vector<int> > E;
// } graph;

// graph readgraph(graph G)
// {
//     cin >> G.n;
//     G.nr = 0, G.nb = 0;
//     G.col = new char[G.n];
//     for (int i = 0; i < G.n; i++)
//     {
//         cin >> G.col[i];
//         if (G.col[i] == 'r')
//             G.nr++, G.Vr.push_back(i);
//         else
//             G.nb++, G.Vb.push_back(i);
//     }
//     G.E.assign(G.n, vector<int>());
//     int a, b;
//     while (1)
//     {
//         cin >> a;
//         if (a == -1)
//             break;
//         cin >> b;
//         G.E[a].push_back(b);
//         G.E[b].push_back(a);
//     }
//     return G;
// }

// void prngraph(graph G)
// {
//     int ir = 0, ib = 0;
//     for (int i = 0; i < G.n; i++)
//     {
//         cout << "    [" << G.col[i] << "] " << setw(3) << (G.col[i] == 'r' ? G.Vr[ir++] : G.Vb[ib++]) << " -> ";
//         if (!G.E[i].empty())
//             cout << setw(3) << G.E[i][0];
//         for (int j = 1; j < G.E[i].size(); j++)
//             cout << "," << setw(3) << (G.col[G.E[i][j]] == 'r' ? G.Vr[G.E[i][j]] : G.Vb[G.E[i][j]]);
//         cout << endl;
//     }
// }

// graph getcolgraph(graph G, char color)
// {
//     graph Gc;
//     if (color == 'r')
//     {
//         Gc.n = G.nr;
//         Gc.nr = G.nr;
//         Gc.nb = 0;
//         Gc.col = new char[Gc.n];
//         for (int i = 0; i < Gc.n; i++)
//             Gc.col[i] = 'r';
//         for (int i = 0; i < G.n; i++)
//             if (G.col[i] == 'r')
//                 Gc.Vr.push_back(i);
//         Gc.E.assign(Gc.n, vector<int>());
//         for (int i = 0; i < Gc.n; i++)
//             for (int j = 0; j < G.E[Gc.Vr[i]].size(); j++)
//                 if (G.col[G.E[Gc.Vr[i]][j]] == 'r')
//                     Gc.E[i].push_back(find(Gc.Vr.begin(), Gc.Vr.end(), G.E[Gc.Vr[i]][j]) - Gc.Vr.begin());
//     }
//     else if (color == 'b')
//     {
//         Gc.n = G.nb;
//         Gc.nr = 0;
//         Gc.nb = G.nb;
//         Gc.col = new char[Gc.n];
//         for (int i = 0; i < Gc.n; i++)
//             Gc.col[i] = 'b';
//         for (int i = 0; i < G.n; i++)
//             if (G.col[i] == 'b')
//                 Gc.Vb.push_back(i);
//         Gc.E.assign(Gc.n, vector<int>());
//         for (int i = 0; i < Gc.n; i++)
//             for (int j = 0; j < G.E[Gc.Vb[i]].size(); j++)
//                 if (G.col[G.E[Gc.Vb[i]][j]] == 'b')
//                     Gc.E[i].push_back(find(Gc.Vb.begin(), Gc.Vb.end(), G.E[Gc.Vb[i]][j]) - Gc.Vb.begin());
//     }

//     return Gc;
// }

// void DFS(graph G, int u, bool *visited, int *parent, int *level)
// {
//     cout << u << " ";
//     visited[u] = true;
//     for (int i = 0; i < G.E[u].size(); i++)
//     {
//         int v = G.E[u][i];
//         if (!visited[v])
//         {
//             parent[v] = u;
//             level[v] = level[u] + 1;
//             DFS(G, v, visited, parent, level);
//         }
//     }
//     cout << "\n";
//     // for(int i = 0; i < G.E[u].size(); i++)
//     // {
//     //     int v = G.E[u][i];
//     //     if(visited[v] && level[v] < level[u] && parent[u] != v)
//     //     {
//     //         int cur = u;
//     //         while(cur != v)
//     //         {
//     //             cout << cur << " ";
//     //             cur = parent[cur];
//     //         }
//     //         cout << v << endl;
//     //     }
//     // }
    
// }

// int *multiDFS(graph G)
// {
//     for(int i = 0; i < G.Vr.size(); i++)
//         cout << i << "|" << G.Vr[i] << " ";
//     cout << endl;
//     bool *visited = new bool[G.n];
//     int *parent = new int[G.n];
//     int *level = new int[G.n];
//     for (int i = 0; i < G.n; i++)
//         visited[i] = false, parent[i] = -1, level[i] = 0;
//     for (int i = 0; i < G.n; i++)
//         if (!visited[i])
//             DFS(G, i, visited, parent, level);
//     return parent;
// }

// int main()
// {
//     graph G;
//     G = readgraph(G);
//     cout << "\n+++ Original graph (G)\n";
//     prngraph(G);
//     graph GR = getcolgraph(G, 'r');
//     cout << "\n+++ Red subgraph (GR)\n";
//     prngraph(GR);
//     graph GB = getcolgraph(G, 'b');
//     cout << "\n+++ Blue subgraph (GB)\n";
//     prngraph(GB);
//     multiDFS(GR);
// }