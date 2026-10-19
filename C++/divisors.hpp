#include <bits/stdc++.h>
using namespace std;

vector<long long> divisors(long long n) {
    vector<long long> res;
    for (long long i = 1; i <= n / i; i++) {
        if (n % i != 0) continue;
        res.push_back(i);
        if (i != n / i) res.push_back(n / i);
    }
    sort(res.begin(), res.end());
    return res;
}
