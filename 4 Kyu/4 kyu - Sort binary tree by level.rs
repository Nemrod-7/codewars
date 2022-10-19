mod preloaded;
use preloaded::Node;
use std::collections::VecDeque;

fn tree_by_levels(root: &Node) -> Vec<u32> {

    let mut order = Vec::new();
    let mut queue:VecDeque<&Node> = VecDeque::from([root]);

    while let Some(node) = queue.pop_front() {
        order.push(node.value);

        if let Some (left) = &node.left {
            queue.push_back (left);
        }
        if let Some (right) = &node.right {
            queue.push_back (right);
        }
    }

    order
}
