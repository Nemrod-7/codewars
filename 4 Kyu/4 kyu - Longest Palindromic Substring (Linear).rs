fn split (src: &str) -> Vec<char> {

    let mut base:Vec<char> = Vec::new();

    for it in src.chars() {
        base.push(it);
        base.push('|');
    }
    base.pop();

    base
}
fn longest_palindrome(s: &str) -> String {

    let s:Vec<char> = split (s);
    let size:usize = s.len();

    let mut maxv:usize = 0;
    let mut ax:usize = 0;
    let mut base:Vec<String> = Vec::new();

    while maxv < size - ax {
        let mut beg:i32 = ax as i32;
        let mut end:i32 = ax as i32;

        while beg >= 0 && end < size as i32 {
            if s[beg as usize] != s[end as usize] { break }
            beg -= 1;
            end += 1;
        }

        let sub = &s[(beg + 1) as usize .. (end) as usize];

        let sub:String = sub.iter().filter(|&&x| x != '|').collect();
        if sub.len() > maxv { base.push (format! ("{sub}")) }
        maxv = std::cmp::max (maxv, sub.len());

        ax += 1;
    }

    for tok in base {
        if tok.len() == maxv { return tok; }
    }

    "".to_string()
}
