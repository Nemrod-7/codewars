


fn main() {

    let mut p = vec![0,1];
    let mut n = 20;

    while p.len() <= n {
        p.push(2 * p[p.len()-1] + p[p.len()-2]);
    }
    
    print!("{}", p.len());
    
}
