use std::collections::{BTreeMap, HashSet};

fn calculate(graph: &[[i32; 4]]) -> i64 {

    let mut area = 0;
    let mut line:HashSet<usize> = HashSet::new();
    let mut sweep:BTreeMap<i32,Vec<usize>> = BTreeMap::new();
    let mut prev = 0;

    for i in 0..graph.len() {
        let x1 = graph[i][0];
        let x2 = graph[i][2];

        match sweep.get_mut(&x1) {
            Some(index) => { index.push(i); },
            None => { sweep.insert(x1, vec![i]); }
        }
        match sweep.get_mut(&x2) {
            Some(index) => { index.push(i); },
            None => { sweep.insert(x2, vec![i]); }
        }
    }

    for (pos, rec) in sweep {

        if !line.is_empty() {
            let x = (pos, prev);
            let mut inter:Vec<(i32,i32)> = Vec::new();
            let mut merge:Vec<(i32,i32)> = Vec::new();

            for index in &line {
                let y1 = graph[*index][1];
                let y2 = graph[*index][3];
                inter.push((y1,y2));
            }

            inter.sort();

            for y in &inter {
                if merge.is_empty() {
                    merge.push (*y);
                } else {
                    let last = merge.len() - 1;

                    if merge[last].1 < y.0 {
                        merge.push(*y);
                    } else {
                        merge[last].1 = std::cmp::max(merge[last].1, y.1);
                    }
                }
            }

            for y in merge {
                area += ((x.1 - x.0).abs() * (y.1 - y.0).abs()) as i64;
            }
        }

        for index in rec {
            if graph[index][0] == pos { line.insert(index); }
            if graph[index][2] == pos { line.remove(&index); }
        }
        prev = pos;
    }

    area
}

fn main() {
    let area = calculate(&[[3,3,8,5], [6,3,8,9], [11,6,14,12]]);
    //print!(" => {area}");
    test();
}

fn test () {
    assert_eq!(calculate(&[[ 1, 7, 3, 10 ],  [ 1, 8, 3, 9 ]]), 6, "1 under 2");
    assert_eq!(calculate(&[[ 6, 7, 9, 10 ],  [ 7, 8, 8, 9 ]]), 9, "nested");
    assert_eq!(calculate(&[[ 1, 7, 4, 10 ],  [ 2, 7, 4, 9 ],  [ 3, 7, 4, 9 ]]), 9, "nested 2");
    assert_eq!(calculate(&[[ 1, 1, 4, 3 ],  [ 2, 2, 3, 4 ]]), 7, "intersection up");
    assert_eq!(calculate(&[[ 5, 0, 7, 3 ],  [ 6, 1, 8, 2 ]]), 7, "intersetion right");
    assert_eq!(calculate(&[[ 9, 0, 11, 2 ],  [ 10, 1, 12, 3 ]]), 7, "intersection up right");
    assert_eq!(calculate(&[[ 13, 5, 15, 6 ],  [ 14, 4, 16, 7 ]]), 7, "intersection of the entire right side");
    assert_eq!(calculate(&[[ 17, 1, 19, 3 ],  [ 18, 0, 20, 2 ]]), 7, "intersection down right");
    assert_eq!(calculate(&[[ 13, 1, 16, 3 ],  [ 14, 0, 15, 2 ]]), 7, "intersection down");
    assert_eq!(calculate(&[[ 1, 3, 4, 6 ],  [ 2, 1, 5, 4 ],  [ 3, 2, 6, 5 ]]), 20, "intersection 3 rect");
    assert_eq!(calculate(&[[1,1,2,2],[2,1,3,2],[3,1,4,2],[1,2,2,3],[2,2,3,3],[3,2,4,3],[1,3,2,4],[2,3,3,4],[3,3,4,4]]), 9, "3*3");
    assert_eq!(calculate(&[[ 1, 1, 6, 6 ],[ 1, 3, 4, 6 ],[ 2, 3, 4, 6 ],[ 2, 4, 5, 6 ],[ 3, 5, 4, 6 ]]), 25, "intersection");
    assert_eq!(calculate(&[[1,1,6,6],[2,1,6,6],[3,1,6,6],[4,1,6,6],[5,2,6,5]]), 25, "shift right");
    assert_eq!(calculate(&[[1,1,7,6],[2,2,8,7],[3,3,9,8],[4,4,10,9],[5,5,11,10]]), 70, "shift right down");
    assert_eq!(calculate(&[[ 1, 4, 5, 6 ],  [ 2, 5, 6, 7 ],[ 3, 6, 7, 8 ],[ 4, 7, 8, 9 ],[ 2, 3, 6, 5 ],[ 3, 2, 7, 4 ],[ 4, 1, 8, 3 ]]), 38, "wings");
    assert_eq!(calculate(&[[ 9, 5, 12, 6 ],  [ 10, 4, 11, 7 ]]), 5, "intersection cross");
    assert_eq!(calculate(&[[ 7, 1, 11, 7 ],  [ 8, 0, 12, 3 ],  [ 8, 4, 13, 5 ],  [ 9, 5, 14, 8 ],[ 10, 2, 15, 6 ]]), 53, "intersection 2");
    assert_eq!(calculate(&[[1,2,6,6],[1,3,5,5],[1,1,7,7]]), 36, "pyramid");
    assert_eq!(calculate(&[[1,2,3,7],[2,1,7,3],[6,2,8,7],[2,6,7,8],[4,4,5,5]]), 37, "circle");
    assert_eq!(calculate(&[[1,1,2,2],[1,1,2,2],[1,1,2,2],[1,1,2,2],[1,1,2,2],[1,1,2,2]]), 1, "one");
    assert_eq!(calculate(&[[3,3,6,5],[4,4,6,6],[4,3,7,5],[4,2,8,5],[4,3,8,6],[9,0,11,4],[9,1,10,6],[9,0,12,2],[10,1,13,5],[12,4,15,6],[14,1,16,5],[12,1,17,2]]), 52, "very hard!");
    assert_eq!(calculate(&[[2, 2, 17, 2], [2, 2, 17, 4], [2, 2, 17, 6], [2, 2, 17, 8], [2, 2, 17, 10], [2, 2, 17, 12], [2, 2, 17, 14], [2, 2, 17, 16], [2, 2, 17, 18], [2, 2, 17, 20], [2, 2, 17, 22], [2, 2, 17, 24], [2, 2, 17, 26], [2, 2, 17, 28]]), 390, "waterfall");

    /*
    */
}
