use itertools::Itertools;

fn itov (mut num: usize) -> Vec<usize> {
    let mut ve = Vec::new();

    while num != 0 {
        ve.push(num % 10);
        num /= 10;
    }

    ve
}
fn permutational_primes (lim: u32, k: u32) -> (u32,u32,u32) {

    let lim = lim as usize;
    let mut prime = vec![true; lim + 1];
    let end = (lim as f64).sqrt() as usize;

    prime[0] = false;
    prime[1] = false;

    for i in (4..=lim).step_by(2) { prime[i] = false }

    for p in (3..=end).step_by(2) {
        if prime[p] == true {
            for i in (p*p ..=lim).step_by(p) {
                prime[i] = false
            } 
        } 
    }
    
    let mut cnt = 0;
    let mut maxv = 0;
    let mut minv = lim as u32;

    for num in 13..=lim {
        if prime[num] == true {

            let mut nperm = 0;
            let decomp = itov (num);

            for perm in decomp.iter().permutations(decomp.len()).unique() {

                if perm[0] != &0 {
                    let perm = perm.iter().fold(0, |acc, &&elem| acc * 10 + elem);

                    if perm <= lim && perm != num && prime[perm] == true {
                        nperm += 1;
                        prime[perm] = false;
                    }
                }
            }

            if nperm == k as usize { 
                cnt += 1;
                maxv = std::cmp::max(maxv,num as u32);
                minv = std::cmp::min(minv,num as u32);
            }
        }
    }

    match cnt {
        0 => (0,0,0),
        _ => (cnt,minv,maxv),
    }
}
