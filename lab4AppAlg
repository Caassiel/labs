#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
using namespace std;
using namespace chrono;

class UnionFind{
private:
    int N;
    int *R, *List, *Next, *Size;

public:
UnionFind(int n) : N(n){
    R = new int[N];
    Next = new int[N];
    List = new int[N];
    Size = new int[N];

    for (int i = 0; i < N; i++) {
        Next[i] = -1;
        R[i] = i;
        Size[i] = 1;
        List[i] = i;
    }
}

int Find(int x){
    return R[x];
}

void Union(int x, int y){
    int A = R[x], B = R[y];
    if (A == B) return;
    if (Size[A] > Size[B]){
        swap(A, B);
    }

    int z = List[A];
    int last = -1;

    while(z != -1){
        R[z] = B;
        last = z;
        z = Next[z];
   }

    if (last != -1){
        Next[last] = List[B];
        List[B] = List[A];
    }

    Size[B] += Size[A];
    Size[A] = 0;
}

~UnionFind(){
    delete[] R;
    delete[] List;
    delete[] Next;
    delete[] Size;
    }

};

struct Edge {
    int u, v;
    float weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class WeightedUndirectedGraph{
private:
    int V;
    vector<Edge> A;
    vector<Edge> MST;

public:

WeightedUndirectedGraph(int v) : V(v) {}

void AddEdge(int v, int u, float w) {
    if (v != u) A.push_back({u, v, w});
}

void GenerateRandomGraph(double p, int low, int high) {
    static mt19937 rng(random_device{}());
    uniform_real_distribution<double> prob(0.0, 1.0);
    uniform_real_distribution<float> weight(low, high);

    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (prob(rng) < p) {
                float w = round(weight(rng) * 100.0f) / 100.0f;
                AddEdge(i, j, w);
            }
        }
    }
}

void PrintGraph() const {
    cout << "Graph: \n";
    for (auto& e : A) cout << "(" << e.u + 1 << ", " << e.v + 1 << ", " << e.weight << ") ";
    cout << "\n\n";
}

void Kruskal(const vector<Edge> &B) {
    UnionFind uf(V);
    for (const auto &e : B) {
        if (uf.Find(e.v) != uf.Find(e.u)) {
            uf.Union(e.u, e.v);
            MST.push_back(e);
        }
        if ((int)MST.size() == V - 1) break;
    }
}

void PrintMST() const {
    float total = 0;
    cout << "Minimum spanning tree: \n";
    for (auto& e : MST) {
        cout << e.u + 1 << " - " << e.v + 1 << " : " << e.weight << "\n";
            total += e.weight;
    }
}

vector<Edge> GetEdges() const {return A;}

};

int main()
{
auto time = nanoseconds::zero();

int V = 100;
float p = 0.01;
int M = 100;

for (int i = 0; i < M; i++){
    WeightedUndirectedGraph G(V);
    G.GenerateRandomGraph(p, 1, 1000);

    vector<Edge> SortedEdges = G.GetEdges();
    sort(SortedEdges.begin(), SortedEdges.end());

    auto Start = high_resolution_clock::now();
        G.Kruskal(SortedEdges);
    auto End = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(End - Start);
    time = time + duration;
}

cout << "Average time per Kruskal: " << time.count()/M << " nanoseconds.\n";

return 0;
}
