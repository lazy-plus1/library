#include <bits/stdc++.h>
using namespace std;


template <class Monoid>
struct SparseTable {
    using M = Monoid;
    using vtype = typename M::vtype;

    int n, lg;
    vector<vector<vtype>> data;

    SparseTable(const vector<vtype> &v) : n((int)v.size()) {
        lg = 0;
        while ((1 << (lg + 1)) <= n) lg++;
        data.resize(lg + 1);
        data[0] = v;
        for (int k = 0; k < lg; k++) {
            int m = n - (1 << (k + 1)) + 1;
            data[k + 1].resize(m);
            for (int i = 0; i < m; i++) {
                data[k + 1][i] = M::op(
                    data[k][i],
                    data[k][i + (1 << k)]
                );
            }
        }
    }

    vtype prod(int L, int R) const {
        int k = 31 - __builtin_clz(R - L);
        return M::op(
            data[k][L],
            data[k][R - (1 << k)]
        );
    }
};

struct Monoid {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        return min(x, y);
    }
    static constexpr vtype unit = INT_MAX;
};
