fn gcd2 (mut u: i32, mut v: i32) -> i32 {
    // Base cases: gcd(n, 0) = gcd(0, n) = n
    if (u == 0) { return v; }
    if (v == 0) { return u; }
   
    // Using identity 2
    let shift = (u | v).trailing_zeros();
    // Make u odd
    u >>= u.trailing_zeros();
    
    loop {
        // Make v odd
        v >>= v.trailing_zeros();

        // Using identity 4 (gcd(u, v) = gcd(|v-u|, min(u, v))
        v -= u;
        let m = v >> 31;
        u += v & m;
        v = (v + m) ^ m;

        if v == 0 {
            break;
        }
    }

    return u << shift;
}

pub fn binarygcd(u: i32, v: i32) -> i32 {
    // `wrapping_abs` gives a number's absolute value, unless that's 2³¹. 2³¹ won't fit in `i32`, so it gives -2³¹ instead.
    let mut v = v.wrapping_abs() as u32;
    if u == 0 { return v as i32; }
    let mut u = u.wrapping_abs() as u32;
    if v == 0 { return u as i32; }
    // `|` is bitwise OR. `trailing_zeros` quickly counts a binary number's
    // trailing zeros, giving its prime factorization's exponent on two.
    let gcd_exponent_on_two = (u | v).trailing_zeros();
    // `>>=` divides the left by two to the power of the right, storing that in
    // the left variable. `u` divided by its prime factorization's power of two
    // turns it odd.
    u >>= u.trailing_zeros();
    v >>= v.trailing_zeros();

    while u != v {
        if u < v {
            // Swap the variables' values with each other.
            core::mem::swap(&mut u, &mut v);
        }
        u -= v;
        u >>= u.trailing_zeros();
    }

    // `<<` multiplies the left by two to the power of the right.
    (u << gcd_exponent_on_two) as i32
}

pub fn gcd(mut a:u32, mut b:u32) -> u32 {

    while b != 0 {
        let tmp = a % b;
        a = b;
        b = tmp % b;
    }

    a
}

fn triple (max: u32) {
    let mut m = 2;
    let mut n = 1;

    while m * m + 1 <= max {
        if n >= m {
            n = m % 2;
            m = m + 1;
        }
        let c = m * m + n * n;

        if c >= max {
            n = m;
        } else {
            if gcd(m,n) == 1 {
                let a = m * m - n * n;
                let b = 2 * m * n;
                print! ("{} {} {} => {}\n", a,b,c,a+b+c);

            }
        }

        n += 2;
    }
}

fn find_max_triple(max: u32) -> (u32, u32, [u32;3]) {
    let mut res = (0,0,[0,0,0]);
    let mut m = 2;
    let mut n = 1;

    while m * m + 1 <= max {
        if n >= m {
            n = m % 2;
            m = m + 1;
        }
        let c = m * m + n * n;

        if c >= max {
            n = m;
        } else if gcd(m,n) == 1 {
            let a = m * m - n * n;
            let b = 2 * m * n;
            res.0 += 1;

            if a + b + c > res.1 {
                res.1 = a + b + c;
                res.2[0] = a;
                res.2[1] = b;
                res.2[2] = c;
            }
            //print! ("{} {} {} => {}\n", a,b,c,a+b+c);
        }

        n += 2;
    }

    res
}

fn main () {

    find_max_triple(50);


}
