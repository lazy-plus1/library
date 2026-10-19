// Interval Set : 区間と値を set で管理する
// VAL : 持つ値の型 | T : 区間 = pair<T, T> | **半開**区間
template <class VAL, class T = long long>
struct IntervalSet {
    struct Node {
        T l, r;
        VAL val;
        Node(const T &l, const T &r, const VAL &val) : l(l), r(r), val(val) {}
        constexpr bool operator < (const Node &rhs) const {
            if (l != rhs.l) return l < rhs.l;
            else return r < rhs.r;
        }
        friend ostream& operator << (ostream &s, const Node &e) {
            return s << "([" << e.l << ", " << e.r << "): " << e.val << ")";
        }
    };

    // identity : 持つ値の初期値
    const VAL identity;
    set<Node> S;

    // constructor : 配列で初期化可能 (連続するものをまとめる)
    IntervalSet(const VAL &identity = VAL()) : identity(identity) {}
    IntervalSet(const vector<VAL> &v, const VAL &identity = VAL()) : identity(identity) {
        vector<Node> vec;
        for (int l = 0; l < (int)v.size();) {
            int r = l;
            while (r < (int)v.size() && v[r] == v[l]) r++;
            vec.emplace_back(l, r, v[l]);
            l = r;
        }
        S = set<Node>(vec.begin(), vec.end());
    }

    // iterator を取得する
    constexpr typename set<Node>::iterator begin() { return S.begin(); }
    constexpr typename set<Node>::iterator end() { return S.end(); }

    // p の属する区間の iterator を取得する (なかったら S.end() を返す)
    constexpr typename set<Node>::iterator get(const T &p) {
        auto it = S.upper_bound(Node(p, numeric_limits<T>::max(), 0));
        if (it == S.begin()) return S.end();
        it = prev(it);
        if (it->l <= p && p < it->r) return it;
        else return S.end();
    }

    // p 以上の要素を含む最も左側の区間の iterator を取得する
    constexpr typename set<Node>::iterator lower_bound(const T &p) {
        auto it = get(p);
        if (it != S.end()) return it;
        return S.upper_bound(Node(p, numeric_limits<T>::max(), 0));
    }

    // 点 p or 区間 [l, r) を含む区間が存在するか判定
    constexpr bool covered(const T &p) {
        auto it = get(p);
        if (it != S.end()) return true;
        else return false;
    }
    constexpr bool covered(const T &l, const T &r) {
        assert(l <= r);
        if (l == r) return true;
        auto it = get(l);
        if (it != S.end() && r <= it->r) return true;
        else return false;
    }

    // p と q が同じ区間に属するか判定
    constexpr bool same(const T &p, const T &q) {
        if (!covered(p) || !covered(q)) return false;
        return get(p) == get(q);
    }

    // 点 p に対応する値を取得
    constexpr VAL get_val(const T &p) {
        auto it = get(p);
        if (it != S.end()) return it->val;
        else return identity;
    }
    VAL operator [] (const T &p) const {
        return get_val(p);
    }

    // p 以上で区間に含まれない最小の点 (mex) を取得
    constexpr T get_mex(const T &p = 0) {
        auto it = S.upper_bound(Node(p, numeric_limits<T>::max(), 0));
        if (it == S.begin()) return p;
        it = prev(it);
        if (it->l <= p && p < it->r) return it->r;
        else return p;
    }

    // 区間 [l, r) を val で更新する
    // del : 区間の削除に伴い, 外部の変数を更新するコールバック関数
    // add : 区間の追加に伴い... (del と add は逆操作)
    template<class ADDFUNC, class DELFUNC> void update(T l, T r, const VAL &val, const ADDFUNC &add, const DELFUNC &del) {
        auto it = S.lower_bound(Node(l, 0, val));
        while (it != S.end() && it->l <= r) {
            if (it->l == r) {
                if (it->val == val) {
                    r = it->r;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                }
                break;
            }
            if (it->r <= r) {
                del(it->l, it->r, it->val);
                it = S.erase(it);
            } else {
                if (it->val == val) {
                    r = it->r;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                } else {
                    Node node = *it;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                    it = S.emplace_hint(it, r, node.r, node.val);
                    add(it->l, it->r, it->val);
                }
            }
        }
        if (it != S.begin()) {
            it = prev(it);
            if (it->r == l) {
                if (it->val == val) {
                    l = it->l;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                }
            } else if (l < it->r) {
                if (it->val == val) {
                    l = min(l, it->l);
                    r = max(r, it->r);
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                } else {
                    if (r < it->r) {
                        it = S.emplace_hint(next(it), r, it->r, it->val);
                        add(it->l, it->r, it->val);
                        it = prev(it);
                    }
                    Node node = *it;
                    del(it->l, it->r, it->val);
                    it = S.erase(it);
                    it = S.emplace_hint(it, node.l, l, node.val);
                    add(it->l, it->r, it->val);
                }
            }
        }
        if (it != S.end()) it = next(it);
        it = S.emplace_hint(it, l, r, val);
        add(it->l, it->r, it->val);
    }
    void update(const T &l, const T &r, const VAL &val) {
        update(l, r, val, [](T, T, VAL){}, [](T, T, VAL){});
    }
    template<class ADDFUNC, class DELFUNC> void insert(T l, T r, const ADDFUNC &add, const DELFUNC &del) {
        update(l, r, VAL(), add, del);
    }
    void insert(const T &l, const T &r) {
        update(l, r, VAL(), [](T, T, VAL){}, [](T, T, VAL){});
    }

    // 区間 [l, r) を削除する
    // del, add : update を参照
    template<class ADDFUNC, class DELFUNC> void erase(T l, T r, const ADDFUNC &add, const DELFUNC &del) {
        auto it = S.lower_bound(Node(l, 0, VAL()));
        while (it != S.end() && it->l <= r) {
            if (it->l == r) break;
            if (it->r <= r) {
                del(it->l, it->r, it->val);
                it = S.erase(it);
            } else {
                Node node = *it;
                del(it->l, it->r, it->val);
                it = S.erase(it);
                it = S.emplace_hint(it, r, node.r, node.val);
                add(it->l, it->r, it->val);
            }
        }
        if (it != S.begin()) {
            it = prev(it);
            if (l < it->r) {
                if (r < it->r) {
                    it = S.emplace_hint(next(it), r, it->r, it->val);
                    add(it->l, it->r, it->val);
                    it = prev(it);
                }
                Node node = *it;
                del(it->l, it->r, it->val);
                it = S.erase(it);
                it = S.emplace_hint(it, node.l, l, node.val);
                add(it->l, it->r, it->val);
            }
        }
    }
    void erase(const T &l, const T &r) {
        erase(l, r, [](T, T, VAL){}, [](T, T, VAL){});
    }

    // debug
    friend ostream& operator << (ostream &s, const IntervalSet &ins) {
        for (auto e : ins.S) {
            s << "([" << e.l << ", " << e.r << "): " << e.val << ") ";
        }
        return s;
    }
};
