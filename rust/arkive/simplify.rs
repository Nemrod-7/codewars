
fn getvar (src: &str) -> (i32, String) {
    let sig = if src.find('-') == None { 1 } else { -1 };
    let num = src.chars().filter(|x| x.is_digit(10)).collect::<String>().parse::<i32>();
    let mut var = src.chars().filter(|x| x.is_alphabetic()).collect::<Vec<_>>();
    var.sort_unstable();
    let var = var.iter().collect::<String>();

    match num {
        Ok(dig) => (sig * dig, var),
        Err(_) => (sig, var), 
    }
}
fn format (args: Vec<(i32,String)>) -> String {
    let mut res = String::new();

    for i in 0..args.len() {
        if args[i].0 > 0 && res.len() > 0 {
            res += "+";
        }

        match args[i].0 {
            0 => (),
           -1 => res += &format! ("-{}", args[i].1),
            1 => res += &format! ("{}", args[i].1),
            _ => res += &format! ("{}{}", args[i].0, args[i].1), 
        }
    }

    res
}
fn simplify (poly: &str) -> String {
    let poly = poly.replace("+", ",+").replace("-", ",-");
    let mut args = poly.split(',').map(|x| getvar(x)).collect::<Vec<_>>();
    let mut i = 0;

    while i < args.len() {
        let mut j = i + 1;

        while j < args.len() {
            if args[i].1 == args[j].1 {
                args[i].0 += args[j].0;
                args.remove(j);
            } else {
                j += 1;
            }
        }
        i += 1;
    }
    
    args.sort_by(
        |a,b| 
        if a.1.len() != b.1.len() {
            a.1.len().cmp(&b.1.len())
        } else if a.1 != b.1 { 
            a.1.cmp(&b.1)
        } else { 
            b.0.cmp(&a.0)
        }
    );

    format(args)
}


fn dotest(polynomial: &str, expected: &str) {
    let actual = simplify(polynomial);
    assert!(actual == expected,
            "Test failed with polynomial = \"{polynomial}\"\nExpected \"{expected}\" but got \"{actual}\"");
}

fn sample_tests() {
    dotest("dc+dcba", "cd+abcd");
    dotest("2xy-yx", "xy");
    dotest("-a+5ab+3a-c-2a", "-c+5ab");
    dotest("-abc+3a+2ac", "3a+2ac-abc");
    dotest("xyz-xz", "-xz+xyz");
    dotest("a+ca-ab", "a-ab+ac");
    dotest("xzy+zby", "byz+xyz");
    dotest("-y+x", "x-y");
    dotest("y-x", "-x+y");
    dotest("3a+b+4ac+bc-ab+3a-cb-a-a", "4a+b-ab+4ac");
    dotest("+n-5hn+7tjhn-4nh-3n-6hnjt+2jhn+9hn", "-2n+2hjn+hjnt");
    dotest("-8fk+5kv-4yk+7kf-qk+yqv-3vqy+4ky+4kf+yvqkf", "3fk-kq+5kv-2qvy+fkqvy");
    dotest("-15cb-12cb-0c+7cb", "-20bc");
    dotest("-12dy+9yzd-9dyz-13y+8y-10yzd-11yd+15yd+9y", "4y-8dy-10dyz");
    dotest("+11x+11x+0xd-12x+5adx+4xd", "10x+4dx+5adx");
    dotest("-0axz-0xz+0axz+0x+4xaz+14x+14zax", "14x+18axz");

    /*
    */

}

fn main () {

    sample_tests();
}
