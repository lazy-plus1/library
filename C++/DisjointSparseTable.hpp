template <class Monoid>
struct DisjointSparseTable {
    using M = Monoid;
    using vtype = typename M::vtype;

    int n, lg;
    vector<vtype> a;
    vector<vector<vtype>> data;

    DisjointSparseTable(const vector<vtype> &v) : n((int)v.size()), a(v) {
        lg = 0;
        while ((1 << lg) < n) lg++;
        data.resize(lg);
        for (int k = 0; k < lg; k++) {
            data[k] = a;
            int w = 1 << k;
            for (int l = 0; l + w < n; l += 2 * w) {
                int m = l + w;
                int r = min(l + 2 * w, n);
                for (int i = m - 2; i >= l; i--) {
                    data[k][i] = M::op(a[i], data[k][i + 1]);
                }
                for (int i = m + 1; i < r; i++) {
                    data[k][i] = M::op(data[k][i - 1], a[i]);
                }
            }
        }
    }

    vtype get(int i) const {
        return a[i];
    }

    vtype prod(int L, int R) const {
        if (L == R) return M::unit;
        if (R == L + 1) return a[L];
        int k = 31 - __builtin_clz(L ^ (R - 1));
        return M::op(data[k][L], data[k][R - 1]);
    }
};

struct Monoid {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        return x + y;
    }
    static constexpr vtype unit = 0;
};
