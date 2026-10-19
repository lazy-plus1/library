#include <bits/stdc++.h>
using namespace std;

struct Mo {
private:
    struct Query {
        int l, r, idx;
        long long ord;
    };

    int n;
    vector<Query> qs;

    static long long hilbert_curve(int x, int y, int lg, int rot = 1) {
        if (lg == 0) return 0;
        int half = 1 << (lg - 1);
        int rx = (x & half) > 0;
        int ry = (y & half) > 0;
        int seg = rx ^ (ry * 3);
        seg = (seg + rot) & 3;
        static const int rotate_delta[4] = {3, 0, 0, 1};
        int nx = x & (half - 1);
        int ny = y & (half - 1);
        int next_rot = (rot + rotate_delta[seg]) & 3;
        long long square_size = 1LL << (2 * lg - 2);
        long long res = seg * square_size;
        long long add = hilbert_curve(nx, ny, lg - 1, next_rot);
        res += (seg == 1 || seg == 2) ? add : square_size - add - 1;
        return res;
    }

public:
    Mo(int n_) : n(n_) {}

    void insert(int l, int r) {
        int idx = (int)qs.size();
        qs.push_back({l, r, idx, 0});
    }

    int size() const {
        return (int)qs.size();
    }

    template <class AddL, class AddR, class DelL, class DelR, class Answer>
    void execute(AddL add_l, AddR add_r, DelL del_l, DelR del_r, Answer answer) {
        int lg = 1;
        while ((1 << lg) <= n) lg++;
        for (auto &q : qs) {
            q.ord = hilbert_curve(q.l, q.r, lg);
        }
        sort(qs.begin(), qs.end(), [](const Query &a, const Query &b) {
            return a.ord < b.ord;
        });
        int nl = 0, nr = 0;
        for (const auto &q : qs) {
            while (nl > q.l) add_l(--nl);
            while (nr < q.r) add_r(nr++);
            while (nl < q.l) del_l(nl++);
            while (nr > q.r) del_r(--nr);
            answer(q.idx);
        }
    }

    // 左右で add/del を分けなくてよい場合用
    template <class Add, class Del, class Answer>
    void execute(Add add, Del del, Answer answer) {
        execute(
            [&](int i) { add(i); },
            [&](int i) { add(i); },
            [&](int i) { del(i); },
            [&](int i) { del(i); },
            answer
        );
    }
};
