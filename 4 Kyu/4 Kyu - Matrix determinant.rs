
fn determinant(mx: &[Vec<i64>]) -> i64 {

    match mx.len() {
        1 => return mx[0][0],
        2 => return mx[0][0] * mx[1][1] - mx[0][1] * mx[1][0],
        _ => {
            let mut det:i64 = 0;
            let mut sign:i64 = -1;

            for dig in 0..mx.len() {
                let mut nxt:Vec<Vec<i64>> = Vec::new();
                sign *= -1;

                for i in 0..mx.len() {
                    let mut ve:Vec<i64> = Vec::new();

                    for j in 0..mx.len() {

                        if i != 0 && j != dig { ve.push (mx[i][j]); }
                    }

                    if i != 0 { nxt.push (ve) }
                }
                det += sign * mx[0][dig] * determinant (&nxt);
            }
            det
        },
    }

}
