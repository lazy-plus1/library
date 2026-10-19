template <class T = int, int LOG = 30>
struct BinaryTrie {
    struct Node {
        array<int, 2> nxt;
        int cnt = 0;
        Node() { nxt.fill(-1); }
    };

    vector<Node> nodes;

    BinaryTrie() { nodes.emplace_back(); }

    int size() const { return nodes[0].cnt; }

    void add(T x) {
        int v = 0;
        nodes[v].cnt++;
        for (int k = LOG - 1; k >= 0; k--) {
            int b = (x >> k) & 1;
            if (nodes[v].nxt[b] == -1) {
                nodes[v].nxt[b] = nodes.size();
                nodes.emplace_back();
            }
            v = nodes[v].nxt[b];
            nodes[v].cnt++;
        }
    }

    bool erase(T x) {
        if (count(x) == 0) return false;
        int v = 0;
        nodes[v].cnt--;
        for (int k = LOG - 1; k >= 0; k--) {
            int b = (x >> k) & 1;
            v = nodes[v].nxt[b];
            nodes[v].cnt--;
        }
        return true;
    }

    int count(T x) const {
        int v = 0;
        for (int k = LOG - 1; k >= 0; k--) {
            int b = (x >> k) & 1;
            if (nodes[v].nxt[b] == -1) return 0;
            v = nodes[v].nxt[b];
        }
        return nodes[v].cnt;
    }

    T kth_element(int k, T xor_val = 0) const {
        assert(0 <= k && k < size());
        int v = 0;
        T res = 0;
        for (int i = LOG - 1; i >= 0; i--) {
            int b = (xor_val >> i) & 1;
            int low = nodes[v].nxt[b];
            int cnt_low = (low == -1 ? 0 : nodes[low].cnt);
            if (k < cnt_low) {
                v = low;
            } else {
                k -= cnt_low;
                res |= T(1) << i;
                v = nodes[v].nxt[b ^ 1];
            }
        }
        return res;
    }

    T min_element(T xor_val = 0) const {
        return kth_element(0, xor_val);
    }

    T max_element(T xor_val = 0) const {
        return kth_element(size() - 1, xor_val);
    }

    int count_less(T x, T xor_val = 0) const {
        int v = 0;
        int res = 0;
        for (int i = LOG - 1; i >= 0; i--) {
            if (v == -1) break;
            int b0 = (xor_val >> i) & 1;
            if ((x >> i) & 1) {
                int low = nodes[v].nxt[b0];
                if (low != -1) res += nodes[low].cnt;
                v = nodes[v].nxt[b0 ^ 1];
            } else {
                v = nodes[v].nxt[b0];
            }
        }
        return res;
    }
};
