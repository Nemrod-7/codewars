fn doubles (maxk: i32, maxn: i32) -> f64 {
    let mut sum: f64 = 0.0;

    for k in 1..= maxk {
        for n in 1..= maxn {
            sum += 1.0 / (k as f64 * f64::powf (n as f64 + 1.0, 2.0 * k as f64));
        }
    }
    
    sum 
}
