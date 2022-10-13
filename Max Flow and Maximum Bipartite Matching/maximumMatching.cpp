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
int s, t, max_flow, flow;


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
    int nVertices, nEdges, vertexnum;
    bool directed;
    ArrayList *adjLst, *edgeWeight;
    vector<vector<int> > res;
    vi parent;
    vector<bool> visited;
    vi color;
public:
    Graph(bool dir = false);
    ~Graph();
    void setnVertices(int n, bool dir = false);
    void addEdge(int u, int v, int w);
    bool isEdge(int u, int v);
    void augment(int u, int minWeight);
    pair<bool, vii > maxMatching();
    bool isBipartite();
};

///=========================bipartite_matching===================


bool Graph::isBipartite() {
    color.assign(nVertices + 3, 0);
    queue<int> q;

    for (int i = 0; i < nVertices; i++) {
        if (color[i] == 0) {
            q.push(i);
            color[i] = 1;

            while (!q.empty()) {
                auto u = q.front(); q.pop();
                int now = (color[u] == 1 ? 2 : 1);
                for (int j = 0; j < adjLst[u].getLength(); j++) {
                    int v = adjLst[u].getItem(j);
                    if (color[v] == 0) {
                        color[v] = now;
                        q.push(v);
                    }
                    else if (color[v] != now) {
                        goto loop;
                    }

                }
            }

        }
    }

    return true;

    loop:
    return false;
}

void Graph::augment(int u, int minWeight) {
    if (u == s) { flow = minWeight; return;}
    else if (parent[u] != -1) {
        augment(parent[u], min(minWeight, res[parent[u]][u]));
        res[parent[u]][u] -= flow;
        res[u][parent[u]] += flow;
    }
}

pair<bool, vii> Graph::maxMatching() {

    if (!isBipartite()) return {false, vii() };

    max_flow = 0;
    parent.assign(nVertices + 3, -1);
    visited.assign(nVertices + 3, false);

    s = nVertices, t = nVertices + 1;

    for (int i = 0; i < nVertices; i++) {///connecting edge from source and sink
        if (color[i] == 1) {
            //cout << i << " " << 1 << endl;
            addEdge(s, i, 1);
            res[i][s] = 0;
        }
        else if (color[i] == 2) {
            for (int j = 0; j < adjLst[i].getLength(); j++) {
                int v = adjLst[i].getItem(j);
                if (color[v] == 1)
                    res[i][v] = 0;
            }
            //cout << i << " " << 2 << endl;
            addEdge(i, t, 1);
            res[t][i] = 0;
        }

    }

    queue<int> q;

    while (true) {

        while (!q.empty()) q.pop();
        for (auto &v : parent)
            v = -1;
        for (int i = 0; i < visited.size(); i++)
            visited[i] = false;
        flow = 0;

        q.push(s);
        visited[s] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == t) break;

            for(int i = 0; i < adjLst[u].getLength(); i++) {
                int v = adjLst[u].getItem(i);

                if (!visited[v] and res[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        augment(t, inf);
        if (flow == 0) break;

        max_flow += flow;
    }

    vii matching_edges;

    for (int i = 0; i < nVertices; i++) {
        if (color[i] == 1) {
            for (int j = 0; j < adjLst[i].getLength(); j++) {
                int v = adjLst[i].getItem(j);
                if (res[i][v] == 0 and v != s) {
                    matching_edges.emplace_back(i, v);
                }
            }
        }
    }

    return {true, matching_edges};

}

Graph::Graph(bool dir)
{
    nVertices = 0 ;
    nEdges = 0 ;
    adjLst = 0 ;
    directed = dir ;
    edgeWeight = 0;
    res.clear();
    parent.clear();
}

void Graph::setnVertices(int n, bool dir) {
    this->nVertices = n;
    if (adjLst != 0) delete[] adjLst;
    if (edgeWeight != 0) delete[] edgeWeight;
    adjLst = new ArrayList[nVertices + 3];
    edgeWeight = new ArrayList[nVertices + 3];
    directed = dir;
    res.assign(nVertices + 3, vi(nVertices + 3, 0));
}

void Graph::addEdge(int u, int v, int w) {
    if (u < 0 || v < 0 || u >= vertexnum || v >= vertexnum) return; //vertex out of range
    this->nEdges += 2;
    adjLst[u].insertItem(v);
    edgeWeight[u].insertItem(w);
    adjLst[v].insertItem(u);
    edgeWeight[v].insertItem(w);
    res[u][v] = 1;
    res[v][u] = 1;
}

bool Graph::isEdge(int u, int v) {
    if (u < 0 || v < 0 || u >= vertexnum || v >= vertexnum) return false;

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
    if (edgeWeight) delete [] edgeWeight;
    edgeWeight = nullptr;
    adjLst = nullptr;
    res.clear();
}


int main() {
    ifstream Cin("in.txt");

    Graph g;
    int n, m, var;
    Cin >> n >> m;
    g.setnVertices(n, false);
    int cnt = 0;
    while(cnt++ < m) {
        int u, v, w;
        Cin >> u >> v;
        g.addEdge(u, v, 1);
    }

    Cin.close();

    cout << "For file output enter 1, else enter 0: "<< endl;
    cin >> var;
    if (var == 1)
        freopen("out.txt", "w", stdout);

    auto ans = g.maxMatching();
    if (!ans.first) {
        cout << "The graph is not bipartite" << endl;
    }
    else {
        cout << ans.second.size() << endl;
        for (auto &v : ans.second) {
            cout << v.first << " " << v.second << endl;
        }
    }




    return 0;
}
