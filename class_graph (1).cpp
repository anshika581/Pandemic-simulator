#include <bits/stdc++.h>
#include "TextTable.h"
using namespace std;

class Environment
{
public:
    int humidity = 100; // some value
    int precaution = 1; // some value;
};


class Node :public Environment
{
public:
    vector<int>age;
    double factor_humidity;
    double factor_precaution;
    vector<double>prob; 
    Node();
};
Node::Node()
{
    factor_humidity = abs((humidity - 50)/100) + 0.25;
    factor_precaution = (10 - precaution)*10 + 0.1;
    age.assign(85000,-1);
    prob.assign(85000,0);
}
class Graph : public Node
{
public:
    int n,m,src;
    int timer = 0;
    vector<pair<int,int>>g[85000];
    vector<bool>vis;
    vector<int>dist;
    vector<int>par;
    vector<int>size;
    set<int>req;
    vector<int>low;
    vector<int>in;
    Graph();
    ~Graph();
    void addEdge(int,int,int);
    void bfs(int);
    void takeInput();
    int findParent(int);
    void unite(int,int);
    void Isolation_required(int,int);
    void findProb();
    void disp();
};
Graph::Graph()
{
    vis.assign(85000,false);
    dist.assign(85000,0);
    in.assign(85000,0);
    low.assign(85000,0);
    par.assign(85000,-1);
    size.assign(85000,-1);
}

Graph::~Graph()
{
    //cout<<"\n-------Thank you for using the system--------"<<endl;
}

int Graph::findParent(int a)
{
    if(par[a] == a)
        return a;
    return par[a] = findParent(par[a]);
}

void Graph::unite(int a, int b)
{
    a = findParent(a);
    b = findParent(b);
    if(a!=b)
    {
        if(size[b] > size[a])
            swap(a,b);
        par[b] = a;
        size[a] += size[b];
    }
}

void Graph::addEdge(int a, int b, int w)
{
    g[a].push_back({b,w});
    g[b].push_back({a,w});
}

void Graph::Isolation_required(int src, int p)
{
    low[src] = true;
    in[src] = low[src] = timer++;
    int children = 0;
    for(auto i : g[src])
    {
        int node = i.first;
        if(node == p)
            continue;
        if(vis[node])
            low[src] = min(low[src],in[node]);
        else
        {
            Isolation_required(node,src);
            if(low[node] >= in[src] && p !=-1)
                req.insert(src);
            low[src] = min(low[src],low[node]);
            ++children;
        }
    }
    if(p == -1 && children > 1)
        req.insert(src);
}
void Graph::takeInput()
{
    cout<<"Enter the number of people : ";
    cin>>n;
    cout<<"Enter the number of relations/edges : ";
    cin>>m;
    cout<<"Enter the source of infection : ";
    cin>>src;
    for(int i = 0;i<n;i++)
    {
        int x;
        cin>>x;
        age[i+1] = x;
    }
    for(int i = 0;i<=n;i++)
    {
        par[i] = i;
        size[i] = 1;
    }
    for(int i = 0;i<m;i++)
    {
        int a,b,w;
        cin>>a>>b>>w;
        addEdge(a,b,w);
        unite(a,b);
    }
    for(int i = 1;i<=n;i++)
    {
        size[i] = size[par[i]];
        //cout<<size[i]<<" ";
    }
    bfs(src);
    vis.assign(85000,false);
    for(int i = 1;i<=n;i++)
    {
        if(!vis[i])
        {
            Isolation_required(src,-1);
        }
    }
}

void Graph::bfs(int src)
{
    queue<int>q;
    q.push(src);
    dist[src] = 0;
    vis[src] = true;
    while(!q.empty())
    {
        int t = q.front();
        q.pop();
        for(auto i : g[t])
        {
            int node = i.first;
            if(!vis[node])
            {
                vis[node] = true;
                dist[node] = dist[t] + i.second;
                q.push(node);
            }
        }
    }
    for(int i = 1;i<=n;i++)
    {
        if(i != src && !dist[i])
            dist[i] = -1;
        //cout<<dist[i]<<" ";
    }
}

// void Graph::findProb()
// {
    
// }
void Graph::disp()
{
    cout<<"The list is : "<<endl;
    TextTable t( '-', '|', '+' );
    t.add( "Node No." );
    t.add( "Age" );
    t.add( "Distance" );
    t.add("Community Size");
    t.add("Probability");
    t.endOfRow();
    for(int i = 1;i<n+1;i++)
    {
        t.add(to_string(i));
        t.add(to_string(age[i]));
        t.add(to_string(dist[i]));
        t.add(to_string(size[i]));
        t.add("NAN");
        t.endOfRow();
    }
    t.setAlignment( 5, TextTable::Alignment::RIGHT );
    cout<<t<<endl;
}
signed main() {
    freopen("out.txt", "w", stdout);
    freopen("imp.txt", "r", stdin);
    Graph o;
    o.takeInput();
    o.disp();
    return 0;
}