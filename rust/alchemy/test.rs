#![allow(dead_code, unused)]

macro_rules! say_hello {
    () => {
        println!("Hello!")
    };
}


 mod tests {
     use super::*;

     pub mod basics {

        pub fn construct() {

        }
    }

     pub fn jump() {

     }
}

fn main () {


    say_hello!();

    tests::basics::construct();

}
