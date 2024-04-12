	
struct BST {
    value: i32,
    count: usize,
    left : Option<Box<BST>>,
    right: Option<Box<BST>>,
}

impl BST {
    fn new(value: i32) -> Self {
        BST { value: value, count: 0, left : None, right: None }
    }
}

fn insert (root: &mut Option<Box<BST>>, smaller: usize, value: i32) -> usize {

    match root {
        Some (node) => {
            if node.value < value {
                return node.count +  insert(&mut node.right, smaller + 1, value);
            } else {
                node.count += 1;
                return insert(&mut node.left, smaller, value);
            }
        },
        None => {
            *root = Some( Box::new( BST::new(value))) ;
            return smaller;
        },
    }
}

fn smaller (arr: &[i32]) -> Vec<usize>{

    let mut root = None;
    let mut res = vec![0;arr.len()];

    for i in (0..arr.len()).rev() {
        res[i] = insert(&mut root, 0, arr[i]);
    }

    res
}


fn main () {

}
