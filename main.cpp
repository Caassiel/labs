#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace chrono;

class QuickSort {
public:
    int n;
    int *A;
    long long Comparisons = 0;
    long long Swaps = 0;

    QuickSort(int N) : n(N) {
        A = new int[n];
    }

    ~QuickSort() { delete[] A; }

    void FillArraySorted() {
        for (int i = 0; i < n; i++) A[i] = i;
    }

    void FillArrayInverseSorted() {
        for (int i = 0; i < n; i++) A[i] = n - i;
    }

    void FillArrayConst() {
        for (int i = 0; i < n; i++) A[i] = n;
    }

    void FillArrayAlmostSorted() {
        for (int i = 1; i < n; i++) A[i] = i - 1;
        A[0] = n;
    }

    void FillArrayTriangle() {
        for (int i = 0; i < n / 2; i++) A[i] = i;
        for (int i = n / 2; i < n; i++) A[i] = n - i;
    }

    void FillArraySparce() {
        for (int i = 0; i < n / 3; i++) A[i] = 1;
        for (int i = n / 3; i < 2 * n / 3; i++) A[i] = n;
        for (int i = 2 * n / 3; i < n; i++) A[i] = n / 2;
    }

    void FillArrayRandom(int low, int high) {
        static mt19937 rng(random_device{}());
        uniform_int_distribution<int> dist(low, high);
        for (int i = 0; i < n; i++) A[i] = dist(rng);
    }

    int ChoosePivot(int l, int r, PivotType type) {
        static mt19937 rng(random_device{}());
        if (type == LAST) return r;
        if (type == RANDOM) return l + rng() % (r - l + 1);

        int m = l + (r - l) / 2;
        int a = A[l], b = A[m], c = A[r];

        if ((a <= b && b <= c) || (c <= b && b <= a)) return m;
        if ((b <= a && a <= c) || (c <= a && a <= b)) return l;
        return r;
    }

    int PartitionLomuto(int l, int r, int p) {
        swap(A[p], A[r]); Swaps++;
        int pivot = A[r], i = l - 1;
        for (int j = l; j < r; j++) {
            Comparisons++;
            if (A[j] <= pivot) {
                swap(A[++i], A[j]); Swaps++;
            }
        }
        swap(A[i + 1], A[r]); Swaps++;
        return i + 1;
    }

    int PartitionHoare(int l, int r, int p) {
        swap(A[l], A[p]); Swaps++;
        int pivot = A[l];
        int i = l - 1, j = r + 1;
        while (true) {
            do { i++; Comparisons++; } while (A[i] < pivot);
            do { j--; Comparisons++; } while (A[j] > pivot);
            if (i >= j) return j;
            swap(A[i], A[j]); Swaps++;
        }
    }

    void QuickSortRec(int l, int r, PivotType pv, PartitionType pt) {
        if (l >= r) return;
        int p = ChoosePivot(l, r, pv);
        if (pt == LOMUTO) {
            int q = PartitionLomuto(l, r, p);
            QuickSortRec(l, q - 1, pv, pt);
            QuickSortRec(q + 1, r, pv, pt);
        } else {
            int q = PartitionHoare(l, r, p);
            QuickSortRec(l, q, pv, pt);
            QuickSortRec(q + 1, r, pv, pt);
        }
    }

    long long RunQuickSort(PivotType pv, PartitionType pt) {
        Comparisons = Swaps = 0;
        auto t1 = high_resolution_clock::now();
        QuickSortRec(0, n - 1, pv, pt);
        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2 - t1).count();
    }

    void QuickSort3Way(int l, int r) {
        if (l >= r) return;
        int pivot = A[l];
        int lt = l, i = l + 1, gt = r;
        while (i <= gt) {
            Comparisons++;
            if (A[i] < pivot) swap(A[lt++], A[i++]), Swaps++;
            else if (A[i] > pivot) swap(A[i], A[gt--]), Swaps++;
            else i++;
        }
        QuickSort3Way(l, lt - 1);
        QuickSort3Way(gt + 1, r);
    }

    long long Run3Way() {
        Comparisons = Swaps = 0;
        auto t1 = high_resolution_clock::now();
        QuickSort3Way(0, n - 1);
        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2 - t1).count();
    }

    void DualPivotQS(int l, int r) {
        if (l >= r) return;
        if (A[l] > A[r]) swap(A[l], A[r]), Swaps++;

        int p = A[l], q = A[r];
        int i = l + 1, lt = l + 1, gt = r - 1;

        while (i <= gt) {
            Comparisons++;
            if (A[i] < p) swap(A[i++], A[lt++]), Swaps++;
            else if (A[i] > q) swap(A[i], A[gt--]), Swaps++;
            else i++;
        }

        swap(A[l], A[--lt]); Swaps++;
        swap(A[r], A[++gt]); Swaps++;

        DualPivotQS(l, lt - 1);
        DualPivotQS(lt + 1, gt - 1);
        DualPivotQS(gt + 1, r);
    }

    long long RunDualPivot() {
        Comparisons = Swaps = 0;
        auto t1 = high_resolution_clock::now();
        DualPivotQS(0, n - 1);
        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2 - t1).count();
    }
};

