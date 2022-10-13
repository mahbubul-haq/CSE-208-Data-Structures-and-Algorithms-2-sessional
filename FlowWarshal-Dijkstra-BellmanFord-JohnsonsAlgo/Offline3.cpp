#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
using namespace std;

///The program will run in c++11 or later versions

#define vi vector<int>
#define vii vector<pair<int, int> >
const int maxn = 200000, inf = 1000000000;

bool isLess(double x) {
    if (abs(x - inf) > 1e-12)
        return true;
    return false;
}

int Round(double val) {
    return (int)(val + (val > 0 ? 0.05 : -0.05));
}

class Data {
public:
    int vertex;
    double distance;
};

class PriorityQueue {
    int qsize;
    vector<Data> pq;
    vector<int> vertexinpq;
public:
    PriorityQueue() {
        qsize = 0;
    }
    PriorityQueue(int n) {
        qsize = n;
        pq.resize(n);
        vertexinpq.resize(n);
        for (int i = 0; i < qsize; i++) {
            vertexinpq[i] = i;
            pq[i].vertex = i, pq[i].distance = inf;
        }
    }

    bool isEmpty() {
        return (qsize == 0);
    }

    void Swap(int i, int j) {
        int v_i = pq[i].vertex, v_j = pq[j].vertex;
        vertexinpq[v_i] = j;
        vertexinpq[v_j] = i;
        swap(pq[i], pq[j]);
    }

    void minHeapify(int i) {
        int left = 2 * i, right = 2 * i + 1, smallest = i;
        if (left < qsize and pq[smallest].distance > pq[left].distance)
            smallest = left;
        if (right < qsize and pq[smallest].distance > pq[right].distance) {
            smallest = right;
        }

        if (smallest != i) {
            Swap(smallest, i);
            minHeapify(smallest);
        }
    }

    void heapify() {
        for (int i = qsize / 2 - 1; i >= 0; i--) {
            minHeapify(i);
        }
    }

    Data extractMin() {
        Data temp;
        temp = pq[0];
        pq[0] = pq[qsize - 1];
        vertexinpq[pq[0].vertex] = 0;
        qsize--;
        pq.pop_back();
        vertexinpq[temp.vertex] = -1;
        minHeapify(0);
        return temp;
    }

    Data getMin() {
        return pq[0];
    }

    int vertexDist(int vtx) {
        if (vertexinpq[vtx] == -1) {
            cout << "the vertex does not exist" << endl;
            return -1;
        }
        return pq[vertexinpq[vtx]].distance;
    }

    void decreaseVal(int vtx, double val) {
        int i = vertexinpq[vtx];
        if (pq[i].distance <= val) {
            //cout << "New value is greater, can't be decreased" << endl;
            return;
        }
        pq[i].distance = val;
        while (i > 0 and pq[i / 2].distance > pq[i].distance) {
            Swap(i, i / 2);
            i /= 2;
        }
    }
};

class Edge {
    int u, v;
    double weight;
public:
    Edge(int u, int v, double w) {
        this->u = u;
        this->v = v;
        this->weight = w;
    }
    int getU() {
        return  u;
    }
    int getV() {
        return v;
    }
    double getW() {
        return weight;
    }
    void setW(double weight) {
        this->weight = weight;
    }
};

class Graph {
    vector<vector<Edge*> > adjLst;
    vector<double> dist;
    double **distanceMatrix;
    int **parentMatrix;
    int nVertices;
    ///for adjacency list it would be better to keep (v, w) in the vector
public:
    void setnVertices(int n) {
        nVertices = n;
        adjLst.resize(nVertices + 1, vector<Edge*>()); ///vertices starts from 1
        distanceMatrix = new double*[nVertices + 1];
        for (int i = 1; i <= nVertices; i++) {
            distanceMatrix[i] = new double[nVertices + 1];
            for (int j = 1; j <= nVertices; j++)
                distanceMatrix[i][j] = inf;
        }
        parentMatrix = new int*[nVertices + 1];
        for (int i = 1; i <= nVertices; i++) {
            parentMatrix[i] = new int[nVertices + 1];
            for (int j = 1; j <= nVertices; j++)
                parentMatrix[i][j] = j;
        }
    }
    bool addEdge(int u, int v, double w) {
        if (isEdge(u, v))
            return false;
        adjLst[u].emplace_back(new Edge(u, v, w));
        return true;
    }
    void printGraph() {
        cout << "Graph:" << endl;
        for (int i = 1; i <= nVertices; i++) {
            cout << i << " : ";
            for (int j = 0; j < adjLst[i].size(); j++) {
                cout << adjLst[i][j]->getV() << "(" << Round(adjLst[i][j]->getW()) << ")";
                if (j != (int)adjLst[i].size() - 1) {
                    cout << " --> ";
                }
            }
            cout << endl;
        }
    }

