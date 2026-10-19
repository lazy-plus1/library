template <class MonoidX, class MonoidA, class Action>
struct LazySegmentTree {
    using X = MonoidX;
    using A = MonoidA;
    using typeX = typename X::vtype;
    using typeA = typename A::vtype;

    vector<typeX> data;
    vector<typeA> lazy;
    int n, lg, size;

    LazySegmentTree(vector<typeX> v) : n(v.size()) {
        assert(n > 0);
        lg = 0;
        while ((1 << lg) < n) lg++;
        size = 1 << lg;
        data.assign(size << 1, X::unit);
        lazy.assign(size << 1, A::unit);
        for (int i = 0; i < n; i++) data[i + size] = v[i];
        for (int i = size - 1; i > 0; i--) data[i] = X::op(data[i << 1], data[i << 1 | 1]);
    }
    LazySegmentTree(int n) : LazySegmentTree(vector<typeX>(n, X::unit)) {}

    typeX _eval(int i) { return Action::apply(data[i], lazy[i]); }

    void _propagate(int i) {
        data[i] = _eval(i);
        if (i < size) {
            lazy[i << 1] = A::op(lazy[i << 1], lazy[i]);
            lazy[i << 1 | 1] = A::op(lazy[i << 1 | 1], lazy[i]);
        }
        lazy[i] = A::unit;
    }

    void _propagate_to(int i) {
        for (int h = lg; h > 0; h--) _propagate(i >> h);
    }

    void _update_from(int i) {
        while (i >>= 1) {
            data[i] = X::op(_eval(i << 1), _eval(i << 1 | 1));
        }
    }

    typeX get(int i) {
        assert(0 <= i && i < n);
        i += size;
        _propagate_to(i);
        return _eval(i);
    }

    void set(int i, typeX x) {
        assert(0 <= i && i < n);
        i += size;
        _propagate_to(i);
        data[i] = x;
        lazy[i] = A::unit;
        _update_from(i);
    }

    void apply(int L, int R, typeA a) {
        assert(0 <= L && R <= n && L <= R);
        if (L == R) return;
        L += size, R += size;
        int L0 = L, R0 = R - 1;
        _propagate_to(L0);
        _propagate_to(R0);
        while (L < R) {
            if (L & 1) {
                lazy[L] = A::op(lazy[L], a);
                L++;
            }
            if (R & 1) {
                --R;
                lazy[R] = A::op(lazy[R], a);
            }
            L >>= 1, R >>= 1;
        }
        _update_from(L0);
        _update_from(R0);
    }

    typeX prod(int L, int R) {
        assert(0 <= L && R <= n && L <= R);
        if (L == R) return X::unit;
        L += size, R += size;
        _propagate_to(L);
        _propagate_to(R - 1);
        typeX vl = X::unit;
        typeX vr = X::unit;
        while (L < R) {
            if (L & 1) vl = X::op(vl, _eval(L++));
            if (R & 1) vr = X::op(_eval(--R), vr);
            L >>= 1, R >>= 1;
        }
        return X::op(vl, vr);
    }

    template <class F>
    int max_right(int L, const F &check) {
        assert(0 <= L && L <= n && check(X::unit));
        if (L == n) return n;
        L += size;
        _propagate_to(L);
        typeX s = X::unit;
        do {
            while (!(L & 1)) L >>= 1;
            if (!check(X::op(s, _eval(L)))) {
                while (L < size) {
                    _propagate(L);
                    L <<= 1;
                    if (check(X::op(s, _eval(L)))) {
                        s = X::op(s, _eval(L));
                        L++;
                    }
                }
                return L - size;
            }
            s = X::op(s, _eval(L));
            L++;
        } while ((L & -L) != L);
        return n;
    }

    template <class F>
    int min_left(int R, const F &check) {
        assert(0 <= R && R <= n && check(X::unit));
        if (R == 0) return 0;
        R += size;
        _propagate_to(R - 1);
        typeX s = X::unit;
        do {
            --R;
            while (R > 1 && (R & 1)) R >>= 1;
            if (!check(X::op(_eval(R), s))) {
                while (R < size) {
                    _propagate(R);
                    R = R << 1 | 1;
                    if (check(X::op(_eval(R), s))){
                        s = X::op(_eval(R), s);
                        R--;
                    }
                }
                return R + 1 - size;
            }
            s = X::op(_eval(R), s);
        } while ((R & -R) != R);
        return 0;
    }
};

struct MonoidX {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        if (x > y) return x;
        else return y;
    }
    static constexpr vtype unit = - (1 << 30);
};

struct MonoidA {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        return x + y;
    }
    static constexpr vtype unit = 0;
};

struct Action {
    using typeX = int;
    using typeA = int;
    static typeX apply(typeX x, typeA a) {
        return x + a;
    }
};
