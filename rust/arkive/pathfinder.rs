fn isinside (size: i32, x: i32, y: i32) -> bool {
    return x >= 0 && y >= 0 && x < size && y < size;
}

fn path_finder(area: &[Vec<u32>]) -> u32 {

    let dir = [(-1,0),(0,-1),(0,1), (1,0)];
    let size = area.len();
    let exit = (size-1, size-1);

    let mut heap = vec![(0,(0,0))];
    let mut graph = vec![vec![(u32::MAX, false); size]; size];

    graph[0][0].0 = 0;

    while let Some ((dist, u)) = heap.pop() {
        graph[u.1][u.0].1 = true;

        if u == exit {
            return dist;
        }

        for d in dir {
            let nx = u.0 as i32 + d.0;
            let ny = u.1 as i32 + d.1;

            if isinside (size as i32, nx, ny) {
                let nx = nx as usize;
                let ny = ny as usize;

                let (nxdist, visited) = graph[ny][nx];
                let alt = dist + (area[u.1][u.0] as i32 - area[ny][nx] as i32).abs() as u32;

                if !visited && alt < nxdist {
                    graph[ny][nx].0 = alt;
                    heap.push((alt,(nx,ny)));
                    heap.sort_by(|a, b| b.0.cmp(&a.0));
                }
            }
        }
    }

    0
}

fn test_equal(input: &[Vec<u32>], actual: u32, expected: u32) {
    assert_eq!(actual, expected, "\nFor the input: {:?}\nYour result (left) did not match the expected output (right)", input);
}

fn test_basic() {
    let area: Vec<Vec<u32>> = vec![
        vec![0, 0, 0],
        vec![0, 0, 0],
        vec![0, 0, 0]
    ];
    test_equal(&area,path_finder(&area), 0);

    let area: Vec<Vec<u32>> = vec![
        vec![0, 1, 0],
        vec![0, 1, 0],
        vec![0, 1, 0]
    ];
    test_equal(&area,path_finder(&area), 2);


    let area: Vec<Vec<u32>> = vec![
        vec![0, 1, 0],
        vec![1, 0, 1],
        vec![0, 1, 0]
    ];
    test_equal(&area,path_finder(&area), 4);

    let area: Vec<Vec<u32>> = vec![
        vec![0, 7, 0, 7],
        vec![7, 0, 7, 0],
        vec![0, 7, 0, 7],
        vec![7, 0, 7, 0]
    ];
    test_equal(&area,path_finder(&area), 42);

    let area: Vec<Vec<u32>> = vec![
        vec![7, 0, 0, 0, 0, 0],
        vec![0, 7, 7, 7, 7, 0],
        vec![0, 7, 7, 7, 7, 0],
        vec![0, 7, 7, 7, 7, 0],
        vec![0, 7, 7, 7, 7, 0],
        vec![0, 0, 0, 0, 0, 7]
    ];
    test_equal(&area,path_finder(&area), 14);

    let area: Vec<Vec<u32>> = vec![
        vec![7, 7, 7, 0, 0, 0],
        vec![0, 0, 7, 0, 0, 0],
        vec![0, 0, 7, 0, 0, 0],
        vec![0, 0, 7, 0, 0, 0],
        vec![0, 0, 7, 0, 0, 0],
        vec![0, 0, 7, 7, 7, 7]
    ];
    test_equal(&area,path_finder(&area), 0);

    let area: Vec<Vec<u32>> = vec![
        vec![0, 0, 0, 0, 0, 0],
        vec![0, 0, 0, 0, 0, 0],
        vec![0, 0, 0, 0, 0, 0],
        vec![0, 0, 0, 0, 1, 0],
        vec![0, 0, 0, 1, 0, 9],
        vec![0, 0, 1, 0, 1, 0]
    ];
    test_equal(&area,path_finder(&area), 4);


}

fn main() {

        test_basic();
}
