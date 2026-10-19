template <class Monoid>
struct SegmentTree {
    using M = Monoid;
    using vtype = typename M::vtype;

    vector<vtype> data;
    int n, lg, size;

    SegmentTree(vector<vtype> v) : n(v.size()) {
        assert(n > 0);
        lg = 0;
        while ((1 << lg) < n) lg++;
        size = 1 << lg;
        data.assign(size << 1, M::unit);
        for (int i = 0; i < n; i++) data[i + size] = v[i];
        for (int i = size - 1; i > 0; i--) _update(i);
    }
    SegmentTree(int n) : SegmentTree(vector<vtype>(n, M::unit)) {}

    void _update(int i) {data[i] = M::op(data[i << 1], data[i << 1 | 1]);}

    vtype get(int i) const {
        assert(0 <= i && i < n);
        return data[i + size];
    }

    void set(int i, vtype x) {
        assert(0 <= i && i < n);
        i += size;
        data[i] = x;
        while (i >>= 1) _update(i);
    }

    vtype prod(int L, int R) const {
        assert(0 <= L && R <= n && L <= R);
        vtype vl = M::unit;
        vtype vr = M::unit;
        L += size, R += size;
        while (L < R) {
            if (L & 1) vl = M::op(vl, data[L++]);
            if (R & 1) vr = M::op(data[--R], vr);
            L >>= 1, R >>= 1;
        }
        return M::op(vl, vr);
    }

    template <class F>
    int max_right(int L, const F &check) const {
        assert(0 <= L && L <= n && check(M::unit));
        if (L == n) return n;
        L += size;
        vtype s = M::unit;
        do {
            while (!(L & 1)) L >>= 1;
            if (!check(M::op(s, data[L]))) {
                while (L < size) {
                    L <<= 1;
                    if (check(M::op(s, data[L]))) {
                        s = M::op(s, data[L]);
                        L++;
                    }
                }
                return L - size;
            }
            s = M::op(s, data[L]);
            L++;
        } while ((L & -L) != L);
        return n;
    }

    template <class F>
    int min_left(int R, const F &check) const {
        assert(0 <= R && R <= n && check(M::unit));
        if (R == 0) return 0;
        R += size;
        vtype s = M::unit;
        do {
            --R;
            while (R > 1 && (R & 1)) R >>= 1;
            if (!check(M::op(data[R], s))) {
                while (R < size) {
                    R = R << 1 | 1;
                    if (check(M::op(data[R], s))) {
                        s = M::op(data[R], s);
                        R--;
                    }
                }
                return R + 1 - size;
            }
            s = M::op(data[R], s);
        } while ((R & -R) != R);
        return 0;
    }
};

struct Monoid {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        return x + y;
    }
    static constexpr vtype unit = 0;
};
