
fn sort (src: Vec<char>) -> String {
    let mut src = src;
    src.sort_by(|a,b| a.cmp(b));
    src.iter().collect::<String>()
}

fn getvar (src: &str) -> (i32, String) {
    let sig = if src.find('-') == None { 1 } else { -1 };
    let num = src.chars().filter(|x| x.is_digit(10)).collect::<String>().parse::<i32>();
    let var = sort(src.chars().filter(|x| x.is_alphabetic()).collect::<Vec<_>>());

    match num {
        Ok(dig) => (sig * dig, var),
        Err(_) => (sig, var), 
    }
}

fn simplify (mut expr: Vec<&str>) {
    let mut res = String::new();
    let mut i = 0;

   while i < expr.len() {
        let mut curr = getvar(expr[i]);
        let mut j = i + 1;
        //print!("curr : {} {} :: ", curr.0, curr.1);
        while j < expr.len() {
            let next = getvar(expr[j]);

            if next.1 == curr.1 {
                curr.0 += next.0;

                if j < expr.len() {
                    expr.remove(j);
                }
            }
            j += 1;
        }
        
        if curr.0 > 0 {
            res += "+";
        } 

        match curr.0 {
            0 => (),
           -1 => res += &format! ("{}", curr.1),
            1 => res += &format! ("{}", curr.1),
            _ => res += &format! ("{}{}", curr.0, curr.1), 
        }

        i += 1;
    }

   print!("{res}\n");
}
fn main() {

    let src = "-32zy";

    let expr = vec!["3x","-yx","+2yx","-x"];

    simplify (expr);

}
