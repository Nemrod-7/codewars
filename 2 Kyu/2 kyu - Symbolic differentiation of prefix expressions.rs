
extern crate regex;
use regex::Regex;

fn calc (a: &str, ope: &str, b: &str) -> String {

    let num = Regex::new("^-?[0-9]+$").unwrap();

    let mut flag = false;
    let mut aa = 0.0;
    let mut bb = 0.0;

    if num.is_match (a) && num.is_match (b) {
        flag = true;
        aa = a.parse::<f64>().unwrap();
        bb = b.parse::<f64>().unwrap();
    }

    match ope {
        "+" => {
            if flag == true { return (aa + bb).to_string() }
            if a == b { return "(* 2 ".to_string() + b + ")"; }
            if a == "0" { return b.to_string() };
            if b == "0" { return a.to_string() };
        },
        "-" => {
            if flag == true { return (aa - bb).to_string() }
            if a == b { return "0".to_string() }
            if b == "0" { return a.to_string() }

        },
        "*" => {
            if flag == true { return (aa * bb).to_string() }
            if a == b { return "(^ 2 ".to_string() + b + ")"; }
            if a == "0" || b == "0" { return "0".to_string() }
            if a == "1" { return b.to_string() };
            if b == "1" { return a.to_string() };
        },
        "/" => {
            if flag == true { return (aa / bb).to_string() }
            if a == b { return "1".to_string(); }
            if b == "1" { return a.to_string() };
        },
        "^" => {
            if flag == true { return ((aa as u32).pow (bb as u32)).to_string() }
            if a == "1" || b == "1" { return a.to_string() }
            if a == "0" { return a.to_string() }
            if b == "0" { return "1".to_string() }
        },
         _  => (),
    }

    return "(".to_string() + ope + " " + a + " " + b + ")";
}
fn simpl (expr: &str) -> String {
    let tok = Regex::new ("\\w+|-?[0-9]+|(\\(.*?\\))|[*/^+-]").unwrap();
    // print! (":{}:\n", expr);
    if expr.len() > 2 {
        let line = tok.captures_iter(&expr[1..]).collect::<Vec<_>>();

        if line.len() == 3 {
            return calc (&line[1][0], &line[0][0], &line[2][0]);
        }
    }
    expr.to_string()
}
fn diff (expr: &str) -> String {

    let cst = Regex::new("^-?[0-9]+$").unwrap();
    let tok = Regex::new ("\\w+|-?[0-9]+|(\\(.*?\\))|[*/^+-]").unwrap();

    if expr == "x" { return "1".to_string() }
    if cst.is_match (expr) { return "0".to_string() }

    let line = tok.captures_iter(&expr[1..]).collect::<Vec<_>>();

    let ope = &line[0][0];
    let ag1 = simpl (&line[1][0]);
    let dx1 = diff (&ag1);
    let mut ag2 = String::new();
    let mut dx2 = String::new();

    if line.len() == 3 {
        ag2 = simpl (&line[2][0]);
        dx2 = diff (&ag2);
    }

    match ope {
        "+" => {
            return calc (&dx1, ope, &dx2);
        },
        "-" => {
            return calc (&dx1, ope, &dx2);
        },
        "*" => {
            let a = calc (&dx1, ope, &ag2);
            let b = calc (&ag1, ope, &dx2);
            return calc (&a, "+", &b);
        },
        "/" => {
            let a = calc (&dx1, "*", &ag2);
            let b = calc (&dx2, "*", &ag1);
            let nom = calc (&a, "-", &b);
            let den = calc (&ag2, "^", "2");

            return calc (&nom, "/", &den);
        },
        "^" => {
            let ag3 = calc (&ag1, "^", &calc (&ag2, "-", "1"));

            return calc (&dx1, "*", &calc (&ag2,"*", &ag3));
        },
        "ln" => {

            return calc ("1", "/", &ag1);
        },
        "cos" => {
            let exp = "-".to_string() + &dx1;
            dx2 = "(sin ".to_string() + &ag1 + ")";
            return calc (&exp, "*", &dx2);
        },
        "sin" => {
            dx2 = "(cos ".to_string() + &ag1 + ")";
            return calc (&dx1, "*", &dx2);
        },
        "exp" => {
            dx2 = "(exp ".to_string() + &ag1 + ")";
            return calc (&dx1, "*", &dx2);
        },
        "tan" => {
            let ag2 = "(cos ".to_string() + &ag1 + ")";
            let ag3 = calc (&ag2, "^", "2");
            let frac = calc ("1", "/", &ag3);

            if dx1 == "1" {
                return frac;
            } else {
                return calc (&dx1, "*", &frac);
            }
        },
          _  => return "".to_string(),
    }
}

