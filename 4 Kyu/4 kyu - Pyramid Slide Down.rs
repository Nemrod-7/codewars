use std::cmp::max;

fn longest_slide_down(pyramid: &[Vec<u16>]) -> u16 {

    let mut res = pyramid.to_vec();
    let mut y = res.len() - 1;

    while y > 0 {
        y -= 1;
        for x in 0..res[y].len() {
            res[y][x] += max (res[y + 1][x], res[y + 1][x + 1]);
        }
    }

    res[0][0]
}
