
fn next_bigger_number(n: i64) -> i64 {

    let mut num = n.to_string().chars().collect::<Vec<_>>();
    let len = num.len();

    let mut up = len;
    let mut pos = len - 1;

    loop {
        pos -= 1;
        if num[pos] < num[pos + 1] { break }
        if pos == 0 { return -1 }
    }

    while up > 0 {
        up -= 1;
        if num[up] > num[pos] { break }
    }

    num.swap(up,pos);

    for i in pos+1 ..len {
        for j in i+1 ..len {
            if num[i] > num[j] {
                num.swap(i,j)
            }
        }
    }

    num.iter().collect::<String>().parse::<i64>().unwrap()
}
