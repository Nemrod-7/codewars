
fn main() {

    let src = "asg(jfh()hyhd(";
    let mut cnt = 0;

    for ch in src.chars() {
        cnt += if ch == '(' { 1 } else if ch == ')' { -1 } else { 0 };
    }
}
