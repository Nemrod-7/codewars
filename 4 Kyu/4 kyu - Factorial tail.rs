fn zeroes(base: i32, number: i32) -> i32 {
    let mut fact = base;
    let mut zeroes = number;
    
    for i in 2..= fact {
        if fact % i == 0 {
            
            let mut p = 0;
            while fact % i == 0 {
                p += 1;
                fact /= i;
            }
            
            let mut c = 0;
            let mut k = number;
            while k / i > 0 {
                c += k / i;
                k /= i;
            }
            
            zeroes = std::cmp::min (zeroes, c / p);
        }
    }
    
    zeroes
}
