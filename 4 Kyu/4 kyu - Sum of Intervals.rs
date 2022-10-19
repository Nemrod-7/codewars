fn sum_intervals(intervals: &[(i32, i32)]) -> i32 {

    let mut intervals = intervals.iter().collect::<Vec<_>>();
    intervals.sort();

    let mut sum = 0;
    let mut maxv = intervals[0].0;

    for curr in intervals {
        if curr.1 > maxv {
            sum += curr.1 - std::cmp::max (curr.0, maxv);
            maxv = curr.1;
        }
    }

    sum
}
