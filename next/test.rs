use std::collections::HashMap;

fn rate (part: &Vec<u32>) -> u32 {

    let mut hist:HashMap<u32,u32> = HashMap::new();
    
    for num in part {
        if let Some(cnt) = hist.get(&num) {
            hist.insert (*num, cnt+1);
        } else {
            hist.insert (*num, 1);
        }
    }

    hist.iter().map(|(x,y)| x.pow(*y) ).sum::<u32>() * part.len() as u32
}

fn recurse (n: u32, mut k:  u32, part: &mut Vec<u32>, mode: &str) -> (Vec<u32>, u32) {

    if n == 1 {
        (part.clone(), rate (part))
    } else {
        let mut pair = (vec![0;0], if mode == "max" { std::u32::MIN } else { std::u32::MAX });
        
        while k > 1 {
            if n % k == 0 {
                part.push(k);
                let res = recurse (n / k, k, part, mode);

                match mode {
                    "max" => if res.1 > pair.1 { pair = res },
                    "min" => if res.1 < pair.1 { pair = res },
                      _   => (),
                }
                
                part.pop();
            }
            k -= 1;
        }
        
        pair
    }
}

fn find_spec_prod_part (n: u32, mode: &str) -> Option<(Vec<u32>, u32)> {
    
    let mut part = vec![0;0];
    let res = recurse(n, n-1, &mut part, mode);

    match res.0.len() {
        0 => None,
        _ => Some (res),
    }
}
fn main() {

    let n = 1416;

    let res = find_spec_prod_part (10007, "max");

    print!("{:?}\n", res);



}
