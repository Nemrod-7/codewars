/*

Overall:1 kyu / 37.4%
BF:6 kyu / 62.7%
C:2 kyu / 99.6%
C++:1 kyu / 26.3%
Python:7 kyu / 1.8%
Rust:2 kyu / 2.2%

*/
fn limit (a: u64, b: u64) -> u64 {
    if a > b { return a - b }
    return 0
}
fn highpow (mut x: u64) -> u64 {
    let mut n = 1;

    while x >= 2 {
        n *= 2;
        x /= 2;
    }

    n
}
fn sumr (m: u64, n: u64, l: u64, t: u64) -> u64 {

    let m2 = limit (m, l);
    let n2 = limit (n, l);
    
    let mut fst = m2 + n2 - 1;
    let mut scd = n2 - m2;

    if fst % 2 != 0 { scd /= 2 } else { fst /= 2 }

    return ((fst % t) * (scd % t)) % t;
}
fn elder_age (mut m: u64, mut n: u64, l: u64, t: u64) -> u64 {

    if m == 0 || n == 0 { return 0 }
    if m < n { std::mem::swap (&mut m, &mut n) }
    if n == 1 { return sumr (0,m,l,t) }

    let pm = highpow (m);
    let pn = highpow (n);
    let pml = limit (pm, l) % t;

    let mut cnt = sumr (0,pm,l,t);

    if pm == pn {
        cnt *= pm % t;

        let a = m + n;
        let b = pm * 2;
        let c = sumr (pm,b,l,t);
        cnt += (c * ((a - b) % t)) % t;
        cnt += elder_age (m - pm, n - pn, l, t);

    } else {
        cnt *= n % t;

        let a = ((pml % t) * ((m - pm) % t)) % t;
        let b = n % t;

        cnt += (a * b) % t;
        cnt += elder_age (m - pm, n, limit (l, pm), t);
    }

    cnt % t
}

fn main() {

    let m:u64 = 8261418348471353299;
    let n:u64 = 6822841712025092036;
    let l:u64 = 975008;
    let t:u64 = 1346162979;

    elder_age (170, 183, 18, 2979);


    print!("\nend\n");
}
