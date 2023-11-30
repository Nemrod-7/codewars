fn isprime (n: i32) -> bool {
    match n {
        1 => false,
        2 => true,
        _ => {
            if n % 2 == 0 { return false }

            for p in (3..=(n as f32).sqrt() as i32).step_by(2) {
                if n % p == 0 {
                    return false;
                }
            }
            true
        },
    }
}

fn big_primefac_div(mut n:i32) {
    let mut i = 2;
    let mut prime = 0;
    let mut fact = 0;
    
    if n < 0 {n = -n }

    while i * i < n {
        if n % i == 0 {
            if isprime(i) == true {
                prime = i;
            }
            fact = std::cmp::max(n / i, fact);
        }
        i += 1;
    }
    
    print!("{prime} {fact}\n");
}

fn spfs (n:i32) {
    
}

fn main () {

    big_primefac_div(100);// == [5, 50]
    big_primefac_div(1969);// == [179, 179]

    big_primefac_div(997);// == []
    big_primefac_div(-1800);// == [5, 900]


}
