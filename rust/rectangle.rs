
fn area (rec: [i32;4]) -> i64 {
    ((rec[0] - rec[2]).abs() * (rec[1] - rec[3]).abs()) as i64
}
fn overlap(r1:[i32;4], r2:[i32;4]) -> bool {
    return r1[0] <= r2[2] && r2[0] <= r1[2] && r1[1] <= r2[3] && r2[1] <= r1[3]; 
}

fn overarea(r1:[i32;4], r2:[i32;4]) -> i64 {
    let width = std::cmp::min(r1[2], r2[2]) as i64 - std::cmp::max(r1[0], r2[0]) as i64 ;
    let height = std::cmp::min(r1[3], r2[3]) as i64 - std::cmp::max(r1[1], r2[1]) as i64;

    if width >= 0 && height >= 0 { return width * height } else { return 0  }
}
fn calculate(graph: &[[i32; 4]]) -> i64 {
    
    let mut sum = 0;
    sum = graph.iter().map(|x| area(*x)).sum::<i64>();
    
    for i in 0..graph.len() {
        for j in  i + 1..graph.len() {

            sum -= overarea(graph[i], graph[j]);
        }
    }

    print!("{sum} ");
    sum
}

fn main() {

}

fn test () {
    assertEquals(calculate([ [ 1, 7, 3, 10 ],  [ 1, 8, 3, 9 ]]), 6, '1 under 2');
    assertEquals(calculate([ [ 6, 7, 9, 10 ],  [ 7, 8, 8, 9 ]]), 9, "nested");
    assertEquals(calculate([ [ 1, 7, 4, 10 ],  [ 2, 7, 4, 9 ],  [ 3, 7, 4, 9 ]]), 9, "nested 2");
    assertEquals(calculate([ [ 1, 1, 4, 3 ],  [ 2, 2, 3, 4 ]]), 7, "intersection up");
    assertEquals(calculate([ [ 5, 0, 7, 3 ],  [ 6, 1, 8, 2 ]]), 7, "intersetion right");
    assertEquals(calculate([ [ 9, 0, 11, 2 ],  [ 10, 1, 12, 3 ]]), 7, "intersection up right");
    assertEquals(calculate([ [ 13, 5, 15, 6 ],  [ 14, 4, 16, 7 ]]), 7, "intersection of the entire right side");
    assertEquals(calculate([ [ 17, 1, 19, 3 ],  [ 18, 0, 20, 2 ]]), 7, "intersection down right");
    assertEquals(calculate([ [ 13, 1, 16, 3 ],  [ 14, 0, 15, 2 ]]), 7, "intersection down");
    assertEquals(calculate([ [ 1, 3, 4, 6 ],  [ 2, 1, 5, 4 ],  [ 3, 2, 6, 5 ]]), 20, "intersection 3 rect");
    assertEquals(calculate([[1,1,2,2],[2,1,3,2],[3,1,4,2],[1,2,2,3],[2,2,3,3],[3,2,4,3],[1,3,2,4],[2,3,3,4],[3,3,4,4]]), 9, "3*3");
    assertEquals(calculate([[ 1, 1, 6, 6 ],[ 1, 3, 4, 6 ],[ 2, 3, 4, 6 ],[ 2, 4, 5, 6 ],[ 3, 5, 4, 6 ]]), 25, "intersection");
    assertEquals(calculate([[1,1,6,6],[2,1,6,6],[3,1,6,6],[4,1,6,6],[5,2,6,5]]), 25, "shift right");
    assertEquals(calculate([[1,1,7,6],[2,2,8,7],[3,3,9,8],[4,4,10,9],[5,5,11,10]]), 70, "shift right down");
    assertEquals(calculate([ [ 1, 4, 5, 6 ],  [ 2, 5, 6, 7 ],[ 3, 6, 7, 8 ],[ 4, 7, 8, 9 ],[ 2, 3, 6, 5 ],[ 3, 2, 7, 4 ],[ 4, 1, 8, 3 ]]), 38, "wings");
    assertEquals(calculate([ [ 9, 5, 12, 6 ],  [ 10, 4, 11, 7 ]]), 5, "intersection cross");
    assertEquals(calculate([ [ 7, 1, 11, 7 ],  [ 8, 0, 12, 3 ],  [ 8, 4, 13, 5 ],  [ 9, 5, 14, 8 ],[ 10, 2, 15, 6 ]]), 53, "intersection 2");
    assertEquals(calculate([[1,2,6,6],[1,3,5,5],[1,1,7,7]]), 36, "pyramid");
    assertEquals(calculate([[1,2,3,7],[2,1,7,3],[6,2,8,7],[2,6,7,8],[4,4,5,5]]), 37, "circle");
    assertEquals(calculate([[1,1,2,2],[1,1,2,2],[1,1,2,2],[1,1,2,2],[1,1,2,2],[1,1,2,2]]), 1, "one");
    assertEquals(calculate([[3,3,6,5],[4,4,6,6],[4,3,7,5],[4,2,8,5],[4,3,8,6],[9,0,11,4],[9,1,10,6],[9,0,12,2],[10,1,13,5],[12,4,15,6],[14,1,16,5],[12,1,17,2]]), 52, 'very hard!');
    assertEquals(calculate([[2, 2, 17, 2], [2, 2, 17, 4], [2, 2, 17, 6], [2, 2, 17, 8], [2, 2, 17, 10], [2, 2, 17, 12], [2, 2, 17, 14], [2, 2, 17, 16], [2, 2, 17, 18], [2, 2, 17, 20], [2, 2, 17, 22], [2, 2, 17, 24], [2, 2, 17, 26], [2, 2, 17, 28]]), 390, "waterfall");
		});
}
