fn lcs(x: &str, y: &str) -> String {

    let a = x.chars().collect::<Vec<_>>();
    let b = y.chars().collect::<Vec<_>>();

    let mut m = a.len();
    let mut n = b.len();
    let mut os = String::new();
    let mut score = vec![vec![0;n+1];m+1];

    for i in 0..=m {
        for j in 0..=n {
            if i == 0 || j == 0 {
                score[i][j] = 0;
            } else if a[i - 1] == b[j - 1] {
                score[i][j] = score[i - 1][j - 1] + 1;
            } else {
                score[i][j] = std::cmp::max(score[i - 1][j], score[i][j - 1]);
            }
        }
    }

    while m > 0 && n > 0 {
        if a[m - 1] == b[n - 1] {
            os.push(a[m - 1]);
            m -= 1; n -= 1;
        } else if score[m - 1][n] > score[m][n - 1] {
            m -= 1;
        } else {
            n -= 1;
        }
    }

    os.chars().rev().collect()