enum PivotType {
    LAST,
    RANDOM,
    MEDIAN_FIXED,
    MEDIAN_RANDOM
};

enum PartitionType {
    LOMUTO,
    HOARE
};

enum class ArrayType {
    SORTED,
    INVERSE,
    RANDOM,
    CONST,
    ALMOST_SORTED,
    TRIANGLE,
    SPARCE
};

enum class QSVariant {
    LOMUTO,
    HOARE,
    THREE_WAY,
    DUAL_PIVOT
};

struct Variant {
    QSVariant type;
    PivotType pivot;
    PartitionType part;
    const char* name;
};

void FillArray(QuickSort &Q, ArrayType t) {
    if (t == ArrayType::SORTED) Q.FillArraySorted();
    if (t == ArrayType::INVERSE) Q.FillArrayInverseSorted();
    if (t == ArrayType::CONST) Q.FillArrayConst();
    if (t == ArrayType::ALMOST_SORTED) Q.FillArrayAlmostSorted();
    if (t == ArrayType::TRIANGLE) Q.FillArrayTriangle();
    if (t == ArrayType::SPARCE) Q.FillArraySparce();
    if (t == ArrayType::RANDOM) Q.FillArrayRandom(1, Q.n);
}

int main() {

    int sizes[] = {16384};

    ArrayType arrays[] = {
        ArrayType::SORTED,
        ArrayType::INVERSE,
        ArrayType::RANDOM,
        ArrayType::CONST,
        ArrayType::ALMOST_SORTED,
        ArrayType::TRIANGLE,
        ArrayType::SPARCE,
    };

    Variant variants[] = {
        {QSVariant::LOMUTO, LAST, LOMUTO, "Lomuto + Last       "},
        {QSVariant::LOMUTO, RANDOM, LOMUTO, "Lomuto + Random     "},
        {QSVariant::LOMUTO, MEDIAN_FIXED, LOMUTO, "Lomuto + Median3    "},
        {QSVariant::LOMUTO, MEDIAN_RANDOM, LOMUTO, "Lomuto + MedianRandom     "},
        {QSVariant::THREE_WAY, MEDIAN_RANDOM, LOMUTO, "3-Way + MedianRandom"},
        {QSVariant::DUAL_PIVOT, MEDIAN_FIXED, LOMUTO, "Dual Pivot + Median3"},
        {QSVariant::HOARE, MEDIAN_FIXED, HOARE, "Hoare + Median3     "},
        {QSVariant::HOARE, RANDOM, HOARE, "Hoare + Random      "}
    };

    for (auto a : arrays) {
        cout << "\n=== Array type " << (int)(a) + 1 << " ===\n"
                     << left << setw(26) << "Algorithm"
                     << " | " << right << setw(10) << "Time (us)"
                     << " | " << setw(12) << "Comparisons"
                     << " | " << setw(12) << "Swaps"
                     << "\n";

        for (int n : sizes) {
            cout << "\n-- n = " << n << " --\n";
            for (auto &v : variants) {
                QuickSort Q(n);
                FillArray(Q, a);
                long long t = 0;

                if (v.type == QSVariant::LOMUTO || v.type == QSVariant::HOARE)
                    t = Q.RunQuickSort(v.pivot, v.part);
                else if (v.type == QSVariant::THREE_WAY)
                    t = Q.Run3Way();
                else
                    t = Q.RunDualPivot();

                cout << string(26 + 3 + 10 + 3 + 12 + 3 + 12, '-') << "\n";

                cout << left
                     << setw(26) << v.name
                     << " | "
                     << right << setw(8) << t << " us"
                     << " | " << setw(10) << Q.Comparisons
                     << " | " << setw(10) << Q.Swaps
                     << "\n";
            }
        }
    }

    return 0;
}

