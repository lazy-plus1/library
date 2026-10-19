template <class X, class MonoidA, class Action>
struct DualSegmentTree {
    using A = MonoidA;
    using typeA = typename A::vtype;
    using typeX = typename X::vtype;

    vector<typeX> data;
    vector<typeA> lazy;
    int n, lg, size;

    DualSegmentTree(vector<typeX> v) : n(v.size()) {
        assert(n > 0);
        lg = 0;
        while ((1 << lg) < n) lg++;
        size = 1 << lg;
        data.assign(size, X::unit);
        lazy.assign(size, A::unit);
        for (int i = 0; i < n; i++) data[i] = v[i];
    }
    DualSegmentTree(int n) : DualSegmentTree(vector<typeX>(n, X::unit)) {}

    void _push(int i, typeA a) {
        if (i < size) {
            lazy[i] = A::op(lazy[i], a);
        } else {
            i -= size;
            data[i] = Action::apply(data[i], a);
        }
    }

    void _propagate(int i) {
        _push(i << 1, lazy[i]);
        _push(i << 1 | 1, lazy[i]);
        lazy[i] = A::unit;
    }

    void _propagate_to(int i) {
        i += size;
        for (int h = lg; h > 0; h--) _propagate(i >> h);
    }

    typeX get(int i) {
        assert(0 <= i && i < n);
        _propagate_to(i);
        return data[i];
    }

    void set(int i, typeX x) {
        assert(0 <= i && i < n);
        _propagate_to(i);
        data[i] = x;
    }

    void apply(int L, int R, typeA a) {
        assert(0 <= L && R <= n && L <= R);
        if (L == R) return;
        _propagate_to(L);
        _propagate_to(R - 1);
        L += size, R += size;
        while (L < R) {
            if (L & 1) _push(L++, a);
            if (R & 1) _push(--R, a);
            L >>= 1, R >>= 1;
        }
    }
};

struct X {
    using vtype = int;
    static constexpr vtype unit = -(1 << 30);
};

struct MonoidA {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        return y;
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
