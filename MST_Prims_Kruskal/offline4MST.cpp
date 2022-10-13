#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<fstream>
using namespace std;

///the program will run on c++14 or later versions, otherwise slight errors may appear

#define vi vector<int>
#define vii vector<pair<int, int> >
const int maxn = 200000, inf = 1000000000, NULL_VALUE = -1000000000;

bool isLess(double x) {
    if (abs(x - inf) > 1e-12)
        return true;
    return false;
}

int Round(double val) {
    return (int)(val + (val > 0 ? 0.05 : -0.05));
}

class ArrayList
{
    int * list;
    int length ;
    int listMaxSize ;
    int listInitSize ;
public:
    ArrayList() ;
    ~ArrayList() ;
    int searchItem(int item) ;
    void insertItem(int item) ;
    void removeItem(int item) ;
    void removeItemAt(int position);
    int getItem(int position) ;
    int getLength();
    bool empty();
    void printList();
} ;


ArrayList::ArrayList()
{
    listInitSize = 2 ;
    listMaxSize = listInitSize ;
    list = new int[listMaxSize] ;
    length = 0 ;
}

void ArrayList::insertItem(int newitem)
{
    int * tempList ;
    if (length == listMaxSize)
    {
        //allocate new memory space for tempList
        listMaxSize = 2 * listMaxSize ;
        tempList = new int[listMaxSize] ;
        int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        delete[] list ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
    };

    list[length] = newitem ; //store new item
    length++ ;
}

int ArrayList::searchItem(int item)
{
    int i = 0;
    for (i = 0; i < length; i++)
    {
        if( list[i] == item ) return i;
    }
    return NULL_VALUE;
}

void ArrayList::removeItemAt(int position) //do not preserve order of items
{
    if ( position < 0 || position >= length ) return ; //nothing to remove
    list[position] = list[length-1] ;
    length-- ;
}


void ArrayList::removeItem(int item)
{
    int position;
    position = searchItem(item) ;
    if ( position == NULL_VALUE ) return ; //nothing to remove
    removeItemAt(position) ;
}


int ArrayList::getItem(int position)
{
    if(position < 0 || position >= length) return NULL_VALUE ;
    return list[position] ;
}

int ArrayList::getLength()
{
    return length ;
}

bool ArrayList::empty()
{
    if(length==0)return true;
    else return false;
}

void ArrayList::printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);
}

ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}


class Graph {
    int nVertices, nEdges;
    bool directed;
    ArrayList *adjLst, *edgeWeight;
    int *parent, *weight;
    vector<bool> taken;
    priority_queue<pair<int,int>, vector<pair<int, int> >, greater<> > pq;
public:
    Graph(bool dir = false);
    ~Graph();
    void setnVertices(int n, bool dir = false);
    void addEdge(int u, int v, int w);
    bool isEdge(int u, int v);
    void process(int u);
    int Find(int u);
    pair<int, vector<pair<int, int> > > prims();
    pair<int, vector<pair<int, int> > > kruskal();
};

///==============================prims=======================

void Graph::process(int u) {
    taken[u] = true;
    for (int i = 0; i < adjLst[u].getLength(); i++) {
        if (!taken[adjLst[u].getItem(i)]) {
            pq.push({edgeWeight[u].getItem(i), adjLst[u].getItem(i)});
            if (weight[adjLst[u].getItem(i)] > edgeWeight[u].getItem(i)) {
                weight[adjLst[u].getItem(i)] = edgeWeight[u].getItem(i);
                parent[adjLst[u].getItem(i)] = u;
            }
        }
    }
}

pair<int, vector<pair<int, int> > > Graph:: prims() {
    taken.resize(nVertices);
    vector<pair<int, int> > edges;

    while (!pq.empty()) pq.pop();

    for (int i = 0; i < nVertices; i++) {
        parent[i] = -1;
        weight[i] = inf;
        taken[i] = false;
    }

    process(0);
    int mst_cost = 0;
    while (!pq.empty()) {
        auto now = pq.top();
        pq.pop();

        if (!taken[now.second]) {
            mst_cost += now.first;
            edges.emplace_back(parent[now.second], now.second);
            process(now.second);
        }
    }

    return {mst_cost, edges};
};

