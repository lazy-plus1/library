template <typename Monoid>
struct DynamicSegmentTree {
    using M = Monoid;
    using X = typename M::vtype;

    struct Node {
        Node *l, *r;
        X v;
    };

    using F = function<X(long long, long long)>;
    F default_prod;
    const long long L0, R0;
    using np = Node *;
    np root = nullptr;

    DynamicSegmentTree(
        long long L0, long long R0,
        F default_prod = [](long long, long long) { return M::unit(); })
        : L0(L0), R0(R0), default_prod(default_prod) {}

    void set(long long pos, const X &val) {
        if (!root) root = new Node{nullptr, nullptr, default_prod(L0, R0)};
        set_rec(root, L0, R0, pos, val);
    }

    X prod(long long l, long long r) {
        X ret = M::unit();
        prod_rec(root, L0, R0, l, r, ret);
        return ret;
    }

    private:
    void set_rec(np t, long long l, long long r, long long pos, const X &val) {
        if (r - l == 1) {
            t->v = val;
            return;
        }
        long long m = (l + r) >> 1;
        if (pos < m) {
            if (!t->l) t->l = new Node{nullptr, nullptr, default_prod(l, m)};
            set_rec(t->l, l, m, pos, val);
        }
        else {
            if (!t->r) t->r = new Node{nullptr, nullptr, default_prod(m, r)};
            set_rec(t->r, m, r, pos, val);
        }
        X lv = t->l ? t->l->v : default_prod(l, m);
        X rv = t->r ? t->r->v : default_prod(m, r);
        t->v = M::op(lv, rv);
    }

    void prod_rec(np t, long long l, long long r, long long ql, long long qr, X &ret) {
        if (qr <= l || r <= ql) return;
        if (!t) {
            long long ml = max(l, ql), mr = min(r, qr);
            if (ml < mr) ret = M::op(ret, default_prod(ml, mr));
            return;
        }
        if (ql <= l && r <= qr) {
            ret = M::op(ret, t->v);
            return;
        }
        long long m = (l + r) >> 1;
        prod_rec(t->l, l, m, ql, qr, ret);
        prod_rec(t->r, m, r, ql, qr, ret);
    }
};
