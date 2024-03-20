
fn sieve(num: usize) -> Vec<i64> {
    let mut sieve = vec![true; (num >> 1) + 1];
    let mut prime = vec![2];

    (3..=num)
        .step_by(2)
        .for_each(
            |i| if sieve[i >> 1] == true {
                prime.push(i as i64);
                (i*i..=num).step_by(2 * i).for_each(|j| sieve[j >> 1] = false)
            } 
            );

    prime
}

fn main () {

    let lst:Vec<i64> = vec![15,21,24,30,45];

    let lim = *lst.iter().max().unwrap() as usize;
    let prime = sieve(lim);
    let mut os = String::new();

    for p in &prime {
        let mut sum = 0; 

        for num in &lst {
            if num.abs() % p == 0 {
                sum += num;
            }
        }

        if sum != 0 {
            os += &format!("({p} {sum})");
        }
    }



}
