#![allow(unused)]
use std::io::Write;
use std::{thread, time};

fn clear_console() {
    // The ANSI escape code for clearing the terminal screen
    print!("\x1B[2J\x1B[1;1H");
    std::io::stdout().flush().unwrap();
}
fn show_track(track: &Vec<Vec<char>>, a_train: &Train, b_train: &Train) -> String {
    let wa = &a_train.2;
    let wb = &b_train.2;
    let mut os = String::new();

    for y in 0..track.len() {
        for x in 0..track[y].len() {
            let la = wa.iter().filter(|&&p| p == (x as i32,y as i32)).collect::<Vec<_>>();
            let lb = wb.iter().filter(|&&p| p == (x as i32,y as i32)).collect::<Vec<_>>();

            if wa.front().unwrap() == &(x as i32,y as i32) {
                os.push(a_train.0);
            } else if wb.front().unwrap() == &(x as i32,y as i32) {
                os.push(b_train.0);
            } else if la.len() != 0 {
                os.push(a_train.0.to_ascii_lowercase());
            } else if lb.len() != 0 {
                os.push(b_train.0.to_ascii_lowercase());
            } else {
                os.push(track[y][x]);
            }
        }
        os.push('\n');
    }

    os
}
fn animation(rail: &Vec<Vec<char>>, a_train: &mut Train, b_train: &mut Train) {
    let ten = time::Duration::from_millis(100);
    let mut wait_a = a_train.1;
    let mut wait_b = b_train.1;

    loop {

        advance(&rail, a_train, &mut wait_a);
        advance(&rail, b_train, &mut wait_b);

        print!("{}", show_track(&rail, &a_train, &b_train) );
        std::io::stdout().flush().unwrap();
        thread::sleep(ten);
    }
}

use std::collections::LinkedList;
const BASE:[&str;4] = ["|+S","/XS", "-S", "\\XS"];
const DIRECT:[(i32,i32);8] = [(0,-1),(1,-1),(1,0),(1,1),(0,1),(-1,1),(-1,0),(-1,-1)];

type Train = (char,usize, LinkedList<(i32,i32)>);

fn add (a: &(i32,i32), b: &(i32,i32)) -> (i32,i32) { (a.0 + b.0, a.1 + b.1) }

fn is_inside(p: &(i32,i32), track: &Vec<Vec<char>>) -> bool {
    return p.0 >= 0 && p.1 >= 0 && p.1 < track.len() as i32 && p.0 < track[p.1 as usize].len() as i32;
}
fn get_origin(track: &Vec<Vec<char>>) -> (i32, i32) {

    for y in 0..track.len() {
        for x in 0..track[y].len() {
            if track[y][x] != ' ' {
                return (x as i32 , y as i32);
            }
        }
    }

    (0,0)
}

fn getcell(track : &Vec<Vec<char>>, prev: &(i32,i32), curr: &(i32,i32), index :usize) -> bool {
    let next = add( curr, &DIRECT[index]);
    let index = index % 4;

    if !is_inside( &next, &track) { return false; }
    if next == *prev { return false; }
    let rail = track[curr.1 as usize][curr.0 as usize];
    let last = track[prev.1 as usize][prev.0 as usize];
    let frwd = track[next.1 as usize][next.0 as usize];

    match rail {
        '-' => if index == 2 { return true; },
        '|' => if index == 0 { return true; },
        'S' => if last == frwd { return true; }
        'X' => if last == frwd { return true; }
        '+' => if (last == '-' && frwd == '-') || (last != '-' && index == 0) { return true; },
        _ => return BASE[index].find(frwd).is_some(),
    }

    false
}

fn move_wagon (track: &Vec<Vec<char>>, wagon: &mut LinkedList<(i32,i32)>) {
    let curr = *wagon.iter().nth(0).unwrap();
    let prev = *wagon.iter().nth(1).unwrap();

    for i in 0..8 {
        if getcell(track, &prev, &curr, i) {
            wagon.push_front( add( wagon.front().unwrap(), &DIRECT[i]));
            wagon.pop_back();
        }
    }
}

fn getstart(track: &Vec<Vec<char>>, pos:usize) -> (i32,i32) {
    let origin = get_origin(track);
    let mut curr = LinkedList::from([origin]);

    for i in 4..8 {
        let nxt = add( &origin, &DIRECT[i] );

        if is_inside( &nxt, &track) {
            if getcell(&track, &origin, &origin, i) {
                curr.push_back( nxt );
            }
        }
    }

    for _ in 0..pos {
        move_wagon(&track, &mut curr);
    }

    *curr.front().unwrap()
}
fn mk_train(track: &Vec<Vec<char>>, train: &str, pos:usize)-> Train {
    let ch = train.chars().next().unwrap();
    let origin = get_origin(track);

    let (mut id, size, mut wagon) = (ch, train.len(), LinkedList::from([origin]) );
    let (fst,scd) = if ch.is_uppercase() { (0,3) } else { (4,8) };

    if !ch.is_uppercase() {
        id = train.chars().last().unwrap();
    }

    for i in fst..scd {
        let p = *wagon.back().unwrap();
        let (sx,sy) = add(&p, &DIRECT[i]);

        if is_inside( &(sx,sy), &track) {
            if BASE[i % 4].find( track[sy as usize][sx as usize] ).is_some() {
                wagon.push_back( (sx, sy) );
            }
        }
    }

    while wagon.len() < size {
        for i in 0..8 {
            let curr = wagon.back().unwrap() ;
            let prev = wagon.iter().rev().nth(1).unwrap();

            if getcell(&track, prev, curr, i) && wagon.len() < size {
                wagon.push_back( add( curr, &DIRECT[i] ) );
            }
        }
    }

    if ch.is_uppercase() {
        let dest = getstart(&track, pos);

        while &dest != wagon.front().unwrap() {
            move_wagon(&track, &mut wagon);
        }
    } else {
        for _ in 0..pos {
            move_wagon(&track, &mut wagon);
        }
    }

    (id, size, wagon)
}
fn collision(a: &Train, b: &Train) -> bool {

    let loco_a = a.2.front().unwrap();
    let loco_b = b.2.front().unwrap();
    // print!("{:?}, {:?}\n", a, b);
    for tr in b.2.iter() {
        if loco_a == tr { return true; }
    }

    for tr in a.2.iter() {
        if loco_b == tr { return true; }
    }

    false
}

