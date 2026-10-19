#include <bits/stdc++.h>
using namespace std;


struct UnionFind {
    vector<int> data;
    UnionFind(int n) : data(n, -1) {}

    int find(int k) { return data[k] < 0 ? k : data[k] = find(data[k]); }

    bool unite(int x, int y) {
        if ((x = find(x)) == (y = find(y))) return false;
        if (data[x] > data[y]) swap(x, y);
        data[x] += data[y];
        data[y] = x;
        return true;
    }

    int size(int k) { return -data[find(k)]; }

    bool same(int x, int y) { return find(x) == find(y); }
};
