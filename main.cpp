#include <iostream>
#include <list>
#include <ctime>
#include <chrono>
#include <random>
using namespace std;
using namespace chrono;

class Graph{
private:
    int V;
    list<int> *A;

public:
Graph(int v){
    V = v;
    A = new list<int>[v];
}

void AddEdge(int v, int u){
A[v].push_back(u);
}

void GenerateRandomGraph(double p){
static mt19937 rng(random_device{}());
uniform_real_distribution<double> dist(0.0, 1.0);

for (int i = 0; i < V; i++){
     for (int j = i; j < V; j++){
          if (dist(rng) < p) {
              AddEdge(i, j);
          }
     }
  }
}

void PrintGraph(){
for (int i = 0; i < V; i++){
cout << i + 1 << ": ";
    for (int neighbor : A[i]){
        cout << neighbor + 1 << " ";
    }
    cout << "\n";
  }
}


void BFS(int s){

bool *visited = new bool[V];
for (int i = 0; i < V; i++){
    visited[i] = false;
}

list<int> Queue;
visited[s] = true;
Queue.push_back(s);

while(!Queue.empty()){
    s = Queue.front();
    Queue.pop_front();

    for (auto i = A[s].begin(); i != A[s].end(); i++){
        if (!visited[*i]){
            visited[*i] = true;
            Queue.push_back(*i);
        }
    }
}

}

void DFS(int s, bool *visited){
visited[s] = true;

for (auto i = A[s].begin(); i != A[s].end(); i++){
    if(!visited[*i]){
        DFS(*i, visited);
    }
}

};

void DFSrecursive(int s){
bool *visited = new bool[V];
for (int i = 0; i < V; i++){
    visited[i] = 0;
}

DFS(s, visited);
}

};



int main()
{
srand(time(0));
auto time1 = nanoseconds::zero();
auto time2 = nanoseconds::zero();
int N = 1000;
int V = 10;
float p = 0.5;

for (int i = 0; i < N; i++){
Graph G(V);
G.GenerateRandomGraph(p);

auto Start1 = high_resolution_clock::now();
G.BFS(2);
auto End1 = high_resolution_clock::now();
auto duration1 = duration_cast<nanoseconds>(End1 - Start1);
time1 = time1 + duration1;

auto Start2 = high_resolution_clock::now();
G.DFSrecursive(2);
auto End2 = high_resolution_clock::now();
auto duration2 = duration_cast<nanoseconds>(End2 - Start2);

time2 = time2 + duration2;
}

cout << "Average time per BFS: " << time1.count()/N << " nanoseconds.\n";
cout << "Average time per DFS (recursive): " << time2.count()/N << " nanoseconds.\n";
//G.PrintGraph();

return 0;
}