fn advance(track: &Vec<Vec<char>>, train: &mut Train, wait: &mut usize) {

    match train.0 {
        'X' => move_wagon(&track, &mut train.2),
        _ => {
            let a = train.2.front().unwrap();
            let (ax,ay) = (a.0 as usize, a.1 as usize);

            if track[ay][ax] == 'S' && *wait < (train.1 - 1)  {
                *wait += 1
            } else {
                if *wait > 0 { *wait = 0 }
                move_wagon(&track, &mut train.2);
            }
        },
    }
}
pub fn train_crash( track: &str, a_train: &str, a_pos: usize, b_train: &str, b_pos: usize, limit: usize,) -> Option<usize> {
    print!("\n{}, \"{}\", {}, \"{}\", {}, {}\n", track, a_train, a_pos, b_train, b_pos, limit);
    let rail = track
        .lines()
        .map(|line| line.trim_end_matches(' ').chars().map(|x| x as char).collect::<Vec<_>>() )
        .collect::<Vec<Vec<char>>>() ;

    let mut a_train = mk_train(&rail, &a_train, a_pos);
    let mut b_train = mk_train(&rail, &b_train, b_pos);
    let [mut wait_a, mut wait_b] = [a_train.1, b_train.1];

    if collision(&a_train, &b_train) { return Some(0); }
    print!("{}", show_track(&rail, &a_train, &b_train));

    for cnt in 0..limit {
        advance(&rail, &mut a_train, &mut wait_a);
        advance(&rail, &mut b_train, &mut wait_b);

        print!("{}", show_track(&rail, &a_train, &b_train));
        std::io::stdout().flush().unwrap();
        thread::sleep( time::Duration::from_millis(100) );

        if collision(&a_train, &b_train) { return Some(cnt + 1); }
    }

    // print!("{}", show_track(&rail, &a_train, &b_train));
    // animation(&rail, &mut a_train, &mut b_train);
    None
}

fn main() {


    const TRACK_EX: &str =
        "                                /------------\\
/-------------\\                /             |
|             |               /              S
|             |              /               |
|        /----+--------------+------\\        |
\\       /     |              |      |        |
 \\      |     \\              |      |        |
 |      |      \\-------------+------+--------+---\\
 |      |                    |      |        |   |
 \\------+--------------------+------/        /   |
        |                    |              /    |
        \\------S-------------+-------------/     |
                             |                   |
/-------------\\              |                   |
|             |              |             /-----+----\\
|             |              |             |     |     \\
\\-------------+--------------+-----S-------+-----/      \\
              |              |             |             \\
              |              |             |             |
              |              \\-------------+-------------/
              |                            |
              \\----------------------------/
              ";
    // let res = train_crash(TRACK_EX, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);
    // Some(516);
const LOOP: &str =
"/-----------------\\
|                 |
|                 |
|                 |
|                 |
\\---------S-------/";

// left: `Some(1)`,
// right: `Some(0)`:

const EIGHT: &str =
"/-------\\
|       |
|       |
|       |
\\-------+--------\\
        |        |
        |        |
        |        |
        \\--------/" ;

const GRAND: &str =
"/-------\\
|       |
|       |
\\-------+-------------------------------------------------------------------\\
        |                                                                   |
        |                                                                   |
        \\-------------------------------------------------------------------/";

const FOUR8: &str =
"/-----\\   /-----\\   /-----\\   /-----\\
|      \\ /       \\ /       \\ /      |
|       X         X         X       |
|      / \\       / \\       / \\      |
\\-----/   \\-----/   \\-----/   \\-----/";

let res = train_crash(GRAND, "aA", 10, "oooooooooooooooooooooooooO", 70, 200);
// left: `Some(27)`,
// right: `None`:
// Your result (left) did not match the expected output (right)

// let res = train_crash(GRAND, "aA", 10, "oooooooooooooooooooooooooO", 70, 200);


// let res = train_crash(LOOP, "xX", 15, "Zzzzzzzzzzzzzz", 40, 100);
print!("{:?}\n", res);

// /-----------------\
// |                 |
// |                 |
// |                 |
// |                 |
// \-----------------/,
//

print!("\nend\n");
}
