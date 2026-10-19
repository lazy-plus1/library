template <typename T>
struct BinaryIndexedTree {
    int n;
    vector<T> bit;

    BinaryIndexedTree(int _n) : n(_n), bit(_n + 1, 0) {}

    void add(int a, T w) {
        for (int x = a; x <= n; x += x & -x) bit[x] += w;
    }

    T sum(int a) {
        T ret = 0;
        for (int x = a; x > 0; x -= x & -x) ret += bit[x];
        return ret;
    }
};
