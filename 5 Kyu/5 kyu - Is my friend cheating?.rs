fn remove_nb (m: i32) -> Vec<(i32, i32)> {
    let n:i64 = m as i64;
    let cnt:i64 = n * (n + 1) / 2;
    let mut seq = Vec::<(i32,i32)>::new();
    
    for a in 2..n {
        
        let b = (cnt - a) / (a + 1);
        let sum = cnt - a - b;
        
        if b < n && a * b == sum {
            seq.push ((a as i32,b as i32));
        }
    }
    
    return seq;
}
