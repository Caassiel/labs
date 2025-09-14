#include <iostream>

using namespace std;

class Set{
private:
    static const int t = 1000;
    static const int b = 64;
    unsigned int* A = new unsigned int[t];
public:

Set(){
for(int i = 0; i < t; i++) A[i] = 0;
}

void Search(int N){
    int j = N & (b - 1);
    int i = N &  ~(b - 1);

    if (A[i] & (1 << j)) cout << "element number " << N << " is in the set.\n";
    else cout << "element number " << N << " is not in the set.\n";
}

void Insert(int N){
    int j = N & (b - 1);
    int i = N &  ~(b - 1);

    A[i] |= (1 << j);
}

void Delete(int N){
    int j = N & (b - 1);
    int i = N &  ~(b - 1);

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

Set X;
X.Insert(73);
X.Insert(40);

Set Y;
Y.Insert(40);

Set K = X.Union(Y);

K.Search(73);
K.Search(40);
K.Search(13);

cout << "\n";
Set L = X.SetDifference(Y);

L.Search(73);
L.Search(40);
L.Search(13);

cout << "\n";
Set M = Y.SetDifference(X);
M.Search(73);
M.Search(40);
M.Search(13);

cout << "\n";
Set N = X.Intersection(Y);

N.Search(73);
N.Search(40);
N.Search(13);

cout << "\n";
Set O = X.SymDifference(Y);

O.Search(73);
O.Search(40);
O.Search(13);

cout << "\n";

X.IsSubsetOf(Y);
Y.IsSubsetOf(X);

cout << "\n";

Y.Clear();
Y.Search(40);

return 0;
}
