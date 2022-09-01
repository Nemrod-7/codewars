use std::collections::HashMap;

fn main() {

    let width = 9;
    let height = 6;
    let grid = vec![vec![0;width]; height];

    let mut visited:HashMap<Vec<Vec<usize>>,bool> = HashMap::new();

    visited.insert(grid, true);

}