///=================================kruskal====================

int Graph::Find(int u) { ///average cost O(logn); n = size of parent
    if (u != parent[u]) {
        parent[u] = Find(parent[u]);
    }
    return parent[u];
}

pair<int, vector<pair< int, int> > > Graph::kruskal() {

    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<> > pq1;
    pair<int, vector<pair<int, int> > > ans;
    ans.first = 0;

    for (int i = 0; i < nVertices; i++)
        parent[i] = i;

    for (int i = 0; i < nVertices; i++) {
        for (int j = 0; j < adjLst[i].getLength(); j++) {
            pq1.push({edgeWeight[i].getItem(j), {i, adjLst[i].getItem(j)}});
        }
    }

    while (!pq1.empty()) {
        auto now = pq1.top();
        pq1.pop();

        int u = Find(now.second.first), v = Find(now.second.second);
        if (u != v) {
            parent[u] = v;
            ans.first += now.first;
            ans.second.emplace_back(now.second.first, now.second.second);
        }
    }

    return ans;
}


Graph::Graph(bool dir)
{
    nVertices = 0 ;
    nEdges = 0 ;
    adjLst = 0 ;
    directed = dir ;
    edgeWeight = 0;
    parent = 0;
    weight = 0;
}

void Graph::setnVertices(int n, bool dir) {
    this->nVertices = n;
    if (adjLst != 0) delete[] adjLst;
    if (edgeWeight != 0) delete[] edgeWeight;
    if (parent != 0) delete [] parent;
    if (weight != 0) delete [] weight;
    adjLst = new ArrayList[nVertices];
    edgeWeight = new ArrayList[nVertices];
    parent = new int[nVertices];
    weight = new int[nVertices];
    for (int i = 0; i < nVertices; i++)
        parent[i] = -1, weight[i] = inf;
    directed = dir;
}

void Graph::addEdge(int u, int v, int w) {
    if (u < 0 || v < 0 || u >= nVertices || v >= nVertices) return; //vertex out of range
    this->nEdges++;
    adjLst[u].insertItem(v);
    edgeWeight[u].insertItem(w);
    if (!directed) adjLst[v].insertItem(u), edgeWeight[v].insertItem(w);
}

bool Graph::isEdge(int u, int v) {
    if (u < 0 || v < 0 || u >= nVertices || v >= nVertices) return false;

    bool edge = false;

    for (int i = 0; i < adjLst[u].getLength(); i++) {
        if (adjLst[u].getItem(i) == v) {
            edge = true;
            break;
        }
    }
    if (directed) {
        for (int i = 0; i < adjLst[v].getLength(); i++) {
            if (adjLst[v].getItem(i) == u) {
                edge = true;
                break;
            }
        }
    }

    return edge;

}

Graph::~Graph()
{
    if(adjLst) delete [] adjLst;
    if (parent) delete[] parent;
    if (weight) delete [] weight;
    adjLst = 0;
    parent = 0;
    weight = 0;

}


int main() {
    ifstream Cin("in.txt");

    Graph g;
    int n, m, var;
    Cin >> n >> m;
    g.setnVertices(n, false);

    while(m--) {
        int u, v, w;
        Cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    Cin.close();

    auto ans = g.prims();
    auto anskruskal = g.kruskal();


    cout << "For file output enter 1, else enter 0: "<< endl;
    cin >> var;
    if (var == 1)
        freopen("out.txt", "w", stdout);

    if (anskruskal.first == ans.first) {
        cout << ans.first << endl;
    }
    else {
        cout << "MST costs are not the same!" << endl;
        return 0;
    }

    cout << "Kruskal's algorithm:" << endl;
    for (auto &x : anskruskal.second)
        cout << x.first << " " << x.second << endl;

    cout << "Prim's Algorithm:" << endl;
    cout << "Root Node = " << 0 << endl;
    for (auto &x : ans.second)
        cout << x.first << " " << x.second << endl;


    return 0;
}