fn main() {


    // diff ("(^ (sin x) 3)");
    // expected (* (cos x) (* 3 (^ (sin x) 2))) or (* (* 3 (^ (sin x) 2)) (cos x)) at src/lib.rs:206:9
    assert_eq!(diff("(exp (* 1 x))"), "(exp x)");
    assert_eq!(diff("(^ x 0)"), "0");
    assert_eq!(diff("(^ x 1)"), "1");
    assert_eq!(diff("(^ (cos x) 0)"), "0");
    assert_eq!(diff("(^ x 0)"), "0");
    assert_eq!(diff("(^ x 3)"), "(* 3 (^ x 2))");

    assert_eq!(diff("(^ (cos x) 1)"), "(* -1 (sin x))");

    assert_eq!(diff("x"), "1");
    assert_eq!(diff("5"), "0");
    assert_eq!(diff("(+ x x)"), "2");
    assert_eq!(diff("(- x x)"), "0");
    assert_eq!(diff("(* x x)"), "(* 2 x)");
    assert_eq!(diff("(+ (sin x) (sin x))"), "(* 2 (cos x))");

    assert_eq!(diff("(* x 2)"), "2");
    assert_eq!(diff("(/ x 2)"), "0.5");
    assert_eq!(diff("(^ x 2)"), "(* 2 x)");
    assert_eq!(diff("(sin x)"), "(cos x)");
    assert_eq!(diff("(cos x)"), "(* -1 (sin x))");

    assert_eq!(diff("(+ x (+ x x))"), "3");
    assert_eq!(diff("(- (+ x x) x)"), "1");
    assert_eq!(diff("(* 2 (+ x 2))"), "2");
    assert_eq!(diff ("(+ (* 85 x) -19)"), "85");
    assert_eq!(diff("(/ 2 (+ 1 x))"), "(/ -2 (^ (+ 1 x) 2))");

    assert_eq!(diff("(ln x)"), "(/ 1 x)");
    assert_eq!(diff("(exp x)"), "(exp x)");
    assert_eq!(diff("(cos (+ x 1))"), "(* -1 (sin (+ x 1)))");
    assert_eq!(diff("(sin (+ x 1))"), "(cos (+ x 1))");
    assert_eq!(diff("(sin (* 2 x))"), "(* 2 (cos (* 2 x)))");

    assert_eq!(diff("(exp (* 2 x))"), "(* 2 (exp (* 2 x)))");
    assert_eq!(diff(&diff("(sin x)")), "(* -1 (sin x))");
    assert_eq!(diff(&diff("(exp x)")), "(exp x)");



    /*
{
    let result = diff(&diff("(^ x 3)"));
    assert!(result == "(* 3 (* 2 x))" || result == "(* 6 x)", "expected (* 3 (* 2 x)) or (* 6 x)");
}

{
    let result = diff("(cos (* 2 x))");
    assert!(
        result == "(* 2 (* -1 (sin (* 2 x))))"
            || result == "(* -2 (sin (* 2 x)))"
            || result == "(* (* -1 (sin (* 2 x))) 2)",
        "expected (* 2 (* -1 (sin (* 2 x)))) or (* -2 (sin (* 2 x))) or (* (* -1 (sin (* 2 x))) 2)"
    );
}

{
    let result = diff("(tan x)");

    assert!(
        result == "(+ 1 (^ (tan x) 2))" || result == "(/ 1 (^ (cos x) 2))",
        "expected (+ 1 (^ (tan x) 2)) or (/ 1 (^ (cos x) 2))"
    );

}

{
    let result = diff("(tan (* 2 x))");

    //print! ("{}", result);

    assert!(
           result == "(* 2 (+ 1 (^ (tan (* 2 x)) 2)))"
        || result == "(* 2 (/ 1 (^ (cos (* 2 x)) 2)))",

        "expected (* 2 (+ 1 (^ (tan (* 2 x)) 2))) or (* 2 (/ 1 (^ (cos (* 2 x)) 2)))"
    );

}

*/
    print!("\nend\n");
}
