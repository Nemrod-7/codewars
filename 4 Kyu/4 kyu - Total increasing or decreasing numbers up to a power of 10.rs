fn total_inc_dec(n: u32) -> u64 {
    let n = n as u64;
    num::integer::binomial (n+9, n) + num::integer::binomial (n+10, n) - 1 - n * 10
}
