use std::collections::HashSet;

fn decomp (num:u32) -> Vec<u32> {
    num.to_string().chars().map (|x| x.to_digit(10).unwrap()) .collect::<Vec<u32>>()
}
fn mklist (limit: usize, arg: &[u32]) -> Vec<u32>  {

    let mut it = [0;10];
    let mut arr = arg.iter().map(|x| decomp (*x)).collect::<Vec<Vec<u32>>>();
    let mut list = Vec::new();
    let mut used = HashSet::new();

    loop {
        for i in 0..arr.len() {
            let dig = arr[i][it[i]];

            if used.get(&dig) == None {
                list.push(dig);
                if list.len() == limit { return list }
            }
            used.insert (dig);
            if it[i] < arr[i].len() - 1 { it[i] += 1; }
        }
    }

    list
}
fn searchcomb (depth:usize, list: &Vec<u32>) -> u32 {

    let mut sum = 0;
    let mut s1 = vec![vec![0;0];1];

    while let Some (comb) = s1.pop() {

        if comb.len() == depth {
            sum += comb.iter().sum::<u32>();
            //print! ("{:?}\n", comb);
        } else {
            for dig in list {
                if !comb.contains(dig) {
                    let mut next = comb.clone();
                    next.push(*dig);
                    s1.push(next);
                }
            }
        }
    }

    sum
}
fn gta (limit: u32, arg: &[u32]) -> u32 {

    let limit = limit as usize;
    let mut sum = 0;
    let mut list = mklist (limit, arg);

    for depth in 1..=limit {
        sum += searchcomb (depth, &list);
    }

    sum
}
fn main() {


    let limit = 8;
    let arr = [12348, 47, 3639];

    let ass:Vec<Vec<u32>> = arr.iter().map(|x| decomp (*x)).collect();

    for row in ass {
        print!("{} ", row.len());
    }
    /*
    print! ("{:?}", a33);
    */
    print! ("\n");
}
