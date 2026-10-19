#include <bits/stdc++.h>
using namespace std;

struct WeightedSlopeTrick {
    using T = long long;
    static constexpr T INF = (1LL << 61);

private:
    struct P {
        T x, c;
        P(T x, T c) : x(x), c(c) {}
        friend bool operator<(const P& a, const P& b) { return a.x < b.x; }
        friend bool operator>(const P& a, const P& b) { return a.x > b.x; }
    };

    priority_queue<P> L;
    priority_queue<P, vector<P>, greater<P>> R;
    T min_f_, l_add, r_add;

    void push_l(T x, T c = 1) {
        if (c == 0) return;
        L.emplace(x - l_add, c);
    }

    void push_r(T x, T c = 1) {
        if (c == 0) return;
        R.emplace(x - r_add, c);
    }

    P top_l() const { return P(L.top().x + l_add, L.top().c); }
    P top_r() const { return P(R.top().x + r_add, R.top().c); }

    void pop_l() { L.pop(); }
    void pop_r() { R.pop(); }

public:
    explicit WeightedSlopeTrick(T initial_min = 0)
        : min_f_(initial_min), l_add(0), r_add(0) {}

    T min_f() const { return min_f_; }

    pair<T, T> argmin() const {
        T l = L.empty() ? -INF : top_l().x;
        T r = R.empty() ? INF : top_r().x;
        return {l, r};
    }

    // f(x) += a
    void add_all(T a) {
        min_f_ += a;
    }

    // f(x) += c * max(x - a, 0)
    void add_x_minus_a(T a, T c = 1) {
        assert(c >= 0);
        T used = 0;
        while (used < c && !L.empty()) {
            auto [x, cnt] = top_l();
            if (x <= a) break;
            pop_l();
            T tmp = min(c - used, cnt);
            push_r(x, tmp);
            if (cnt > tmp) push_l(x, cnt - tmp);
            min_f_ += (x - a) * tmp;
            used += tmp;
        }
        if (used > 0) push_l(a, used);
        if (c > used) push_r(a, c - used);
    }

    // f(x) += c * max(a - x, 0)
    void add_a_minus_x(T a, T c = 1) {
        assert(c >= 0);
        T used = 0;
        while (used < c && !R.empty()) {
            auto [x, cnt] = top_r();
            if (x >= a) break;
            pop_r();
            T tmp = min(c - used, cnt);
            push_l(x, tmp);
            if (cnt > tmp) push_r(x, cnt - tmp);
            min_f_ += (a - x) * tmp;
            used += tmp;
        }
        if (used > 0) push_r(a, used);
        if (c > used) push_l(a, c - used);
    }

    // f(x) += c * |x - a|
    void add_abs(T a, T c = 1) {
        add_x_minus_a(a, c);
        add_a_minus_x(a, c);
    }

    // f(x) <- f(x - a)
    void shift(T a) {
        l_add += a;
        r_add += a;
    }

    void shift_left(T a) { l_add += a; }
    void shift_right(T a) { r_add += a; }

    // f(x) <- min_{x-b <= y <= x-a} f(y)
    // requires a <= b
    void shift(T a, T b) {
        assert(a <= b);
        l_add += a;
        r_add += b;
    }

    void clear_left() { L = priority_queue<P>(); }
    void clear_right() { R = priority_queue<P, vector<P>, greater<P>>(); }

    // destructive merge
    void merge(WeightedSlopeTrick& other) {
        if (L.size() + R.size() < other.L.size() + other.R.size()) {
            swap(*this, other);
        }
        while (!other.L.empty()) {
            auto [x, c] = other.top_l();
            other.pop_l();
            add_a_minus_x(x, c);
        }
        while (!other.R.empty()) {
            auto [x, c] = other.top_r();
            other.pop_r();
            add_x_minus_a(x, c);
        }
        add_all(other.min_f_);
    }

    // return f(x) : O(n log n)
    T get_value(T x) const {
        T res = min_f_;
        auto lq = L;
        while (!lq.empty()) {
            auto [v, c] = lq.top();
            lq.pop();
            T p = v + l_add;
            if (p < x) break;
            res += (p - x) * c;
        }
        auto rq = R;
        while (!rq.empty()) {
            auto [v, c] = rq.top();
            rq.pop();
            T p = v + r_add;
            if (x < p) break;
            res += (x - p) * c;
        }
        return res;
    }

    void debug() const {
        cerr << "l_add, r_add, min_f : ";
        cerr << l_add << ", " << r_add << ", " << min_f_ << '\n';
        auto lq = L;
        cerr << "L : ";
        while (!lq.empty()) {
            auto [x, c] = lq.top();
            lq.pop();
            cerr << "(" << x + l_add << ", " << c << "), ";
        }
        cerr << '\n';
        auto rq = R;
        cerr << "R : ";
        while (!rq.empty()) {
            auto [x, c] = rq.top();
            rq.pop();
            cerr << "(" << x + r_add << ", " << c << "), ";
        }
        cerr << '\n';
        auto [l, r] = argmin();
        cerr << "argmin : [" << l << ", " << r << "]\n";
        cerr << "min_f : " << min_f_ << '\n';
    }
};
