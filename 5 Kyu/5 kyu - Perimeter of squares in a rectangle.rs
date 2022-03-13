fn perimeter(n: u64) -> u64 {
    let mut prev;
    let mut now = 1;
    let mut nxt = 1;
    let mut sum = 0;
    
    for _i in 0..n + 1 {
        prev = now;
        now = nxt;
        nxt = now + prev;
        sum += prev * 4;
    }
    
    return sum;
}
