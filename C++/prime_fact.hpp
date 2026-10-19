#include <bits/stdc++.h>
using namespace std;

vector<pair<long long, int>> fact(long long n) {
    vector<pair<long long, int>> res;
    for (long long p = 2; p <= n / p; p++) {
        if (n % p != 0) continue;
        int e = 0;
        while (n % p == 0) {
            n /= p;
            e++;
        }
        res.push_back({p, e});
    }
    if (n > 1) res.push_back({n, 1});
    return res;
}
