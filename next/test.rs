use std::collections::HashMap;

fn find_spec_prod_part (n: u64, mode: &str) -> u64 {

    let maxv = std::u64::MIN;
    let minv = std::u64::MAX;

    match mode {
        "max" => (),
        "min" => (),
          _   => (),
    }

    0
}
fn eval (buff: &Vec<u64>) -> u64 {

    let mut hist:HashMap<u64,usize> = HashMap::new();

    for num in buff {
        hist.insert (*num, if let Some (cnt) = hist.get(&num) {cnt + 1} else { 1  } );
    }

    let sum = hist.iter().map(|(num,&cnt)| num.pow(cnt as u32)).sum::<u64>() * buff.len() as u64;

    sum
}
fn is_prime (num: i32) -> bool {

    if num <= 3 { return true }
    if num % 2 == 0 || num % 3 == 0 { return false }

    let end = (num as f32).sqrt() as i32;
    let mut i = 5;

    while i <= end {
        if num % i == 0 || num % (i + 2) == 0 { return false }
        i += 6;
    }
    true
}
fn decomp (num: i32) -> String {

    let mut os = String::new();
    let mut comp:Vec<(i32,i32)> = Vec::new();

    for factor in 2 ..= num {
        if is_prime (factor) {
            let mut sum = 0;

            for k in 1 .. num {
                let cnt = num / factor.pow(k as u32);
                
                if cnt == 0 {
                    comp.push((factor,sum));
                    break;
                }
                sum += cnt;
            }
        }
    }

    for (fst,scd) in comp {
       if scd != 1 { os += &format!("{fst}^{scd}") } else { os += &format!("{fst}") }
       os += &format!(" * ");
    }

    os[0..os.len() - 3].to_string()
}
fn factorize (n: u32) -> Vec<u32> {
    let fact:Vec<u32> = Vec::new();
    
    for k in 2...n {
        if n % k == 0 { fact.push(k) }
    }

    fact
}
fn decomp2 (num: i32) -> i32 {

    let mut os = String::new();
    let mut comp:Vec<(i32,i32)> = Vec::new();

    for factor in 2 ..= num {
        if is_prime (factor) {

        let partition:Vec<i32> = Vec::new();

        let mut subset= vec![factor];
     
    }
    0
}

fn main () {
    
    let res = decomp2 (12);

    print!("{res}\n");

}
