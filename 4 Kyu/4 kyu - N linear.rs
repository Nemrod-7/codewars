

fn n_linear(m: &[u32], n: usize) -> u32 {
/*
    let size = m.size(), i = 0;
    vector<uint32_t> N(m.begin(), m.end()), it(size), u{1};
    vector<queue<uint32_t>> Q(size);

    for (auto num : m)
        Q[i++].push(num + 1);

    while(n-->0) {
        uint32_t dist = numeric_limits<uint32_t>::max();
        for (i = 0; i < size; i++) {
            if (Q[i].front() < dist)
                dist = Q[i].front();
        }

        u.push_back(dist);
        for (i = 0; i < size; i++)
            if (Q[i].front() == dist) {
                Q[i].pop();
                it[i]++;
                Q[i].push(u[it[i]] * N[i] + 1);
            }

    }
    */
    return u.back();
}

fn main () {


    return 0;
}
