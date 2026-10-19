#include <bits/stdc++.h>
using namespace std;

vector<bool> prime_sieve(int N) {
    vector<bool> sieve(max(1, N) + 1, 1);
    sieve[0] = sieve[1] = 0;
    for (int i = 2; i * i <= N; i++) {
        if (sieve[i]) {
            for (int j = i * i; j <= N; j += i) sieve[j] = 0;
        }
    }
    return sieve;
}
