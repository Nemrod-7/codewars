fn ex_euler(nb: i32) -> f64 {
    
    let h:f64 = 1.0 / nb as f64;
    let mut x:f64 = 0.0;
    let mut y:f64 = 1.0;
    let mut mid:f64 = 0.0;
    
    for _ in 0..nb {
        y = y + (2.0 - f64::exp (-4.0 * x) - 2.0 * y) * h;
        x += h;
        let z = 1.0 + 0.5 * f64::exp (-4.0 * x) - 0.5 * f64::exp (-2.0 * x);
        
        mid += f64::abs (y - z) / z;        
    }
    return f64::floor (mid / (nb + 1) as f64 * 1e6) / 1e6;
}
