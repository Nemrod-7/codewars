use std::collections::VecDeque;

fn is_inside (pos:i32, len:usize) -> bool { pos >= 0 && pos < len as i32 }
fn is_free (pos:i32, maze:&Vec<char>) -> bool { maze[pos as usize] == '.' }
fn path_finder(maze: &str) -> Option<u32> {

    if maze.len() < 2 { return Some(0); }

    let size = maze.find('\n').unwrap() + 1;
    let exit = maze.len() - 1;
    let compass = [-1,1, 0-size as i32, size as i32];
    let mut maze:Vec<char> = maze.chars().collect();
    let mut heap:VecDeque<(usize,u32)> = vec![(0,0)].into();

    while let Some ((index, alt)) = heap.pop_back() {

        if index == exit {
            return Some(alt);
        }

        for dir in compass {
            let next = index as i32 + dir;

            if is_inside (next, exit + 1) && is_free (next, &maze) {
                heap.push_front((next as usize, alt + 1));
                maze[next as usize] = 'x';
            }
        }
    }

    None
}
