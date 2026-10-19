#include <bits/stdc++.h>
using namespace std;

struct UndoableUnionFind {
    struct History {
        int a, b;
        int old_a, old_b;
        int old_components;
    };

    vector<int> data;
    vector<History> history;
    int components;

    explicit UndoableUnionFind(int n)
        : data(n, -1), components(n) {}

    int root(int x) const {
        while (data[x] >= 0) x = data[x];
        return x;
    }

    bool same(int a, int b) const {
        return root(a) == root(b);
    }

    int size(int x) const {
        return -data[root(x)];
    }

    bool unite(int a, int b) {
        a = root(a), b = root(b);
        if (-data[a] < -data[b]) swap(a, b);
        history.push_back({
            a, b,
            data[a], data[b],
            components
        });
        if (a == b) return false;
        data[a] += data[b];
        data[b] = a;
        components--;
        return true;
    }

    void undo() {
        auto h = history.back();
        history.pop_back();
        data[h.a] = h.old_a;
        data[h.b] = h.old_b;
        components = h.old_components;
    }

    int snapshot() const {
        return history.size();
    }

    void rollback(int snap) {
        while ((int)history.size() > snap) undo();
    }

    int count_components() const {
        return components;
    }
};
