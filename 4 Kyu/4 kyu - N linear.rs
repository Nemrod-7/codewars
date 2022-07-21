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
}
