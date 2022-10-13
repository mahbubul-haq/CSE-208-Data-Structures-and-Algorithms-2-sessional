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

class Info {
public:
    int u, v, f, w;
    Info(int u, int v, int f, int w) {
        this->u = u, this->v = v, this->f = f, this->w = w;
    }
};


class Graph {
    int nVertices, nEdges, vertexnum;
    bool directed;
    ArrayList *adjLst, *edgeWeight;
    vector<vector<int> > res;
    vi parent;
    vector<bool> visited;

public:
    Graph(bool dir = false);
    ~Graph();
    void setnVertices(int n, bool dir = false);
    void addEdge(int u, int v, int w);
    bool isEdge(int u, int v);
    void augment(int u, int minWeight);
    pair<int, vector<Info> > maxFlow(int source, int sink);
};
///=========================max_flow===================

void Graph::augment(int u, int minWeight) {
    if (u == s) { flow = minWeight; return;}
    else if (parent[u] != -1) {
        augment(parent[u], min(minWeight, res[parent[u]][u]));
        res[parent[u]][u] -= flow;
        res[u][parent[u]] += flow;
    }
}

pair<int, vector<Info> > Graph::maxFlow(int source, int sink) {
    max_flow = 0;
    parent.assign(vertexnum, -1);
    visited.assign(vertexnum, false);
    queue<int> q;

    while (true) {

        while (!q.empty()) q.pop();
        for (auto &v : parent)
            v = -1;
        for (int i = 0; i < visited.size(); i++)
            visited[i] = false;
        flow = 0;

        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == sink) break;

            for(int i = 0; i < adjLst[u].getLength(); i++) {
                int v = adjLst[u].getItem(i);

                if (!visited[v] and res[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        augment(sink, inf);
        if (flow == 0) break;

        max_flow += flow;
    }

    vector <Info> ans;

    for (int i = 0; i < nVertices; i++) {
        for (int j = 0; j < adjLst[i].getLength(); j++) {
            int v = adjLst[i].getItem(j);
            int w = edgeWeight[i].getItem(j);
            if (v >= nVertices) {
                int tempv = v;
                v = adjLst[v].getItem(0);

                if (w > 0) {
                    ans.push_back({i, v, w - res[i][tempv], w});
                }

            }
            else {
                if (w > 0) {
                    ans.push_back({i, v, w - res[i][v], w});
                }
            }
        }
    }

    return {max_flow, ans};

}

Graph::Graph(bool dir)
{
    nVertices = 0 ;
    nEdges = 0 ;
    adjLst = 0 ;
    directed = dir ;
    edgeWeight = 0;
    vertexnum = 0;
    res.clear();
    parent.clear();
}

void Graph::setnVertices(int n, bool dir) {
    this->nVertices = n;
    if (adjLst != 0) delete[] adjLst;
    if (edgeWeight != 0) delete[] edgeWeight;
    adjLst = new ArrayList[2 * nVertices + 1];
    edgeWeight = new ArrayList[2 * nVertices + 1];
    directed = dir;
    vertexnum = n;
    res.assign(vertexnum * 2 + 1, vi(vertexnum * 2 + 1, 0));
}

void Graph::addEdge(int u, int v, int w) {
    if (directed and res[v][u] > 0) {
        addEdge(u, vertexnum, w);
        addEdge(vertexnum, v, w);
        vertexnum++;
    }
    if (u < 0 || v < 0 || u >= vertexnum || v >= vertexnum) return; //vertex out of range
    this->nEdges += 2;
    adjLst[u].insertItem(v);
    edgeWeight[u].insertItem(w);
    adjLst[v].insertItem(u);
    edgeWeight[v].insertItem(0);
    res[u][v] = w;
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
    g.setnVertices(n, true);
    int cnt = 0;
    while(cnt++ < m) {
        int u, v, w;
        Cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    Cin >> s >> t;

    Cin.close();

    cout << "For file output enter 1, else enter 0: "<< endl;
    cin >> var;
    if (var == 1)
        freopen("out.txt", "w", stdout);

    auto ans = g.maxFlow(s, t);
    cout << ans.first << endl;

    for (auto &v : ans.second) {
        cout << v.u << " " << v.v << " " << v.f << "/" << v.w << endl;
    }



    return 0;
}
