fn zeros(n: u64) -> u64 {
    let mut pow = 5;
    let mut cnt = 0;
    
    while pow <= n {
        cnt += n / pow;
        pow *= 5;
    }
    
    return cnt;
}
