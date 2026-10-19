template <int SIGMA = 26, char OFFSET = 'a'>
struct Trie {
    struct Node {
        array<int, SIGMA> nxt;
        int pass = 0;
        int end = 0;
        Node() { nxt.fill(-1); }
    };

    vector<Node> nodes;

    Trie() { nodes.emplace_back(); }

    int id(char c) const {
        int x = c - OFFSET;
        assert(0 <= x && x < SIGMA);
        return x;
    }

    int add(const string &s) {
        int v = 0;
        nodes[v].pass++;
        for (char c : s) {
            int x = id(c);
            if (nodes[v].nxt[x] == -1) {
                nodes[v].nxt[x] = nodes.size();
                nodes.emplace_back();
            }
            v = nodes[v].nxt[x];
            nodes[v].pass++;
        }
        nodes[v].end++;
        return v;
    }

    int find_node(const string &s) const {
        int v = 0;
        for (char c : s) {
            int x = id(c);
            if (nodes[v].nxt[x] == -1) return -1;
            v = nodes[v].nxt[x];
        }
        return v;
    }

    int count(const string &s) const {
        int v = find_node(s);
        return v == -1 ? 0 : nodes[v].end;
    }

    int count_prefix(const string &s) const {
        int v = find_node(s);
        return v == -1 ? 0 : nodes[v].pass;
    }
};
