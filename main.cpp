#include <iostream>
#include <random>
#include <queue>
#include <chrono>
using namespace std;
using namespace chrono;

class MergeSort{
public:
    int n;
    int *A;

    long long Comparisons;
    long long Swaps;


    MergeSort(int N) : n(N){
        A = new int[n];
    }

    void FillArraySorted(){

    for (int i = 0; i < n; i++) A[i] = i;
    }

    void FillArrayInverseSorted(){
    for (int i = 0; i < n; i++) A[i] = n - i;
    }

    void FillArrayRandom(int low, int high){
        static mt19937 rng(random_device{}());
        uniform_int_distribution<int> dist(low, high);

        for (int i = 0; i < n; i++) {
             A[i] = dist(rng);
        }
    }

    void FillArraySparce(){
    for (int i = 0; i < floor(n/3); i++) A[i] = 1;
    for (int i = floor(n/3); i < floor(2*n/3); i++) A[i] = n;
    for (int i = floor(2*n/3); i < n; i++) A[i] = floor(n/2);
    }

    void TopDownMerge(int arr[], int temp[], int left, int mid, int right) {
        int i = left, j = mid + 1, k = left;

        while (i <= mid && j <= right) {
            Comparisons++;
            if (arr[i] <= arr[j]) temp[k++] = arr[i++];
            else                  temp[k++] = arr[j++];
            Swaps++;
        }

        while (i <= mid) {
            temp[k++] = arr[i++];
            Swaps++;
        }

        while (j <= right) {
            temp[k++] = arr[j++];
            Swaps++;
        }

        for (int t = left; t <= right; t++) {
            arr[t] = temp[t];
            Swaps++;
        }
    }

    void TopDownSort(int arr[], int temp[], int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            TopDownSort(arr, temp, left, mid);
            TopDownSort(arr, temp, mid + 1, right);
            TopDownMerge(arr, temp, left, mid, right);
        }
    }

     void BottomUpMerge(int arr[], int l, int m, int r, int temp[]) {
        int i = l, j = m + 1, k = l;

        while (i <= m && j <= r) {
            Comparisons++;
            if (arr[i] <= arr[j]) temp[k++] = arr[i++];
            else                 temp[k++] = arr[j++];
            Swaps++;
        }

        while (i <= m) {
            temp[k++] = arr[i++];
            Swaps++;
        }

        while (j <= r) {
            temp[k++] = arr[j++];
            Swaps++;
        }

        for (int t = l; t <= r; t++) {
            arr[t] = temp[t];
            Swaps++;
        }
    }

    void BottomUpSort() {
        int *temp = new int[n];

        for (int width = 1; width < n; width *= 2) {
            for (int i = 0; i < n; i += 2 * width) {

                int left  = i;
                int mid   = min(i + width - 1, n - 1);
                int right = min(i + 2 * width - 1, n - 1);

                if (mid < right) BottomUpMerge(A, left, mid, right, temp);
            }
        }

        delete[] temp;
    }

    void InsertionSort(int arr[], int left, int right, long long &Comps, long long &Swaps) {
        for (int i = left + 1; i <= right; i++) {
            int key = arr[i];
            int j = i - 1;

            while (j >= left && (++Comps, arr[j] > key)) {
                arr[j + 1] = arr[j];
                Swaps++;
                j--;
            }
            arr[j + 1] = key;
            Swaps++;
        }
    }

    void mergeBU(int src[], int dst[], int left, int mid, int right, long long &Comps, long long &Swaps)
    {
        int i = left;
        int j = mid + 1;

        for (int k = left; k <= right; k++) {
            if      (i > mid)                   dst[k] = src[j++], Swaps++;
            else if (j > right)                 dst[k] = src[i++], Swaps++;
            else if (++Comps, src[j] < src[i])  dst[k] = src[j++], Swaps++;
            else                                dst[k] = src[i++], Swaps++;
        }
    }

    void OptimizedBottomUp(int A[], int n, long long &Comps, long long &Swaps)
    {
        const int CUTOFF = 10;
        int *temp = new int[n];

        for (int i = 0; i < n; i += CUTOFF) {
            int right = min(i + CUTOFF - 1, n - 1);
            InsertionSort(A, i, right, Comps, Swaps);
        }

        int *src = A;
        int *dst = temp;

        for (int width = CUTOFF; width < n; width *= 2) {

            for (int left = 0; left < n; left += 2 * width) {
                int mid   = min(left + width - 1, n - 1);
                int right = min(left + 2 * width - 1, n - 1);

                if (mid >= right) continue;

                Comps++;
                if (src[mid] <= src[mid + 1]) {
                    for (int k = left; k <= right; k++) {
                        dst[k] = src[k];
                        Swaps++;
                    }
                    continue;
                }

                mergeBU(src, dst, left, mid, right, Comps, Swaps);
            }

            swap(src, dst);
        }


        if (src != A) {
            for (int i = 0; i < n; i++) {
                A[i] = src[i];
                Swaps++;
            }
        }

        delete[] temp;
    }

    struct Item {
        int value;
        int block;
    };

    struct Cmp {
        bool operator()(const Item& a, const Item& b) const {
            return a.value > b.value;
        }
    };

    void Merge10(int* A, int* Aux, int start, int Size, int n)
    {
        int blockStart[10];
        int blockEnd[10];

        for (int i = 0; i < 10; i++) {
            blockStart[i] = start + i * Size;
            blockEnd[i]   = min(start + (i+1) * Size, n);
            Swaps += 3;
        }

        priority_queue<Item, vector<Item>, Cmp> heap;

        int ptr[10];
        for (int i = 0; i < 10; i++) {
            Comparisons++;
            ptr[i] = blockStart[i];
            if (ptr[i] < blockEnd[i]) {
                heap.push({A[ptr[i]], i});
                ptr[i]++;
                Swaps += 3;
            }
        }

        int out = start; Swaps++;
        while (!heap.empty()) {
            Item x = heap.top(); heap.pop();
            Aux[out++] = x.value;
            Swaps += 3;

            int b = x.block;
            Comparisons++;
            if (ptr[b] < blockEnd[b]) {
                heap.push({A[ptr[b]], b});
                ptr[b]++;
                Swaps += 3;
            }
        }

        for (int i = start; i < out; i++){
           Comparisons++;
           A[i] = Aux[i];
           Swaps++;
        }
    }

    void BottomUp10(int* A, int n) {
        vector<int> Aux(n);
        Swaps++;
        for (int Size = 1; Size < n; Size *= 10) {
            Comparisons++;
            for (int i = 0; i < n; i += 10 * Size) {
                Comparisons++;
                Merge10(A, Aux.data(), i, Size, n);
            }
        }
    }

    long long TopDownSortTime() {
        Comparisons = Swaps = 0;
        int *temp = new int[n];

        auto t1 = high_resolution_clock::now();
        TopDownSort(A, temp, 0, n - 1);
        auto t2 = high_resolution_clock::now();

        delete[] temp;
        return duration_cast<microseconds>(t2 - t1).count();
    }

    long long BottomUpSortTime() {
        Comparisons = Swaps = 0;
        auto t1 = high_resolution_clock::now();

        BottomUpSort();

        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2 - t1).count();
    }

    long long OptimizedBottomUpSortTime() {
        Comparisons = Swaps = 0;
        auto t1 = high_resolution_clock::now();

        OptimizedBottomUp(A, n, Comparisons, Swaps);

        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2 - t1).count();
    }

    long long BottomUp10Time() {
        Comparisons = Swaps = 0;
        auto t1 = high_resolution_clock::now();

        BottomUp10(A, n);

        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2 - t1).count();
    }

    void Print(){
        for (int i = 0; i < n; i++) cout << A[i] << " ";
        cout << "\n__________________________________\n";
    }

    long long MemoryBytes() const {
        return sizeof(*this) + n * sizeof(int);
    }

    ~MergeSort(){
        delete[] A;
    }

};

