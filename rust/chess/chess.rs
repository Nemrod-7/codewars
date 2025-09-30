#![allow(dead_code, unused)]
use std::collections::HashMap;
// use rand;

fn display(player : &WhitePlayer) {

    let white_piece = HashMap::from([ ('K', '♚'), ('R', '♜') ]);
    let black_piece = HashMap::from([ ('K', '♔'), ('R', '♖') ]);
    let mut grid = vec![vec![' '; 8]; 8];

    for i in 0..player.black.len() {
        let (id,x,y) = player.black[i];
        grid[y as usize][x as usize] = black_piece[&id];
    }
    for i in 0..player.white.len() {
        let (id,x,y) = player.white[i];
        grid[y as usize][x as usize] = white_piece[&id];
    }

    print!("\n");

    for i in 0..8 {
        for j in 0..8 {
            let cell = grid[i][j];

            if cell != ' ' {
                print!(" {} ", cell);
            } else {
                if (i + j) % 2 == 0 {
                    print!(" . ");
                } else {
                    print!("   ");
                }
            }
        }
        print!("\n");
    }
}

struct Heuristic { }

impl Heuristic {
    fn rook() -> [i8; 64] {
        [
             0,  0,  0,  0,  0,  0,  0,  0,
             5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
             0,  0,  0,  5,  5,  0,  0,  0

        ]
    }
    fn king() -> [i8; 64] {
        [
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -20,-30,-30,-40,-40,-30,-30,-20,
            -10,-20,-20,-20,-20,-20,-20,-10,
             20, 20,  0,  0,  0,  0, 20, 20,
             20, 30, 10,  0,  0, 10, 30, 20
        ]
    }

}

fn is_inside (x:i8, y:i8) -> bool { x >= 0 && y >=0 && x < 8 && y < 8 }

fn identify(piece:&str) -> (char,u8,u8) {
    let piece :Vec<_> = piece.chars().collect();
    let id = piece[0];
    let x = piece[1] as u8 - 97;
    let y = piece[2] as u8 - 49;

    (id,x,y)
}

struct WhitePlayer {
    white : Vec<(char,u8,u8)>,
    black : Vec<(char,u8,u8)>,
}
fn get_score(ch:char) -> i8 {
    match ch {
        'R' => 5,
        'Q' => 8,
        'K' => 99,
         _ => 0,
    }

}

impl WhitePlayer {
    fn new (txt: &str) -> WhitePlayer {
        let white :Vec<_> = txt[0..7].split(",").collect::<Vec<_>>();
        // let white :Vec<(char,u8,u8)> = white.iter().map(|x| identify(x)).collect();
        // let black :Vec<(char,u8,u8)> = vec![identify(&txt[10..])];
        // let heur = Heuristic::new();

        WhitePlayer {
            white : white.iter().map(|x| identify(x)).collect(),
            black : vec![identify(&txt[10..])],
        }
    }

    fn get_moves(&self) -> Vec<(char,usize,usize)> {
        let cross = vec![(0,1),(0,-1),(1,0),(-1,0)];
        let diag = vec![(-1,-1),(-1,1),(1,1),(1,-1)];
        let comp = vec![(0,1),(0,-1),(1,0),(-1,0),(-1,-1),(-1,1),(1,1),(1,-1)];

        let mut moves :Vec< (char,usize,usize) > = vec![];

        for i in 0..self.white.len() {
            let (id,x,y) = self.white[i];
            let curr = (y * 8 + x) as usize;
            let tyo;

            match id {
                'K' => tyo = (id, 1, &comp, Heuristic::king()),
                'R' => tyo = (id, 8, &cross, Heuristic::rook()),
                 _  => tyo = (id, 0, &cross, Heuristic::rook()),
            }

            let (id, mode, direction, heuristic) = tyo;

            for (dx, dy) in direction {
                for dist in 1..=mode {
                    let nx = x as i8 + dx * dist;
                    let ny = y as i8 + dy * dist;

                    if is_inside(nx, ny) {
                        let next = ( ny as u8 * 8 + nx as u8) as usize ;
                        moves.push( (id, curr, next) );
                    } 

                }
            }
        }

        moves
    }

    fn play (&mut self,txt : &str) {

        let moves = self.get_moves();

        self.black = vec![identify(&txt)];

        let bx = self.black[0].1;
        let by = self.black[0].2;

        // print!("{:?} ", self.black);
        for (id,curr,next) in moves.iter() {
            let sx = curr % 8;
            let sy = curr / 8;
            let nx = next % 8;
            let ny = next / 8;
            let mut heur = 0;
            let notation = format!("{}{}{}", id, (nx as u8 + 97) as char, ny as u8 + 1);

            if *id == 'R' {
                heur = Heuristic::rook()[*next] - Heuristic::rook()[*curr];
            }

            if *id == 'K' {
                heur = Heuristic::king()[*next] - Heuristic::king()[*curr];

            }

            print!("{notation} {heur}\n");

        }


    }

}


fn main() {


    let mut white_player = WhitePlayer::new("Ke8,Rh7 - Kc8");
    let mut white_move = white_player.play("Kb8");


    display(&white_player);

    //
}
