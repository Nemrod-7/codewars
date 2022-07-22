fn decompose(n: i64) -> Option<Vec<i64>> {

    let mut n2:i64 = 0;
    let mut vs = vec![n];

    loop  {
        let mut num = *vs.last().unwrap();
        vs.pop();
        n2 += num * num;

        while num > 1 {
            num -= 1;
            if num * num <= n2 {
                vs.push (num);
                n2 -= num * num;
            }
        }

        if n2 == 0 || vs.len() == 0 { break }
    }

    vs.sort();

    if vs.len() == 0 { return None }
    Some (vs)
}
