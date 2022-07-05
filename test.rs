

fn main() {
    let maze = String::from("this is only\n a test");
    path_finder(maze);
}
fn hypot(x:f64, y:f64)-> f64 {
    let num = x.powi(2) + y.powi(2);
    num.powf(0.5)
}
fn distance (a: [i32; 2], b: [i32; 2]) {

}
fn path_finder(maze: String) {

    let directions: [[i32; 2]; 4] = [[-1,0],[1,0],[0,-1],[0,1]];
    let mut sz;

    match maze.find('\n') {
        Some (x) => sz = x + 1,
        None    => sz = 0,
    }
    println!("Result: {:?}", sz);
    //q1.push (0,0);
    //let x : i8 = sze;
    //let directions : [i8; 4] = [-1,1,-sze,sze];
    //maze.find('\n') + 1;

    //println!("{}",sze);
}