enum InputType {RANDOM, SORTED, INVERSE, SPARCE};

void FillInput(MergeSort &M, InputType t)
{
    switch(t)
    {
        case RANDOM:
            M.FillArrayRandom(1, M.n);
            break;
        case SORTED:
            M.FillArraySorted();
            break;
        case INVERSE:
            M.FillArrayInverseSorted();
            break;
        case SPARCE:
            M.FillArraySparce();
            break;
    }
}

string InputName(InputType t)
{
    switch(t)
    {
        case RANDOM:  return "Random";
        case SORTED:  return "Sorted";
        case INVERSE: return "Inverse-sorted";
        case SPARCE:  return "Sparce";
    }
    return "";
}

void RunExperiment(const string& name, MergeSort& MS, long long(MergeSort::*Run)())
{
    cout << "__________________________________\n";
    cout << name << "\n";
    long long time = (MS.*Run)();
    cout << "Time:           "         << time << " microseconds\n";
    cout << "Comparisons:    "  << MS.Comparisons << "\n";
    cout << "Swaps:          "        << MS.Swaps << "\n";
    cout << "Memory:         "       << MS.MemoryBytes() << " bytes\n";
    cout << "__________________________________\n\n";
}

int main()
{
    int Sizes[] = {1024, 16384, 131072, 1048576};
    InputType Inputs[] = {RANDOM, SORTED, INVERSE, SPARCE};

    bool RUN_TOPDOWN      = true;
    bool RUN_BOTTOMUP     = true;
    bool RUN_OPT_BOTTOMUP = true;
    bool RUN_BOTTOMUP10   = true;


    for (int n : Sizes)
    {
        cout << "==================================\n";
        cout << "n = " << n << "\n";
        cout << "==================================\n\n";

        for (InputType T : Inputs)
        {
            cout << InputName(T) << "\n\n";

            MergeSort TD(n), BU(n), OPT(n), BU10(n);

            FillInput(TD,  T);
            FillInput(BU,  T);
            FillInput(OPT, T);
            FillInput(BU10,T);

            if (RUN_TOPDOWN) RunExperiment("Top-Down MergeSort", TD, &MergeSort::TopDownSortTime);
            if (RUN_BOTTOMUP) RunExperiment("Bottom-Up MergeSort", BU, &MergeSort::BottomUpSortTime);
            if (RUN_OPT_BOTTOMUP) RunExperiment("Optimized Bottom-Up MergeSort", OPT, &MergeSort::OptimizedBottomUpSortTime);
            if (RUN_BOTTOMUP10) RunExperiment("10-Way Bottom-Up MergeSort", BU10, &MergeSort::BottomUp10Time);

            cout << "\n";
        }
        cout << "\n\n";
    }

return 0;
}
