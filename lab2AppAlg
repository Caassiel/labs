#include <iostream>
#include <vector>
#include <random>
using namespace std;

class Graph{

protected:
    vector<vector<int>> A;

public:

Graph(int n) : A(n, vector<int>(n, 0)) {};

void PrintGraph(){
int N = A.size();
for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
        cout << A[i][j] << " ";
    }
    cout << "\n";
  }
}

void AddVertex(){
int N = A.size();
A.push_back(vector<int>(N + 1, 0));
for (int i = 0; i < N; i++){
    A[i].push_back(0);
  }
}

void DeleteVertex(){
A.pop_back();
int N = A.size();
for (int i = 0; i < N + 1; i++){
    A[i].pop_back();
  }
}

void AddEdge(int i, int j){};
void DeleteEdge(int i, int j){};

Graph ConvertToList(){
int N = A.size();
Graph L(N);

for (int i = 0; i < N; i++){
     for (int j = 0; j < N; j++){
         if (A[i][j] == 1) {
            L.A[i].push_back(j);
            L.A[i][j] = j + 1;
         }
     }
}

return L;
}

Graph ConvertToMatrix(){
int N = A.size();
Graph L(N);

for (int i = 0; i < N; i++){
     for (int j = 0; j < N; j++){
         if (A[i][j] != 0) {
            L.A[i].push_back(j);
            L.A[i][j] = 1;
         }
     }
}

return L;
}

};

class UndirectedGraph : public Graph{
public:
UndirectedGraph(int n) : Graph(n) {}

void AddEdge(int i, int j){A[i][j] = 1; A[j][i] = 1;}
void DeleteEdge(int i, int j){A[i][j] = 0; A[j][i] = 0;}

void GenerateRandomGraph(double p){
static mt19937 rng(random_device{}());
uniform_real_distribution<double> dist(0.0, 1.0);
int N = A.size();

for (int i = 0; i < N; i++){
     for (int j = i; j < N; j++){
          if (dist(rng) < p) {
              AddEdge(i, j);
              AddEdge(j, i);
          }
     }
  }
}

};

class DirectedGraph : public Graph{
public:
DirectedGraph(int n) : Graph(n) {}

void AddEdge(int i, int j){A[i][j] = 1;}
void DeleteEdge(int i, int j){A[i][j] = 0;}

void GenerateRandomGraph(double p){
static mt19937 rng(random_device{}());
uniform_real_distribution<double> dist(0.0, 1.0);
int N = A.size();

for (int i = 0; i < N; i++){
     for (int j = 0; j < N; j++){
          if (dist(rng) < p) {
              AddEdge(i, j);
          }
     }
  }
}

};

class WeightedGraph : public DirectedGraph{
public:
WeightedGraph(int n) : DirectedGraph(n) {}

void AddEdge(int i, int j, double w){
A[i][j] = w;
}
void DeleteEdge(int i, int j){
A[i][j] = 0;
}

void GenerateRandomWeightedGraph(double p, float low, float high){
static mt19937 rng(random_device{}());
uniform_real_distribution<double> dist1(0.0, 1.0);
uniform_real_distribution<double> dist2(low, high);
int N = A.size();

for (int i = 0; i < N; i++){
     for (int j = 0; j < N; j++){
          if (dist1(rng) < p) {
              AddEdge(i, j, dist2(rng));
          }
     }
  }
}

};

int main()
{

DirectedGraph G(5);
G.DeleteVertex();

G.AddEdge(0 ,3);
G.AddEdge(1, 0);
G.AddEdge(1, 2);
G.AddEdge(1, 3);
G.AddEdge(2, 0);
G.AddEdge(2, 3);

G.PrintGraph();

cout << "\n";

Graph L = G.ConvertToList();
L.PrintGraph();

cout << "\n";

Graph K = L.ConvertToMatrix();
K.PrintGraph();

cout << "\n";

DirectedGraph B(10);
B.GenerateRandomGraph(0.1);
B.PrintGraph();

WeightedGraph C(10);

cout << "\n";

C.GenerateRandomWeightedGraph(0.1, 1, 10);
C.PrintGraph();

return 0;
}
