use std::collections::BTreeSet;

fn part (num:i64) -> String {

    let mut cluster:Vec<i64> = vec![0;num as usize];
    let mut set:BTreeSet<i64> = BTreeSet::new();

    let mut index:usize = 0;
    let mut stack:i64 = 0;

    cluster[index] = num + 1;

    while cluster[0] > 1 {

        let mut digit = cluster[index] - 1;
        let mut next = index;
        let mut rem = stack;

        while rem < num {
            if num - rem >= digit {
                if digit > 1 {
                    index = next;
                    stack = rem;
                }

                cluster[next] = digit;
                rem += digit;
                next += 1;
            } else {
                digit -= 1;
            }
        }

        set.insert (cluster.iter().filter(|&&x| x != 0).product::<i64>());

        if index > 0 && cluster[index] == 1 {
            index -= 1;
            stack -= cluster[index];
        }
    }

    let hist =  set.iter().map(|&x| x as f64).collect::<Vec<f64>>();
    let size = hist.len();

    let mean:f64 = hist.iter().sum::<f64>() / size as f64;
    let range = hist[size - 1] - hist[0];
    let median:f64;

    if size % 2 == 0 { median = (hist[size / 2] + hist[size / 2 - 1]) / 2.0 } else { median = hist[size / 2] }
    //print! ("Range: {range} Average: {mean} Median: {median}");

    format! ("Range: {range:.2} Average: {mean:.2} Median: {median:.2}")
}

fn main() {

    part (5); // "Range: 5 Average: 3.50 Median: 3.50"
    print!("\nend\n");
}

fn _mult (arr: &Vec<i64>, size: usize) -> i64 {

    let mut prod = 1;

    for i in 0..size {
        prod *= arr[i];
    }

    prod
}
