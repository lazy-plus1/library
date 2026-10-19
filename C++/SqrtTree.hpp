template <class Monoid>
struct SqrtTree {
    using M = Monoid;
    using vtype = typename M::vtype;

    int n;
    vector<vtype> a;
    vector<vector<vtype>> prefix, suffix, between;
    vector<int> sz_log;
    vector<int> bit_to_lv;

    SqrtTree(const vector<vtype> &v) : n((int)v.size()), a(v) {
        assert(n > 0);
        int K = 0;
        while ((1 << K) < n) K++;
        bit_to_lv.assign(K, -1);
        while (K > 1) {
            for (int i = K / 2; i < K; i++) {
                bit_to_lv[i] = (int)sz_log.size();
            }
            sz_log.push_back(K);
            K >>= 1;
        }
        for (int lv = 0; lv < (int)sz_log.size(); lv++) {
            _build(lv);
        }
    }

private:
    void _build(int lv) {
        int K1 = sz_log[lv];
        int K2 = K1 >> 1;
        int block_size = 1 << K2;
        int mask = block_size - 1;
        vector<vtype> pref = a;
        vector<vtype> suff = a;
        for (int i = 0; i < n; i++) if (i & mask) pref[i] = M::op(pref[i - 1], a[i]);
        for (int i = n - 1; i >= 1; i--) if (i & mask) suff[i - 1] = M::op(a[i - 1], suff[i]);
        prefix.push_back(pref);
        suffix.push_back(suff);
        int block_count = (n + block_size - 1) >> K2;
        vector<vtype> block_prod(block_count);
        for (int i = 0; i < block_count; i++) block_prod[i] = suff[i << K2];
        int child_count = 1 << (K1 - K2);
        vector<vtype> b;
        for (int j = 0; j < block_count; j++) {
            int s = (j / child_count) * child_count;
            b.push_back(block_prod[j]);
            for (int i = j - 1; i >= s; i--) b.push_back(M::op(block_prod[i], b.back()));
        }
        between.push_back(b);
    }

public:
    vtype get(int i) const {
        assert(0 <= i && i < n);
        return a[i];
    }

    vtype prod(int L, int R) const {
        assert(0 <= L && L <= R && R <= n);
        if (L == R) return M::unit;
        R--;
        if (L == R) return a[L];
        int lv = bit_to_lv[31 - __builtin_clz(L ^ R)];
        if (lv == -1) return M::op(a[L], a[R]);
        int K1 = sz_log[lv];
        int K2 = K1 >> 1;
        vtype res = suffix[lv][L];
        int l_block = (L >> K2) + 1;
        int r_block = (R >> K2) - 1;
        if (l_block <= r_block) {
            int child_count = 1 << (K1 - K2);
            int node = l_block / child_count;
            int a_block = l_block - node * child_count;
            int b_block = r_block - node * child_count;
            int idx = node * (child_count * (child_count + 1) / 2);
            idx += (b_block + 1) * b_block / 2;
            idx += b_block - a_block;
            res = M::op(res, between[lv][idx]);
        }
        res = M::op(res, prefix[lv][R]);
        return res;
    }
};

struct Monoid {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        return x + y;
    }
    static constexpr vtype unit = 0;
};