    void removeEdge(int u, int v) {
        int idx = -1;
        for (int i = 0; i < adjLst[u].size(); i++) {
            if (adjLst[u][i]->getV() == v) {
                idx = i;
                break;
            }
        }

        if (idx != -1) {
             if (adjLst[u].size() > 1){
                adjLst[u][idx] = adjLst[u].back();
            }
            adjLst[u].pop_back();
        }
    }
    Edge* searchEdge(int u, int v) {
        for (int i = 0; i < adjLst[u].size(); i++) {
            if (adjLst[u][i]->getV() == v) {
                return adjLst[u][i];
            }
        }
        return NULL;
    }

    void reweightEdge(int u, int v, int w) {
        for (int i = 0; i < adjLst[u].size(); i++) {
            if (adjLst[u][i]->getV() == v) {
                adjLst[u][i]->setW(w);
                return;
            }
        }
        adjLst[u].emplace_back(new Edge(u, v, w));
    }

    bool isEdge(int u, int v) {
        for (int i = 0; i < adjLst[u].size(); i++) {
            if (adjLst[u][i]->getV() == v) {
                return true;
            }
        }
        return false;
    }

    double getWeight(int u, int v) {
        for (int i = 0; i < adjLst[u].size(); i++) {
            if (adjLst[u][i]->getV() == v) {
                return adjLst[u][i]->getW();
            }
        }
        return inf;
    }
    ~Graph() {
        adjLst.clear();
        for (int i = 1; i <= nVertices; i++) {
            if (distanceMatrix[i])
                delete distanceMatrix[i];
            if (parentMatrix[i])
                delete parentMatrix[i];
        }
        delete(distanceMatrix);
        delete parentMatrix;
    }

    void floydWarshal();
    bool bellmanFord();
    void dijkstra(int);
    void johnsonsAlgo();
    double getShortestPathWeight(int, int);
    void printShortestPath(int u, int v);
    void printDistanceMatrix();
    void printPredecessorMatrix();
    void clearSPInfo();
};

void Graph::floydWarshal() {
    for (int i = 1; i <= nVertices; i++) {
        for (int j = 0; j < adjLst[i].size(); j++) {
            distanceMatrix[i][adjLst[i][j]->getV()] = adjLst[i][j]->getW();
        }
        distanceMatrix[i][i] = 0;
    }

    for (int i = 1; i <= nVertices; i++) {
        for (int j = 1; j <= nVertices; j++)
            parentMatrix[i][j] = i;
    }

    for (int k = 1; k <= nVertices; k++) {
        for (int i = 1; i <= nVertices; i++) {
            for (int j = 1; j <= nVertices; j++) {
                if (distanceMatrix[i][j] > distanceMatrix[i][k] + distanceMatrix[k][j]) {
                    distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
                    parentMatrix[i][j] = parentMatrix[k][j];
                }
            }
        }
    }
}

bool Graph::bellmanFord() {
    dist.assign(nVertices + 1, inf);
    dist[0] = 0; ///s = 0;
    adjLst[0].clear();

    for (int i = 1; i <= nVertices; i++) {
        adjLst[0].emplace_back(new Edge(0, i, 0));
    }

    for (int k = 0; k < nVertices; k++) {
        for (int i = 0; i <= nVertices; i++) {
            for (int j = 0; j < adjLst[i].size(); j++) {
                if (dist[adjLst[i][j]->getV()] > dist[i] + adjLst[i][j]->getW() and isLess(dist[i])) {
                    dist[adjLst[i][j]->getV()] = dist[i] + adjLst[i][j]->getW();
                }
            }
        }
    }

    for (int i = 0; i <= nVertices; i++) {
        for (int j = 0; j < adjLst[i].size(); j++) {
            if (dist[adjLst[i][j]->getV()] > dist[i] + adjLst[i][j]->getW() and isLess(dist[i])) {
                return true;
            }
        }
    }
    return false;
}

