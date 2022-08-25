<<<<<<< HEAD
fn n_linear(m: &[u32], n: usize) -> u32 {

    let size = m.len();
    let mut it = vec![0;size];
    let mut q = vec![0;size];
    let mut u = vec![1];

    for i in 0..size {
        q[i] = m[i] + 1;
    }

    for _k in 0..n {

        let mut dist = u32::MAX;

        for i in 0..size {
            if q[i] < dist {
                dist = q[i];
            }
        }

        u.push (dist);
        for i in 0..size {
            if q[i] == dist {
                it[i] += 1;
                q[i] = m[i] * u[it[i]] + 1;
            }
        }
    }

    u[u.len() - 1]
=======


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
>>>>>>> f93690170342421b022abdcea3458bec7b404f00
}
