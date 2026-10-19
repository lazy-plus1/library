#include <bits/stdc++.h>
using namespace std;

struct SlopeTrick {
    using T = long long;
    static constexpr T INF = (1LL << 61);
    priority_queue<T> L;
    priority_queue<T, vector<T>, greater<T>> R;
    T min_f, l_add, r_add;

    explicit SlopeTrick(T ini = 0) : min_f(ini), l_add(0), r_add(0) {
        push_l(-INF);
        push_r(INF);
    }

private:
    void push_l(T x) {
        L.push(x - l_add);
    }

    void push_r(T x) {
        R.push(x - r_add);
    }

    T pop_l() {
        T x = L.top();
        L.pop();
        return x + l_add;
    }

    T pop_r() {
        T x = R.top();
        R.pop();
        return x + r_add;
    }

    T pushpop_l(T x) {
        push_l(x);
        return pop_l();
    }

    T pushpop_r(T x) {
        push_r(x);
        return pop_r();
    }

public:
    T left_max() const {
        return L.top() + l_add;
    }

    T right_min() const {
        return R.top() + r_add;
    }

    T min() const {
        return min_f;
    }

    pair<T, T> argmin() const {
        return {left_max(), right_min()};
    }

    // f(x) += a
    void add_all(T a) {
        min_f += a;
    }

    // f(x) += max(x - a, 0)
    void add_x_minus_a(T a) {
        min_f += max(left_max() - a, T(0));
        T x = pushpop_l(a);
        push_r(x);
    }

    // f(x) += max(a - x, 0)
    void add_a_minus_x(T a) {
        min_f += max(a - right_min(), T(0));
        T x = pushpop_r(a);
        push_l(x);
    }

    // f(x) += |x - a|
    void add_abs(T a) {
        add_x_minus_a(a);
        add_a_minus_x(a);
    }

    // f(x) <- f(x - a)
    void shift(T a) {
        l_add += a;
        r_add += a;
    }

    // f(x) <- min_{x-b <= y <= x-a} f(y)
    // requires a <= b
    void shift(T a, T b) {
        assert(a <= b);
        l_add += a;
        r_add += b;
    }

    void clear_left() {
        L = priority_queue<T>();
        push_l(-INF);
    }

    void clear_right() {
        R = priority_queue<T, vector<T>, greater<T>>();
        push_r(INF);
    }

    // return f(x) : O(n log n)
    T get_value(T x) const {
        T res = min_f;
        auto lq = L;
        while (!lq.empty()) {
            T v = lq.top();
            lq.pop();
            res += max(T(0), (v + l_add) - x);
        }
        auto rq = R;
        while (!rq.empty()) {
            T v = rq.top();
            rq.pop();
            res += max(T(0), x - (v + r_add));
        }
        return res;
    }
};

