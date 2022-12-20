fn mix(s1: &str, s2: &str) -> String {
    let cnt1 = getcnt (s1);
    let cnt2 = getcnt (s2);
    let mut hist: Vec<String> = Vec::new();
    
    for c in 'a'..='z' {
        let c1 = s1.chars().filter(|&x| x == c).count();
        let c2 = s2.chars().filter(|&x| x == c).count();
        let freq = std::cmp::max(c1,c2);
        let mut line = String::new();

        if freq > 1 {
            match c1.cmp(&c2) {
                Less => line += &format!("1:"),
                Equal => line += &format!("=:"),
                Greater => line += &format!("2:"),
            }

            line += &String::from_utf8(vec![c as u8; freq]).unwrap();
            hist.push(line);
        }
    }

    hist.sort_by(|a,b| if a.len() == b.len() { a.cmp(b) } else { b.len().cmp(&a.len()) });
    hist.join("/")
}

fn main () {

}

