use std::cmp::Ordering;

fn cmp_x(a: &(f64,f64), b: &(f64,f64)) -> Ordering {
    if a.0 < b.0 {
        return Ordering::Less;
    } else if a.0 > b.0 {
        return Ordering::Greater;
    }
    return Ordering::Equal;
}
fn cmp_y(a: &(f64,f64), b: &(f64,f64)) -> Ordering {
    if a.1 < b.1 {
        return Ordering::Less;
    } else if a.1 > b.1 {
        return Ordering::Greater;
    }
    return Ordering::Equal;
}

fn distance (a:(f64,f64), b:(f64,f64)) -> f64 {
    let xx = (a.0 - b.0).abs();
    let yy = (a.1 - b.1).abs();
    let add = (xx * xx) + (yy * yy);
    add.sqrt()
}
fn distance2 (cp:((f64,f64),(f64,f64))) -> f64 {
    distance (cp.0, cp.1)
}
fn mindist (a:((f64,f64),(f64,f64)), b:((f64,f64),(f64,f64))) -> ((f64,f64),(f64,f64)) {
    if distance2 (a) < distance2 (b) { return a } else { return b }
}

fn bruteforce (graph: &Vec<(f64, f64)>) -> ((f64, f64), (f64, f64)) {

    let mut pair = ((0.0,0.0), (0.0,0.0));
    let mut maxv = f64::MAX;

    for i in 0..graph.len() - 1 {
        for j in i + 1..graph.len(){
            let dist = distance (graph[i], graph[j]);

            if dist < maxv {
                maxv = dist;
                pair.0 = graph[i];
                pair.1 = graph[j];
            }
        }
    }

    pair
}
fn stripdist (mut graph: Vec<(f64, f64)>, cp:((f64, f64), (f64, f64))) -> ((f64, f64), (f64, f64)) {

    graph.sort_by (&cmp_y);

    let size = graph.len();
    let mut pair = cp;
    let mut minv = distance2 (cp);

    for i in 0..size {
        let mut j = i + 1;
        while j <  size && (graph[i].1 - graph[j].1).abs() < minv {
            let dist = distance (graph[i], graph[j]);

            if dist < minv {
                minv = dist;
                pair.0 = graph[i];
                pair.1 = graph[j];
            }
            j += 1;
        }
    }

    pair
}
fn closetp (graph: &Vec<(f64,f64)>) -> ((f64, f64), (f64, f64)) {

    let size = graph.len();
    let mid = size / 2;

    if size < 4 { return bruteforce (graph) }
    let center = graph[mid];

    let mut left:Vec<(f64,f64)> = Vec::new();
    let mut right:Vec<(f64,f64)> = Vec::new();
    let mut strip:Vec<(f64,f64)> = Vec::new();

    for p in graph {
        if p.0 < center.0 { left.push (*p) } else { right.push (*p) }
    }

    let close = mindist (closetp (&left), closetp (&right));
    let minv = distance2 (close);

    for p in graph {
        if (p.0 - center.0).abs() < minv {
            strip.push (*p)
        }
    }

    stripdist (strip, close)
}
fn closest_pair (graph: &[(f64, f64)]) -> ((f64, f64), (f64, f64)) {

    let mut graph:Vec<(f64,f64)> = graph.iter().cloned().collect();
    graph.sort_by (&cmp_x);

    closetp (&graph)
}

fn main () {

}
