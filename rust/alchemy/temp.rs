
macro_rules! assert_delta {
    ($x:expr, $y:expr, $eq1:ident <= $eps:expr, $txt:expr) => {
        print!("{} eq = {}", $txt, $eq1);

        if (($x - $y) as f64).abs() > $eps {
            print!("\nexpected : {} got : {}\n", $x, $y);
        }

        print!("\n");
    };
}

fn main () {

    let result = 16.;
    let actual = 16.9;   

    assert_delta!(16.0, actual, abs <= 0.00001,"Testing molecular weight");

    //    assert_float_eq!(16., methane.molecular_weight().unwrap(), abs <= 0.00001, "Testing molecular weight");

}
