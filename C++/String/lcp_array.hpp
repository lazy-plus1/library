#include <bits/stdc++.h>
using namespace std;

// lcp[i] = LCP(suffix sa[i], suffix sa[i + 1])
template <class T>
vector<int> lcp_array(const vector<T> &s, const vector<int> &sa) {
    int n = int(s.size());
    assert(n == (int)sa.size());
    assert(n >= 1);
    vector<int> rank(n);
    for (int i = 0; i < n; i++) {
        assert(0 <= sa[i] && sa[i] < n);
        rank[sa[i]] = i;
    }
    vector<int> lcp(n - 1);
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (h > 0) h--;
        int r = rank[i];
        if (r == 0) continue;
        int j = sa[r - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) {
            h++;
        }
        lcp[r - 1] = h;
    }
    return lcp;
}

vector<int> lcp_array(const string &s, const vector<int> &sa) {
    int n = int(s.size());
    vector<int> s2(n);
    for (int i = 0; i < n; i++) {
        s2[i] = (unsigned char)s[i];
    }
    return lcp_array(s2, sa);
}
