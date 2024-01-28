#![allow(dead_code, unused)]

use std::collections::HashMap;

fn tokenize(src: &str) -> Vec<(String,i32)> {
    let mut vc:Vec<(String,i32)> = Vec::new();

    for ch in src.chars() {
        if ch.is_lowercase() {
            if vc.len() > 0 {
                let size = vc.len() - 1;
                &vc[size].0.push(ch);
            }
        } else {
            vc.push((ch.to_string(), 1));
        }
    }

    vc
}

fn parse_molecule (src: &str) -> Vec<(String, i32)> {

    let mut code = src.chars().map(|x| (x.to_string(), 1)).collect::<Vec<_>>();
    let mut map: HashMap<String,i32> = HashMap::new(); 
    let mut index = 0;

    while index != code.len() {
        let cell = &code[index].0;

        if let Ok(num) = cell.parse::<i32>() {
            let mut it = index - 1;
            let mut search:String = String::new();
            let last = format!("{}", code[it].0);

            match &code[it].0 as &str {
                ")" => search = format!("("),
                "]" => search = "[".to_string(),
                _ => search = code[it].0.clone(),
            }

            loop {
                code[it].1 *= num;

                if code[it].0 == search { break }
                it -= 1;
            }
        }

        index += 1;
    }

    for it in code {
        if it.0.chars().all(|x| x.is_alphabetic()) {

            if map.contains_key(&it.0) ==  false {
                map.insert(it.0, it.1);
            } else {
                *map.entry(it.0.to_owned()).or_default() += it.1;
            }
        }
    }

    //print!("{:?}\n", map);
    map.iter().map(|x| (x.0.clone(), x.1.clone())).collect::<Vec<_>>()
}

fn main () {

    let res = parse_molecule("K4[ON(SO3)2]2"); 

    let src = "On4H5";


}
