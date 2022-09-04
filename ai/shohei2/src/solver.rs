
use core::panic;

use crate::data::*;
use crate::draw::calc_colored_state;
use crate::eval::similarity;
use image::RgbaImage;
use rand::thread_rng;
use rand::Rng;

pub fn solve(target:&RgbaImage) -> State {
    let mut min_state = Element {
        state: State::new(RgbaImage::new(target.width(), target.height())),
        score: i64::MAX
    };
    let mut rng = thread_rng();
    let mut current = vec![min_state.clone()];
    let beam_w = 1000;

    for step in 0..25 {
        println!("step {}", step);
        let mut next = Vec::new();
        let size = usize::min(current.len(), (beam_w as f64 / 2.5) as usize);
        for i in 0..beam_w
        {
            let mut element = current[i % size].clone();
            let node_num = element.state.tree.get_leaf_num();
            let id = element.state.tree.leaf_at(rng.gen_range(0, node_num));
            let node = element.state.tree.find(&id);
            match node {
                Tree::Leaf(rect, _) => {
                    if rect.w * rect.h < 200 { continue; }
                    match rng.gen_range(0, 4) {
                        0 => {
                            if rect.w < 5 { continue; }
                            let x = rng.gen_range(rect.x + 2, rect.right() - 2);
                            element.state.apply_command(&Command::LineCut(id.clone(), true, x));
                        },
                        1 => {
                            if rect.h < 5 { continue; }
                            let y = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                            element.state.apply_command(&Command::LineCut(id.clone(), false, y));
                        },
                        2 => {
                            if rect.w < 5 { continue; }
                            if rect.h < 5 { continue; }
                            let x = rng.gen_range(rect.x + 2, rect.right() - 2);
                            let y = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                            element.state.apply_command(&Command::PointCut(id.clone(), Point{x, y}));
                        },
                        3 => {
                            if rect.w < 5 { continue; }
                            if rect.h < 5 { continue; }
                            let x = rng.gen_range(rect.x + 2, rect.right() - 2);
                            let y = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                            element.state.apply_command(&Command::PointCut(id.clone(), Point{x, y}));
                            let mut a = id.clone();
                            let mut b = id.clone();
                            let i = rng.gen_range(0, 4);
                            a.push(i);
                            b.push((i + 1) % 4); 
                            element.state.apply_command(&Command::Merge(a, b));
                        },
                        _ => {}
                    }
                },
                _ => panic!("must be leaf")
            }

            // eval
            let colored_state = calc_colored_state(&element.state, target, true);
            element.score = colored_state.cost + similarity(&colored_state.image, &target);
            if min_state.score > element.score {
                min_state = element.clone();
            }
            next.push(element);
        }
        next.sort_by(|a, b| a.score.cmp(&b.score));
        current = next;
    }
    calc_colored_state(&min_state.state, target, false)
}

#[derive(Clone)]
struct Element {
    state: State,
    score: i64,
}