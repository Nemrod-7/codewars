#![allow(dead_code, unused)]

const WHITE:usize = 0;
const BLACK:usize = 1;

const KING:usize = 0;
const ROOK:usize = 1;

const HEUR: [[i32;64];2] = [
        [
            -50,-40,-30,-20,-20,-30,-40,-50,
            -30,-20,-10,  0,  0,-10,-20,-30,
            -30,-10, 20, 30, 30, 20,-10,-30,
            -30,-10, 30, 40, 40, 30,-10,-30,
            -30,-10, 30, 40, 40, 30,-10,-30,
            -30,-10, 20, 30, 30, 20,-10,-30,
            -30,-30,  0,  0,  0,  0,-30,-30,
            -50,-30,-30,-30,-30,-30,-30,-50
        ] , [
             0,  0,  0,  0,  0,  0,  0,  0,
             5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
             0,  0,  0,  5,  5,  0,  0,  0
        ]
];

fn identify(piece:char) -> usize {
    match piece {
        'K' => KING,
        'R' => ROOK,
         _ => 2,
    }

}
fn notation(piece:&str) -> (char, u64) {
    let piece :Vec<_> = piece.chars().collect();
    let id = piece[0] ;
    let x = piece[1] as u64- 97;
    let y = piece[2] as u64 - 49;

    (id, x  + (7 - y) * 8)
}
fn is_inside (x:i8, y:i8) -> bool { x >= 0 && y >=0 && x < 8 && y < 8 }

fn to_notation(id: char, pos:u64 ) -> String {
    let nx = pos % 8;
    let ny = pos / 8;

    format!("{}{}{}", id, (nx as u8 + 97) as char,  (8 - ny) as u8)
}
fn get_score(ch:char) -> i8 {
    match ch {
        'R' => 5,
        'K' => 10,
        _ => 0,
    }
}

struct WhitePlayer {
    piece : [char;64],
    board : [u64;2],
}

impl WhitePlayer {
    fn new (txt: &str) -> WhitePlayer {
        let white :Vec<_> = txt[0..7].split(",").collect::<Vec<_>>();

        let mut piece = [' '; 64];
        let mut board = [0u64,0u64];

        let (id, position) = notation(&txt[10..]);
        piece[position as usize] = id;
        board[BLACK] |= 1u64 << position;

        for (id, position) in white.iter().map(|x| notation(x)).collect::<Vec<_>>() {
            piece[position as usize] = id;
            board[WHITE] |= 1u64 << position;
        }

        WhitePlayer {
            piece : piece,
            board : board,
        }
    }
    fn get_moves(&self) -> Vec<(char,u64,u64)> {
        let cross = vec![(0,1),(0,-1),(1,0),(-1,0)];
        let diag = vec![(-1,-1),(-1,1),(1,1),(1,-1)];
        let comp = vec![(0,1),(0,-1),(1,0),(-1,0),(-1,-1),(-1,1),(1,1),(1,-1)];

        let mut curr = 0;
        let mut position = self.board[WHITE];
        let mut moves :Vec<(char,u64,u64)> = vec![];

        while position > 0 {
            if position &1 == 1 {

                let id = self.piece[curr];
                let (x,y) = (curr % 8, curr / 8);
                let (dist, direction) = match id {
                    'K' => (1, &comp),
                    'R' => (8, &cross),
                    _  =>  (0, &cross),
                };

                for (dx, dy) in direction {
                    for dist in 1..=dist {
                        let nx = x as i8 + dx * dist;
                        let ny = y as i8 + dy * dist;

                        if is_inside(nx, ny) {
                            let next = ny * 8 + nx;
                            moves.push( (id, curr as u64, next as u64) );
                        } 

                    }

                }
            }

            curr += 1;
            position >>= 1;
        }

        moves
    }

    fn minimax(&mut self, depth:i32, mut alpha:i32, mut beta:i32, mode:bool) -> i32 {
        if depth == 0 {
            
        }

        let mut maxv = -9999;
        let mut minv = 9999;
        let moves = self.get_moves();

        for node in moves.iter() {
            let (id,curr,next) = node;

            if mode == true {
                maxv = std::cmp::max(maxv, self.minimax(depth - 1, alpha, beta, false));
                alpha = std::cmp::max(alpha,maxv);
            } else {
                maxv = std::cmp::max(maxv, self.minimax(depth - 1, alpha, beta, true));
                beta = std::cmp::min(beta,minv);
            }

            if mode == true  && maxv >= beta  { break }
            if mode == false && minv <= alpha { break }
        }

        return if mode == true { maxv } else { minv }
    }


    fn select(&mut self) -> (char,u64,u64) {
        let moves = self.get_moves();

        let mut maxv = -999;
        let mut best = &(' ', 0, 0);
        // print!("{:?} ", self.black);
        for node in moves.iter() {
            let (id,curr,next) = node;
            let mut heur = 0;

            if *id == 'R' { heur = HEUR[ROOK][*next as usize] - HEUR[ROOK][*curr as usize]; } 
            if *id == 'K' { heur = HEUR[KING][*next as usize] - HEUR[KING][*curr as usize]; }

            if heur > maxv {
                maxv = heur;
                best = node;
            }

            print!("{} {}\n", to_notation(*id, *next), heur);
        }

        *best
    }



    fn play (&mut self, txt : &str) {

        // self.black = vec![identify(&txt)];
        // let bx = self.black[0].1;
        // let by = self.black[0].2;
        //
        // let best = self.select();
        //
        // print!("{}", to_notation(best.0, best.2));
        // // format!("{}{}{}", id, (nx as u8 + 97) as char,  (8 - ny) as u8)
        // to_notation(best.0 ,best.2)


    }

}

fn main () {

    let mut white_player = WhitePlayer::new("Ke8,Rh7 - Kc8");
    // let mut white_move = white_player.play("Kb8");

    // print!("{:?}\n{:?}\n", white_player.white, white_player.black);
    // let (id,x,y) = identify("Ra8");

    // display(&white_player);



    white_player.get_moves();
    // print!("{:?}", white_player.board);

}
