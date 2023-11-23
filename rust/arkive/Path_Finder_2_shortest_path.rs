fn display (maze:&Vec<char>, size:usize) {
    for y in 0..size - 1 {
        for x in 0..size - 1 {
            let cell = &maze[y * size + x];
            print! ("{cell}");
        }
        print! ("\n");
    }
}
fn is_inside (pos:i32, len:usize) -> bool { pos >= 0 && pos < len as i32 }
fn is_free (pos:i32, maze:&Vec<char>) -> bool { maze[pos as usize] == '.' }
fn path_finder(maze: &str) -> Option<u32> {
    let mut maze:Vec<char> = maze.chars().collect();
    let size = maze.iter().position (|&x| x == '\n').unwrap() + 1;
    let exit = maze.len() - 1;
    let compass = [-1,1, 0-size as i32, size as i32];

    let mut Q = vec![(0,0)];

    while let Some ((index, alt)) = Q.pop() {

        if index == exit {
            return Some(alt);
        }

        for dir in compass {
            let next = index as i32 + dir;

            if is_inside (next, exit + 1) && is_free (next, &maze) {

                Q.push((next as usize, alt + 1));
                maze[next as usize] = 'x';
            }
        }
    }

    None
}

fn main () {

    path_finder(".W.\n.W.\n...");
    /*
    assert_eq!(path_finder(".W.\n.W.\n..."), Some(4), "\nYour answer (left) is not the expected answer (right).");
    assert_eq!(path_finder(".W.\n.W.\nW.."), None, "\nYour answer (left) is not the expected answer (right).");
    assert_eq!(path_finder("......\n......\n......\n......\n......\n......"), Some(10), "\nYour answer (left) is not the expected answer (right).");
    assert_eq!(path_finder("......\n......\n......\n......\n.....W\n....W."), None, "\nYour answer (left) is not the expected answer (right).");
    */
}
