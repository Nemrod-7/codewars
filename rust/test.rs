
#![allow(dead_code, unused)]

use std::collections::HashMap;

fn main() {

    let mut list:HashMap<String, Vec<i32>> = HashMap::new();
    
    let num = 1;

    match list.get_mut("x") {
        Some(x) => x.push(num),
        None => { list.insert ("x".to_string(),vec![num]); } ,
    }

    /*
       for it in list {

       print!("{:?}\n", it);
       }

       print!("\n");
       */
    //show(getvar(t1));
}
