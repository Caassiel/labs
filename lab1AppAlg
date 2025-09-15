#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <chrono>
#include <random>
using namespace std;
using namespace chrono;

class Set{
private:
    static const int t = 1000;
    static const int w = 6;
    static const int b = 1 << w;
    int* A = new int[t];
public:

Set(){
for(int i = 0; i < t; i++) A[i] = 0;
}

void Generate(){
static mt19937_64 rng(random_device{}());
uniform_int_distribution<uint64_t> dist(0, UINT_MAX);

    for (int i = 0; i < t; i++) {
        A[i] = dist(rng);
    }
}


void Search(int N){
    int j = N & (b - 1);
    int i = N >> w;

    if (A[i] & (1 << j));
}

void Insert(int N){
    int j = N & (b - 1);
    int i = N >> w;

    A[i] |= (1 << j);
}

void Delete(int N){
    int j = N & (b - 1);
    int i = N >> w;

    A[i] &= ~(1 << j);
}

void Clear(){
for(int i = 0; i < t; i++) A[i] = 0;
}

Set Union(const Set &x) const {
        Set X;
        for (int i = 0; i < t; i++) {
            X.A[i] = A[i] | x.A[i];
        }
        return X;
}

Set Intersection(const Set &x) const {
        Set X;
        for (int i = 0; i < t; i++) {
            X.A[i] = A[i] & x.A[i];
        }
        return X;
}

Set SetDifference(const Set &x) const {
        Set X;
        for (int i = 0; i < t; i++) {
            X.A[i] = A[i] & ~(x.A[i]);
        }
        return X;
}

Set SymDifference(const Set &x) const {
        Set X;
        for (int i = 0; i < t; i++) {
            X.A[i] = A[i] ^ x.A[i];
        }
        return X;
}

void IsSubsetOf(const Set &x) const {
         for (int i = 0; i < t; i++) {
            if (A[i] & ~(x.A[i])) {cout << "Set is not a subset of another.\n"; return;}
         }
         cout << "Set is a subset of another.\n";
}

~Set(){
delete[] A;
}

};


int main()
{
srand(time(0));

auto time1 = nanoseconds::zero();
auto time2 = nanoseconds::zero();

int N = 2000;

for (int i = 0; i < N; i++){
Set X;
X.Generate();
auto Start = high_resolution_clock::now();
X.Search(1000);
auto End = high_resolution_clock::now();
auto duration = duration_cast<nanoseconds>(End - Start);

time1 = time1 + duration;
}

for (int i = 0; i < N; i++){
Set X;
Set Y;
X.Generate();
Y.Generate();
auto Start = high_resolution_clock::now();
X.SymDifference(Y);
auto End = high_resolution_clock::now();
auto duration = duration_cast<nanoseconds>(End - Start);

time2 = time2 + duration;
}

cout << "Average time per search: " << time1.count()/N << " nanoseconds. \n";
cout << "Average time per operation: " << time2.count()/N << " nanoseconds. \n";
return 0;
}
