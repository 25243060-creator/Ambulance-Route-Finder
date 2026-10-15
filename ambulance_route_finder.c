#include <stdio.h>
#define MAX 20
#define INF 9999

int n, g[MAX][MAX], visited[MAX];
char name[MAX][20];

void bfs(int src)
{
    int queue[MAX], front = 0, rear = 0, i, u;
    for (i = 0; i < n; i++)
        visited[i] = 0;
    queue[rear++] = src;
    visited[src] = 1;
    do
    {
        u = queue[front++];
        printf("%s ", name[u]);
        for (i = 0; i < n; i++)
            if (g[u][i] != 0 && visited[i] == 0)
            {
                visited[i] = 1;
                queue[rear++] = i;
            }
    } while (front < rear);
}

int dfs(int u)
{
    int i, count = 1;
    visited[u] = 1;
    printf("%s ", name[u]);
    for (i = 0; i < n; i++)
        if (g[u][i] != 0 && visited[i] == 0)
            count = count + dfs(i);
    return count;
}

int countPaths(int u, int dest)
{
    int i, total = 0;
    if (u == dest)
        return 1;
    visited[u] = 1;
    for (i = 0; i < n; i++)
        if (g[u][i] != 0 && visited[i] == 0)
            total = total + countPaths(i, dest);
    visited[u] = 0;
    return total;
}

void dijkstra(int src, int dist[], int parent[])
{
    int i, u, count = 0, min;
    for (i = 0; i < n; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = 0;
    }
    dist[src] = 0;
    do
    {
        u = -1;
        min = INF;
        for (i = 0; i < n; i++)
            if (visited[i] == 0 && dist[i] < min)
            {
                min = dist[i];
                u = i;
            }
        if (u == -1)
            break;
        visited[u] = 1;
        for (i = 0; i < n; i++)
            if (g[u][i] != 0 && visited[i] == 0 && dist[u] + g[u][i] < dist[i])
            {
                dist[i] = dist[u] + g[u][i];
                parent[i] = u;
            }
        count++;
    } while (count < n);
}

void printPath(int parent[], int t)
{
    if (parent[t] != -1)
    {
        printPath(parent, parent[t]);
        printf(" -> ");
    }
    printf("%s", name[t]);
}

int prim()
{
    int inTree[MAX], i, j, u, v, min, total = 0, count = 1;
    for (i = 0; i < n; i++)
        inTree[i] = 0;
    inTree[0] = 1;
    do
    {
        min = INF;
        u = -1;
        v = -1;
        for (i = 0; i < n; i++)
            if (inTree[i] == 1)
                for (j = 0; j < n; j++)
                    if (inTree[j] == 0 && g[i][j] != 0 && g[i][j] < min)
                    {
                        min = g[i][j];
                        u = i;
                        v = j;
                    }
        if (u == -1)
            break;
        inTree[v] = 1;
        total = total + min;
        printf("%s - %s : %d\n", name[u], name[v], min);
        count++;
    } while (count < n);
    return total;
}

int main()
{
    int e, i, u, v, w, depot, accident, nh, hos[MAX];
    int dist[MAX], parent[MAX], dist2[MAX], parent2[MAX];
    int best, reached;

    printf("Enter number of junctions: ");
    scanf("%d", &n);
    printf("Enter junction names:\n");
    i = 0;
    do
    {
        scanf("%s", name[i]);
        i++;
    } while (i < n);

    printf("Enter number of roads: ");
    scanf("%d", &e);
    printf("Enter roads (from to time):\n");
    i = 0;
    do
    {
        scanf("%d %d %d", &u, &v, &w);
        g[u][v] = w;
        g[v][u] = w;
        i++;
    } while (i < e);

    printf("Enter depot junction number: ");
    scanf("%d", &depot);
    printf("Enter accident junction number: ");
    scanf("%d", &accident);
    printf("Enter number of hospitals: ");
    scanf("%d", &nh);
    printf("Enter hospital junction numbers: ");
    i = 0;
    do
    {
        scanf("%d", &hos[i]);
        i++;
    } while (i < nh);

    printf("\nBFS from accident spot: ");
    bfs(accident);

    printf("\n\nDFS from depot: ");
    for (i = 0; i < n; i++)
        visited[i] = 0;
    reached = dfs(depot);
    if (reached == n)
        printf("\nRoad network is fully connected");
    else
        printf("\nRoad network is NOT fully connected");
    for (i = 0; i < n; i++)
        visited[i] = 0;
    printf("\nNumber of routes from depot to accident spot: %d\n", countPaths(depot, accident));

    printf("\nFastest route (Dijkstra)\n");
    dijkstra(depot, dist, parent);
    dijkstra(accident, dist2, parent2);
    best = hos[0];
    i = 1;
    while (i < nh)
    {
        if (dist2[hos[i]] < dist2[best])
            best = hos[i];
        i++;
    }
    printf("Depot to accident: ");
    printPath(parent, accident);
    printf(" (%d min)\n", dist[accident]);
    printf("Accident to nearest hospital: ");
    printPath(parent2, best);
    printf(" (%d min)\n", dist2[best]);
    printf("Total time: %d min\n", dist[accident] + dist2[best]);

    printf("\nMinimum cost road network (Prim)\n");
    printf("Total cost: %d\n", prim());

    printf("\nEnter blocked road (from to): ");
    scanf("%d %d", &u, &v);
    g[u][v] = 0;
    g[v][u] = 0;
    printf("\nRoute after blocking %s - %s\n", name[u], name[v]);
    dijkstra(depot, dist, parent);
    dijkstra(accident, dist2, parent2);
    best = hos[0];
    i = 1;
    while (i < nh)
    {
        if (dist2[hos[i]] < dist2[best])
            best = hos[i];
        i++;
    }
    printf("Depot to accident: ");
    printPath(parent, accident);
    printf(" (%d min)\n", dist[accident]);
    printf("Accident to nearest hospital: ");
    printPath(parent2, best);
    printf(" (%d min)\n", dist2[best]);
    printf("Total time: %d min\n", dist[accident] + dist2[best]);
    return 0;
}
