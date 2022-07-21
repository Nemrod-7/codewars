fn is_free (maze: &Vec<char>, pos: i32) -> bool {
    return pos >= 0 && pos < maze.len() as i32 && maze[pos as usize] == '.';
}
fn path_finder (maze: &str) -> bool {

    let size:i32 = maze.len() as i32;
    if size < 2 { return true; }

    let width:i32 = maze.find('\n').unwrap() as i32 + 1;
    let direction = [-1, 1, -width, width];

    let mut maze:Vec<char> = maze.chars().collect();
    let mut path = vec![0i32];
    let mut dist = vec![0i32];

    while !path.is_empty() {
        let pos = path.last().unwrap() + 0;
        let alt = dist.last().unwrap() + 1;

        path.pop(); dist.pop();
        maze[pos as usize] = 'x';

        if pos == size - 1 { return true }

        for dir in direction {
            let nxt = dir + pos;

            if is_free (&maze, nxt) {
                path.push(nxt);
                dist.push(alt);
            }
        }
    }

    false
}
