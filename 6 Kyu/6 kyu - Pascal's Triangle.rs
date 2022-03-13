fn binom (n:usize, k:usize) -> usize {
    if k == 0 || k == n { return 1; }
    if k  > n { return 0; }
    
    let mut res:usize = 1;
    
    for i in 1..k + 1 {
        res = res * (n + 1 - i) / i;
    }
    
    return res as usize;
}

fn pascals_triangle (n: usize) -> Vec<usize> {
    let mut tri = Vec::<usize>::new();
    
    for n in 0..n {
        for k in 0..n + 1 {
            tri.push (binom (n, k));
        }
    }
    return tri;
}
