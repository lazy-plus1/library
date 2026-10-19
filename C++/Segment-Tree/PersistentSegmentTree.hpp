template <class Monoid, int NODES = 20000000>
struct PersistentSegmentTree {
    using M = Monoid;
    using vtype = typename M::vtype;

    struct Node {
        vtype data;
        Node *l, *r;
        Node() {}
        Node(const vtype &_data) : data(_data), l(nullptr), r(nullptr) {}
    };

    Node *pool;
    int pid;
    long long N;
    Node *nil;
    vector<Node *> roots;

    PersistentSegmentTree(const vector<vtype> &v)
        : pid(0), N((long long)v.size()), nil(nullptr) {
        pool = new Node[NODES];
        nil = _new_node(M::unit);
        nil->l = nil->r = nil;
        roots.reserve(262144);
        roots.push_back(_build(0, N, v));
    }

    PersistentSegmentTree(long long N_)
        : pid(0), N(N_), nil(nullptr) {
        pool = new Node[NODES];
        nil = _new_node(M::unit);
        nil->l = nil->r = nil;
        roots.reserve(262144);
        roots.push_back(nil);
    }

    Node *_new_node(const vtype &x) {
        pool[pid].data = x;
        pool[pid].l = pool[pid].r = nil;
        return &(pool[pid++]);
    }

    Node *_merge(Node *l, Node *r) {
        pool[pid].data = M::op(l->data, r->data);
        pool[pid].l = l;
        pool[pid].r = r;
        return &(pool[pid++]);
    }

    Node *_build(long long l, long long r, const vector<vtype> &v) {
        if (l + 1 == r) return _new_node(v[l]);
        long long m = (l + r) >> 1;
        return _merge(_build(l, m, v), _build(m, r, v));
    }

private:
    Node *_set(Node *cur, long long i, const vtype &x, long long l, long long r) {
        if (l + 1 == r) return _new_node(x);
        long long m = (l + r) >> 1;
        if (i < m) {
            return _merge(_set(cur->l, i, x, l, m), cur->r);
        } else {
            return _merge(cur->l, _set(cur->r, i, x, m, r));
        }
    }

    Node *_add(Node *cur, long long i, const vtype &x, long long l, long long r) {
        if (l + 1 == r) {
            return _new_node(M::op(cur->data, x));
        }
        long long m = (l + r) >> 1;
        if (i < m) {
            return _merge(_add(cur->l, i, x, l, m), cur->r);
        } else {
            return _merge(cur->l, _add(cur->r, i, x, m, r));
        }
    }

    vtype _prod(Node *cur, long long L, long long R, long long l, long long r) const {
        if (cur == nil) return M::unit;
        if (r <= L || R <= l) return M::unit;
        if (L <= l && r <= R) return cur->data;
        long long m = (l + r) >> 1;
        return M::op(
            _prod(cur->l, L, R, l, m),
            _prod(cur->r, L, R, m, r)
        );
    }

public:
    Node *set(Node *root, long long k, const vtype &x) {
        Node *new_root = _set(root, k, x, 0, N);
        roots.push_back(new_root);
        return new_root;
    }

    Node *set(int t, long long k, const vtype &x) {
        Node *new_root = _set(roots[t], k, x, 0, N);
        roots.push_back(new_root);
        return new_root;
    }

    Node *set(long long k, const vtype &x) {
        Node *new_root = _set(roots.back(), k, x, 0, N);
        roots.push_back(new_root);
        return new_root;
    }

    Node *add(Node *root, long long k, const vtype &x) {
        Node *new_root = _add(root, k, x, 0, N);
        roots.push_back(new_root);
        return new_root;
    }

    Node *add(int t, long long k, const vtype &x) {
        Node *new_root = _add(roots[t], k, x, 0, N);
        roots.push_back(new_root);
        return new_root;
    }

    Node *add(long long k, const vtype &x) {
        Node *new_root = _add(roots.back(), k, x, 0, N);
        roots.push_back(new_root);
        return new_root;
    }

    vtype prod(Node *root, long long L, long long R) const {
        return _prod(root, L, R, 0, N);
    }

    vtype prod(int t, long long L, long long R) const {
        return _prod(roots[t], L, R, 0, N);
    }

    vtype prod(long long L, long long R) const {
        return _prod(roots.back(), L, R, 0, N);
    }

    vtype get(Node *root, long long k) const {
        return prod(root, k, k + 1);
    }

    vtype get(int t, long long k) const {
        return prod(t, k, k + 1);
    }

    vtype get(long long k) const {
        return prod(k, k + 1);
    }

    Node *new_tree() const {
        return nil;
    }
};

struct Monoid {
    using vtype = int;
    static constexpr vtype op(const vtype &x, const vtype &y) {
        return x + y;
    }
    static constexpr vtype unit = 0;
};
