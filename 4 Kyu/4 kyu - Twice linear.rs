fn dbl_linear(n: u32) -> u32{
        
    let mut a = 0;
    let mut b = 0;
    let mut u = vec![0; 2 * n as usize];
    
    u[0] = 1;
    
    for x in 1..n + 1 {
        let y = 2 * u[a] + 1;
        let z = 3 * u[b] + 1;
        
        if y <= z {
            u[x as usize] = y;         
            a += 1;
            if y == z { b += 1; }
        } else {
            u[x as usize] = z;       
            b += 1;
        }
    }
    
    u[n as usize]
}
