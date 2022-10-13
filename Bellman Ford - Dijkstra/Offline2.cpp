#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include<bits/stdc++.h>
using namespace std;

#define vi vector<int>
#define vii vector<pair<int, int> >
const int maxn = 200000, inf = 1000000000;

class Data {
public:
    int vertex, distance;
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

    void decreaseVal(int vtx, int val) {
        int i = vertexinpq[vtx];
        if (pq[i].distance <= val) {
            cout << "New value is greater, can't be decreased" << endl;
            return;
        }
        pq[i].distance = val;
        while (i > 0 and pq[i / 2].distance > pq[i].distance) {
            Swap(i, i / 2);
            i /= 2;
        }
    }
};

vii g[maxn]; /// g -> adjacency list

pair<int, vi> dijkstra(int n, int s, int d) {
    vi dist(n, inf), parent(n, -1);
    dist[s] = 0;
    PriorityQueue pq(n);
    pq.decreaseVal(s, 0);

    while (!pq.isEmpty()) {
        Data temp = pq.extractMin();
        int v = temp.vertex, dis = temp.distance;
        if (v == d)
            break;

        for (pair<int, int> &vv : g[v]) {
            if (dis + abs(vv.second) < dist[vv.first]) {
                dist[vv.first] = dis + abs(vv.second);
                pq.decreaseVal(vv.first, dist[vv.first]);
                parent[vv.first] = v;
            }
        }
    }
    vi path;
    path.push_back(d);

    while (parent[path.back()] != -1) {
        path.emplace_back(parent[path.back()]);
    }
    reverse(path.begin(), path.end());
    if (dist[d] == inf)
        path.clear();

    return {dist[d], path};
}

pair<pair<bool, int>, vi> bellmanFord(int n, int s, int d) {
    vi dist(n, inf), parent(n, -1);
    dist[s] = 0;
    for (int i = 1; i < n; i++) {
        for (int u = 0; u < n; u++) {
            for (auto &v : g[u]) {
                if (dist[u] + v.second < dist[v.first]) {
                    dist[v.first] = dist[u] + v.second;
                    parent[v.first] = u;
                }
            }
        }
    }

    pair<pair<bool, int>, vi> ans;
    ans.first.first = false;
    for (int u = 0; u < n; u++) {
        for (auto &v : g[u]) {
            if (dist[u] + v.second < dist[v.first]) {
                ans.first.first = true;
                return ans;
            }
        }
    }

    ans.first.second = dist[d];

    ans.second.emplace_back(d);

    while (parent[ans.second.back()] != -1) {
        ans.second.emplace_back(parent[ans.second.back()]);
    }

    reverse(ans.second.begin(), ans.second.end());
    if (ans.first.second == inf) {
        ans.second.clear();
    }
    return ans;
}

int main() {

    int n, m; /// n nodes m edges
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);

    cin >> n >> m;

    while (m--) {
        int u, v, w;
        cin >> u >> v >> w; /// vertices start from 0
        g[u].emplace_back(v, w);
    }

    int s, d;
    cin >> s >> d;

    cout << "Bellman Ford Algorithm: " << endl;

    pair<pair<bool, int>, vi> ans = bellmanFord(n, s, d);

    if (ans.first.first) {
        cout << "Negative Cycle Found" << endl;
    }
    else {
        cout << ans.first.second << endl;
        for (int i = 0; i < ans.second.size(); i++) {
            cout << ans.second[i] << " ";
            if (i != ans.second.size() - 1) {
                cout << "-> ";
            }
        }
        cout << endl;
    }

    cout << endl;

    cout << "Dijkstra Algorithm: " << endl;

    pair<int, vi> ans1 = dijkstra(n, s, d);

    cout << ans1.first << endl;
    for (int i = 0; i < ans1.second.size(); i++) {
        cout << ans1.second[i] << " ";
        if (i != ans1.second.size() - 1) {
            cout << "-> ";
        }
    }
    cout << endl;

    return 0;
}
