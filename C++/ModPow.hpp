long long mpow(long long a, long long e, long long p) {
    long long ret = 1;
    while (e > 0) {
        if (e & 1) ret = ret * a % p;
        a = a * a % p;
        e >>= 1;
    }
    return ret;
}
