use std::collections::BTreeSet;

fn part (num:i64) -> String {

    let mut cluster:Vec<i64> = vec![0;num as usize];
    let mut set:BTreeSet<i64> = BTreeSet::new();

    let mut index:usize = 0;
    let mut prev:i64 = 0;

    cluster[index] = num + 1;

    while cluster[0] > 1 {

        let mut digit = cluster[index] - 1;
        let mut next = index;
        let mut rem = prev;

        while rem < num {
            if num - rem >= digit {
                if digit > 1 {
                    index = next;
                    prev = rem;
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
            prev -= cluster[index];
        }
    }

    let hist =  set.iter().map(|&x| x as f64).collect::<Vec<f64>>();
    let size = hist.len();

    let mean:f64 = hist.iter().sum::<f64>() / size as f64;
    let range = hist[size - 1] - hist[0];
    let median:f64;

    if size % 2 == 0 { median = (hist[size / 2] + hist[size / 2 - 1]) / 2.0 } else { median = hist[size / 2] }

    format! ("Range: {range} Average: {mean:.2} Median: {median:.2}")
}
