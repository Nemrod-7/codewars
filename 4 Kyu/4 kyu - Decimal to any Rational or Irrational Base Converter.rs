
fn converter(mut n: f64, decimals: u8, base: f64) -> String { // Decimal to any Rational or Irrational Base Converter 
    if n == 0.0 {
        return "0.".to_string() + &(0..decimals).map(|_| "0").collect::<String>();
    }

    let alpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ".chars().collect::<Vec<_>>();
    let precision = -1.0 * (decimals + 1) as f64;
    let mut num = n.abs();
    let mut res = String::new();
    let k:f64 = f64::floor(num.log10() / base.log10()) + 1.0;
    let mut i = k - 1.0;

    while i > precision {
        if res.len() == k as usize { res += "."; }

        let exp = f64::powf(base,i);
        let dig = f64::floor((num / exp) % base);

        num -= dig * exp;
        res.push(alpha[dig as usize]);        

        i -= 1.0;
    }
    
    if n < 0.0 {
        res = format!("-{res}");
    } 

    res
}

fn main() {


    assert_eq!(converter(13.0, 0, std::f64::consts::PI), "103", "It should convert 13.0 into 103");
    assert_eq!(converter(10.0, 0, std::f64::consts::PI), "100", "It should convert 10.0 into 100");
    assert_eq!(converter(std::f64::consts::PI, 0, std::f64::consts::PI), "10", "It should convert pi into 10");

    assert_eq!(converter(13.0, 3, std::f64::consts::PI), "103.010", "It should convert 13.0 into 103.010 base pi");
    assert_eq!(converter(10.0, 2, std::f64::consts::PI), "100.01", "It should convert 10.0 into 103 base pi");
    assert_eq!(converter(std::f64::consts::PI, 5, std::f64::consts::PI), "10.00000", "It should convert pi into 10.0000 base pi");

    assert_eq!(converter(13.0, 0, 8.0), "15", "It should convert 13.0 into 15 base 8.0");
    assert_eq!(converter(10.0, 0, 16.0), "A", "It should convert 10.0 into A base 16.0");
    assert_eq!(converter(std::f64::consts::PI, 0, 2.0), "11", "It should convert pi into 11 base 2.0");

    assert_eq!(converter(7.0, 0, 19.0), "7", "It should convert 7.0 into 7 base 19.0");
    assert_eq!(converter(1.0, 0, 2.0), "1", "It should convert 1.0 into 1 base 2.0");
    assert_eq!(converter(13.5, 4, 16.0), "D.8000", "It should convert 13.5 into D.8000 base 16.0");

    assert_eq!(converter(10.5, 0, 16.0), "A", "It should convert 10.5 into A base 16.0");
    assert_eq!(converter(1.0, 2, 2.0), "1.00", "It should convert 1.0 into 1.00 base 2.0");
    assert_eq!(converter(-10.0, 0, 23.0), "-A", "It should convert -10.0 into -A base 23.0");
    assert_eq!(converter(0.0, 4, 26.0), "0.0000", "It should convert 0.0 into 0.0000 base 26.0");
    assert_eq!(converter(-15.5, 2, 23.0), "-F.BB", "It should convert -15.5 into -F.BB base 23.0");

    assert_eq!(converter(13.0, 0, 10.0), "13", "It should keep 13.0 into 13 base 10.0");
    assert_eq!(converter(10.0, 0, 10.0), "10", "It should keep 10.0 into 10 base 10.0");
    assert_eq!(converter(5.5, 1, 10.0), "5.5", "It should keep 5.5 into 5.5 base 10.0");


    /*
    */


}

