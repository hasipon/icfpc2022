
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
    let beam_w = 80;

    for step in 0..25 {
        println!("step {}", step);
        let mut next = Vec::new();
        let size = usize::min(current.len(), (beam_w as f64 / 2.5) as usize);
        if size == 0 { break; }
        for i in 0..size / 4 {
            next.push(current[i].clone());
        }
        for i in 0..beam_w
        {
            let source = &current[i % size];
            let node_num = source.state.tree.get_leaf_num();
            let id = source.state.tree.leaf_at(rng.gen_range(0, node_num));
            let node = source.state.tree.find(&id);
            let mut element;
            match node {
                Tree::Leaf(rect, _) => {
                    if rect.w * rect.h < 200 { continue; }
                    match rng.gen_range(0, 5) {
                        0 => {
                            if rect.w < 5 { continue; }
                            let x = {
                                let x1 = rng.gen_range(rect.x + 2, rect.right() - 2);
                                let x2 = rng.gen_range(rect.x + 2, rect.right() - 2);
                                let y = rng.gen_range(rect.y, rect.bottom());
                                let ax = i32::min(x1, x2);
                                let bx = i32::max(x1, x2);
                                let mut max_diff= 0.0;
                                let mut max_val = x1;
                                for i in ax..bx {
                                    let diff = compare(i, y, i + 1, y, &target);
                                    if max_diff < diff {
                                        max_diff = diff;
                                        max_val = i;
                                    }
                                }
                                if max_diff < 40.0 { continue; }
                                max_val + 1
                            };
                            element = source.clone();
                            element.state.apply_command(&Command::LineCut(id.clone(), true, x));
                        },
                        1 => {
                            if rect.h < 5 { continue; }
                            let y = {
                                let x = rng.gen_range(rect.x, rect.right());
                                let y1 = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                                let y2 = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                                let ay = i32::min(y1, y2);
                                let by = i32::max(y1, y2);
                                let mut max_diff= 0.0;
                                let mut max_val = y1;
                                for i in ay..by {
                                    let diff = compare(x, i, x, i + 1, &target);
                                    if max_diff < diff {
                                        max_diff = diff;
                                        max_val = i;
                                    }
                                }
                                if max_diff < 40.0 { continue; }
                                max_val + 1
                            };
                            
                            element = source.clone();
                            element.state.apply_command(&Command::LineCut(id.clone(), false, y));
                        },
                        2 => {
                            if rect.w < 5 { continue; }
                            if rect.h < 5 { continue; }
                            let x = {
                                let x1 = rng.gen_range(rect.x + 2, rect.right() - 2);
                                let x2 = rng.gen_range(rect.x + 2, rect.right() - 2);
                                let y = rng.gen_range(rect.y, rect.bottom());
                                let ax = i32::min(x1, x2);
                                let bx = i32::max(x1, x2);
                                let mut max_diff= 0.0;
                                let mut max_val = x1;
                                for i in ax..bx + 1 {
                                    let diff = compare(i, y, i + 1, y, &target);
                                    if max_diff < diff {
                                        max_diff = diff;
                                        max_val = i;
                                    }
                                }
                                if max_diff < 40.0 { continue; }
                                max_val + 1
                            };
                            let y = {
                                let x = rng.gen_range(rect.x, rect.right());
                                let y1 = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                                let y2 = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                                let ay = i32::min(y1, y2);
                                let by = i32::max(y1, y2);
                                let mut max_diff= 0.0;
                                let mut max_val = y1;
                                for i in ay..by + 1 {
                                    let diff = compare(x, i, x, i + 1, &target);
                                    if max_diff < diff {
                                        max_diff = diff;
                                        max_val = i;
                                    }
                                }
                                if max_diff < 40.0 { continue; }
                                max_val + 1
                            };
                            element = source.clone();
                            element.state.apply_command(&Command::PointCut(id.clone(), Point{x, y}));
                        },
                        3 => {
                            if rect.w < 5 { continue; }
                            if rect.h < 5 { continue; }
                            let x = {
                                let x1 = rng.gen_range(rect.x + 2, rect.right() - 2);
                                let x2 = rng.gen_range(rect.x + 2, rect.right() - 2);
                                let y = rng.gen_range(rect.y, rect.bottom());
                                let ax = i32::min(x1, x2);
                                let bx = i32::max(x1, x2);
                                let mut max_diff= 0.0;
                                let mut max_val = x1;
                                for i in ax..bx + 1 {
                                    let diff = compare(i, y, i + 1, y, &target);
                                    if max_diff < diff {
                                        max_diff = diff;
                                        max_val = i;
                                    }
                                }
                                if max_diff < 40.0 { continue; }
                                max_val + 1
                            };
                            let y = {
                                let x = rng.gen_range(rect.x, rect.right());
                                let y1 = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                                let y2 = rng.gen_range(rect.y + 2, rect.bottom() - 2);
                                let ay = i32::min(y1, y2);
                                let by = i32::max(y1, y2);
                                let mut max_diff= 0.0;
                                let mut max_val = y1;
                                for i in ay..by + 1 {
                                    let diff = compare(x, i, x, i + 1, &target);
                                    if max_diff < diff {
                                        max_diff = diff;
                                        max_val = i;
                                    }
                                }
                                if max_diff < 40.0 { continue; }
                                max_val + 1
                            };
                            element = source.clone();
                            element.state.apply_command(&Command::PointCut(id.clone(), Point{x, y}));
                            let mut a = id.clone();
                            let mut b = id.clone();
                            let i = rng.gen_range(0, 4);
                            a.push(i);
                            b.push((i + 1) % 4); 
                            element.state.apply_command(&Command::Merge(a, b));
                        },
                        4 => {
                            if source.state.commands.len() == 0 { continue; }
                            element = source.clone();
                            element.state.remove_command_at(rng.gen_range(0, source.state.commands.len()));
                        }
                        _ => panic!("unknown")
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

fn compare(x1:i32, y1:i32, x2:i32, y2:i32, target:&RgbaImage) -> f64 {
    let p0 = target.get_pixel(
        x1 as u32,
        399 - y1 as u32
    );
    let p1 = target.get_pixel(
        x2 as u32,
        399 - y2 as u32
    );
    let r = p0[0] as f64 - p1[0] as f64;
    let g = p0[1] as f64 - p1[1] as f64;
    let b = p0[2] as f64 - p1[2] as f64;
    let a = p0[3] as f64 - p1[3] as f64;
    r * r + g * g + b * b + a * a
}

#[derive(Clone)]
struct Element {
    state: State,
    score: i64,
}