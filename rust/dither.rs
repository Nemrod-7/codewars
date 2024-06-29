use std::collections::VecDeque;

fn show(mat: &Vec<Vec<usize>>) {
    print!("\n");
    for i in 0..mat.len() {
        for j in 0..mat[i].len(){
            print!("{:2} ", mat[i][j]);
        }
        print!("\n");
    }
    print!("\n");
}

fn scale (size:usize) -> usize {
    let nx = f32::round(size as f32 / 2.0) as usize;

    match nx % 2  {
        1 => nx + 1,
        _ => nx + 0,
    }
}

fn dither (width: usize, height: usize) -> Vec<(usize,usize)> {

    let rx = scale(width);
    let ry = scale(height);
    let dither = [(0,0),(1,1),(1,0),(0,1)];

    print!("[{rx} {ry}]\n");
    let mut q1 = VecDeque::new();
    let mut res:Vec<(usize,usize)> = Vec::new();
    let mut visit = vec![vec![false; width]; height];
    let mut index = 1;
    let mut mat = vec![vec![0; width]; height];

    q1.push_back((0,0));

    while let Some((sx,sy)) = q1.pop_front() {

        if !visit[sy][sx] {

            for (dx,dy) in &dither {
                let nx = dx * rx + sx;
                let ny = dy * ry + sy;

                if nx < width && ny < height {
                    visit[ny][nx] = true;
                    mat[ny][nx] = index;
                    index += 1;
                    //print!("[{nx},{ny}]");
                    res.push((nx,ny));
                    q1.push_back((nx / 2, ny / 2));
                }
            }
        }
    }

    show(&mat);
    res
}


fn main() {

    /*

 1 16  6 21  3 18
25 10 28 13 26 11
 8 23  5 20  9 24
29 14 27 12 30 15
 4 19  7 22  2 17

 */


    dither(6,5);

}