void Graph::dijkstra(int n) {
    distanceMatrix[n][n] = 0;
    PriorityQueue pq(nVertices);
    pq.decreaseVal(n - 1, 0);

    while (!pq.isEmpty()) {
        Data u = pq.extractMin();
        int v = u.vertex + 1;
        double dis = u.distance;
        for (int i = 0; i < adjLst[v].size(); i++) {
            if (dis + adjLst[v][i]->getW() < distanceMatrix[n][adjLst[v][i]->getV()] and isLess(dis)) {
                distanceMatrix[n][adjLst[v][i]->getV()] = dis + adjLst[v][i]->getW();
                pq.decreaseVal(adjLst[v][i]->getV() - 1, distanceMatrix[n][adjLst[v][i]->getV()]);
                parentMatrix[n][adjLst[v][i]->getV()] = v;
            }
        }
    }
}

void Graph::johnsonsAlgo() {
    for (int i = 1; i <= nVertices; i++) {
        for (int j = 1; j <= nVertices; j++)
            distanceMatrix[i][j] = inf, parentMatrix[i][j] = i;
    }
    if (bellmanFord()) {
        cout << "There is a negative-weight cycle." << endl;
        return;
    }
    for (int i = 1; i <= nVertices; i++) {
        for (int j = 0; j < adjLst[i].size(); j++) {
            adjLst[i][j]->setW(adjLst[i][j]-> getW() + dist[i] - dist[adjLst[i][j]->getV()]);
        }
    }
    for (int i = 1; i <= nVertices; i++) {
        dijkstra(i);
        for (int j = 1; j <= nVertices; j++) {
            distanceMatrix[i][j] = distanceMatrix[i][j] + dist[j] - dist[i];
        }
    }

    for (int i = 1; i <= nVertices; i++) {
        for (int j = 0; j < adjLst[i].size(); j++) {
            adjLst[i][j]->setW(adjLst[i][j]-> getW() - dist[i] + dist[adjLst[i][j]->getV()]);
        }
    }
}

double Graph::getShortestPathWeight(int u, int v) {
    return distanceMatrix[u][v];
}

void Graph::printShortestPath(int u, int v) {
    cout << "Shortest Path Weight: " << Round(distanceMatrix[u][v]) << endl;
    cout << "Path: ";
    if (!isLess(distanceMatrix[u][v]))
        return;
    vi path;
    path.emplace_back(v);
    do {
        v = parentMatrix[u][v];
        path.emplace_back(v);
    } while (u != v);
    reverse(path.begin(), path.end());
    cout << path[0];

    for (int i = 1; i < path.size(); i++) {
        cout << " --> " << path[i] << "(" << Round(getWeight(path[i - 1], path[i])) << ")";
    }
    cout << endl;
}

void Graph::printDistanceMatrix() {
    cout << "Distance Matrix:" << endl;
    for (int i = 1; i <= nVertices; i++) {
        for (int j = 1; j <= nVertices; j++) {
            if (!isLess(distanceMatrix[i][j])) {
                cout << "INF ";
            }
            else cout << Round(distanceMatrix[i][j]) << " ";
        }
        cout << endl;
    }
}

void Graph::printPredecessorMatrix() {
    cout << "Predecessor Matrix:" << endl;
    for (int i = 1; i <= nVertices; i++) {
        for (int j = 1; j <= nVertices; j++) {
            if (!isLess(distanceMatrix[i][j]) or i == j)
                cout << "NIL ";
            else cout << parentMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::clearSPInfo() {
    for (int i = 1; i <= nVertices; i++) {
        for (int j = 1; j <= nVertices; j++) {
            distanceMatrix[i][j] = inf;
            parentMatrix[i][j] = -1;
        }
    }
}

int main() {

    freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);

    int n, m;
    cin >> n >> m;
    Graph g;
    g.setnVertices(n);
    while (m--) {
        int u, v;
        double w;
        cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }
    cout << "Graph Created." << endl;

    int c;
    while (cin >> c and c >= 1 and c <= 7) {
        if (c == 1) {
            g.clearSPInfo();
            cout << "APSP matrices cleared" << endl;
        }
        else if (c == 2) {
            g.floydWarshal();
            cout << "Floyd-Warshall ALgorithm implemented" << endl;
        }
        else if (c == 3) {
            g.johnsonsAlgo();
            cout << "Johnson's algorithm implemented" << endl;
        }
        else if (c == 4) {
            int u, v;
            cin >> u >> v;
            g.printShortestPath(u, v);
        }
        else if (c == 5) {
            g.printGraph();
        }
        else if (c == 6) {
            g.printDistanceMatrix();
        }
        else g.printPredecessorMatrix();
    }

    return 0;
}
