fn find_all (sum: u8, ndigs: u8) -> Option<(usize, u64, u64)> {

    if sum < ndigs || sum > ndigs * 9 { return None }

    let mut cnt: usize = 0;
    let mut min: u64 = core::u64::MAX;
    let mut max: u64 = core::u64::MIN;
    let mut comb:Vec<(u64, u8, u8)> = vec![(0,0,0)];

    while !comb.is_empty() {
        let vertex = comb.pop().unwrap();
        let curr = vertex.0;
        let total = vertex.1;
        let size = vertex.2;

        if total == sum && size == ndigs {
            cnt += 1;
            min = std::cmp::min (min, curr);
            max = std::cmp::max (max, curr);
        } else if total < sum && size < ndigs {
            for dig in curr % 10 .. 10 {
                if dig > 0 {
                    comb.push((curr * 10 + dig, total + dig as u8, size + 1));
                }
            }
        }
    }

    Some ((cnt,min,max))
}
