

use std::borrow::Borrow;
use std::ops::Index;

use crate::data::*;
use crate::eval::similarity;
use crate::solver;
use image::Luma;
use image::Rgba;
use image::RgbaImage;
use image::GrayImage;
use imageproc::drawing::draw_filled_rect_mut;
use imageproc::rect::Rect;
use rand::Rng;
use rand::thread_rng;

#[derive(Clone)]
struct PainterState{
    rects: Vec<Rectangle>,
    score: i64,
}

pub struct PainterResult {
    pub commands: Vec<Command>,
    pub cost: i64,
    pub similarity: i64,
    pub image:RgbaImage,
}

pub fn solve(target:&RgbaImage) -> PainterResult {
    let mut best_rects = Vec::new();
    let mut best_result = PainterResult {
        commands: Vec::new(),
        cost: 0,
        similarity: i64::MAX,
        image:RgbaImage::new(1, 1)
    };
    let mut rng = thread_rng();
    let initial_state = PainterState {
        rects: Vec::new(),
        score: i64::MAX
    };
    let mut gray_image = GrayImage::new(target.width(), target.height());

    let mut current = vec![initial_state];
    let beam_w = 30;
    let w = target.width() as i32;
    let h = target.height() as i32;
    for step in 0..2500 {
        println!("step {}", step);
        let mut next = Vec::new();
        let size = usize::min(current.len(), (beam_w as f64 / 2.5) as usize);
        if size == 0 { break; }
        if step % 5 != 4 {
            for i in 0..size {
                next.push(current[i].clone());
            }
        }
        for i in 0..beam_w {
            let source = &current[i % size];
            let mut rects = source.rects.clone();

            match rng.gen_range(0, if step == 0 { 1 } else { 8 }) {
                0 | 5 | 7 => {
                    let (diff, x1) = if rng.gen_bool(0.1) { (10000.0, 0) } else { solver::find_x_boundary(0, w, 0, h, &target, &mut rng) };
                    if diff < 40.0 { continue; }
                    let (diff, x2) = if rng.gen_bool(0.1) { (10000.0, w) } else { solver::find_x_boundary(0, w, 0, h, &target, &mut rng) };
                    if diff < 40.0 { continue; }
                    let (diff, y1) = if rng.gen_bool(0.1) { (10000.0, 0) } else { solver::find_y_boundary(0, w, 0, h, &target, &mut rng) };
                    if diff < 40.0 { continue; }
                    let (diff, y2) = if rng.gen_bool(0.1) { (10000.0, h) } else { solver::find_y_boundary(0, w, 0, h, &target, &mut rng) };
                    if diff < 40.0 { continue; }

                    let ax = std::cmp::min(x1, x2);
                    let bx = std::cmp::max(x1, x2);
                    let ay = std::cmp::min(y1, y2);
                    let by = std::cmp::max(y1, y2);
                    if ax == bx { continue; }
                    if ay == by { continue; }
                    rects.insert(rng.gen_range(0, rects.len() + 1), Rectangle { 
                        x: ax,
                        y: ay,
                        w: bx - ax,
                        h: by - ay,
                    });
                },
                1 => {
                    if rects.len() < 2 { continue; }
                    let i0 = rng.gen_range(0, rects.len()); 
                    let i1 = rng.gen_range(0, rects.len());
                    if i0 == i1 { continue; }
                    let is_t0 = rng.gen_bool(0.5);
                    let is_t1 = rng.gen_bool(0.5);
                    let av0 = if is_t0 { rects[i0].x } else { rects[i0].right() };
                    let av1 = if is_t1 { rects[i1].x } else { rects[i1].right() };
                    let bv0 = if is_t1 { rects[i1].right() } else { rects[i1].x };
                    let bv1 = if is_t0 { rects[i0].right() } else { rects[i0].x };
                    if av0 == bv0 { continue; }
                    if av1 == bv1 { continue; }
                    let l0 = std::cmp::min(av0, bv0);
                    let r0 = std::cmp::max(av0, bv0);
                    let l1 = std::cmp::min(av1, bv1);
                    let r1 = std::cmp::max(av1, bv1);
                    rects[i0].x = l0;
                    rects[i0].w = r0 - l0;
                    rects[i1].x = l1;
                    rects[i1].w = r1 - l1;
                },
                2 => {
                    if rects.len() < 2 { continue; }
                    let i0 = rng.gen_range(0, rects.len()); 
                    let i1 = rng.gen_range(0, rects.len());
                    if i0 == i1 { continue; }
                    let is_t0 = rng.gen_bool(0.5);
                    let is_t1 = rng.gen_bool(0.5);
                    let av0 = if is_t0 { rects[i0].y } else { rects[i0].bottom() };
                    let av1 = if is_t1 { rects[i1].y } else { rects[i1].bottom() };
                    let bv0 = if is_t1 { rects[i1].bottom() } else { rects[i1].y };
                    let bv1 = if is_t0 { rects[i0].bottom() } else { rects[i0].y };
                    if av0 == bv0 { continue; }
                    if av1 == bv1 { continue; }
                    let l0 = std::cmp::min(av0, bv0);
                    let r0 = std::cmp::max(av0, bv0);
                    let l1 = std::cmp::min(av1, bv1);
                    let r1 = std::cmp::max(av1, bv1);
                    rects[i0].y = l0;
                    rects[i0].h = r0 - l0;
                    rects[i1].y = l1;
                    rects[i1].h = r1 - l1;
                },
                3 => {
                    if rects.len() < 2 { continue; }
                    let i0 = rng.gen_range(0, rects.len()); 
                    let i1 = rng.gen_range(0, rects.len());
                    if i0 == i1 { continue; }
                    rects.swap(i0, i1);
                },
                4 => {
                    if rects.len() < 1 { continue; }
                    rects.remove(rng.gen_range(0, rects.len()));
                },
                6 => {
                    if rects.len() < 1 { continue; }
                    let mut rect = rects[rng.gen_range(0, rects.len())];
                    if rng.gen_bool(0.5) {
                        if rng.gen_bool(0.5) {
                            if rng.gen_bool(0.5) {
                                if rect.y == rect.bottom() - 1 { continue; }
                                rect.y += 1;
                            } else {
                                if rect.x == 0 { continue; }
                                rect.y -= 1;
                            }
                        } else {
                            if rng.gen_bool(0.5) {
                                if rect.x == rect.right() - 1 { continue; }
                                rect.x += 1;
                            } else {
                                if rect.x == 0 { continue; }
                                rect.x -= 1;
                            }
                        }
                    } else {
                        if rng.gen_bool(0.5) {
                            if rng.gen_bool(0.5) {
                                if rect.bottom() == h { continue; }
                                rect.h += 1;
                            } else {
                                if rect.bottom() - 1  == rect.y { continue; }
                                rect.h -= 1;
                            }
                        } else {
                            if rng.gen_bool(0.5) {
                                if rect.right() == w { continue; }
                                rect.w += 1;
                            } else {
                                if rect.right() - 1  == rect.x { continue; }
                                rect.w -= 1;
                            }
                        }
                    }
                    rects.remove(rng.gen_range(0, rects.len()));
                },
                _ => {} 
            }

            let result = eval(target, &mut rects, &mut gray_image, true, &mut rng);
            let score = result.cost + result.similarity;

            if score < best_result.cost + best_result.similarity {
                best_result = result;
                best_rects = rects.clone();
            }

            next.push(PainterState { rects, score });
        }

        next.sort_by(|a, b| a.score.cmp(&b.score));
        current = next;
    }

    eval(target, &mut best_rects, &mut gray_image, false, &mut rng)
}

fn eval<R:Rng>(
    target:&RgbaImage, 
    rects:&mut Vec<Rectangle>, 
    gray_image:&mut GrayImage,
    fast:bool,
    rng:&mut R
) -> PainterResult {
    gray_image.fill(0u8);

    let h = target.height() as i32;
    let w = target.width() as i32;
    for (i, rect) in rects.iter().enumerate() {
        draw_filled_rect_mut(
            gray_image, 
            Rect::at(rect.x, rect.y).of_size(rect.w as u32, rect.h as u32), 
            Luma([(i + 1) as u8])
        );
    }
    
    let mut fill_colors = Vec::new();
    let mut fill_size  = Vec::new();
    for _ in 0..rects.len() + 1 {
        fill_colors.push([127.5, 127.5, 127.5, 127.5]);
        fill_size.push(0.0);
    }
    for x in 0..gray_image.width() {
        for y in 0..gray_image.height() {
            fill_size[gray_image.get_pixel(x, y)[0] as usize] += 1.0;
        }
    }

    let mut power = 127.0;
    let (len, scale) = if fast { (3, 0.28) } else { (18, 0.65) };
    
    for _ in 0..len {
        for x in 0..gray_image.width() {
            for y in 0..gray_image.height() {
                let index = gray_image.get_pixel(x, y)[0] as usize;
                let size = fill_size[index];
                let color = fill_colors[index];
                let color2 = target.get_pixel(
                    x as u32,
                    399 - y as u32
                );
                let color3 = Rgba::from([
                    color2[0] as f64 - color[0],
                    color2[1] as f64 - color[1],
                    color2[2] as f64 - color[2],
                    color2[3] as f64 - color[3]
                ]);
                let d = (
                    color3[0] * color3[0] +
                    color3[1] * color3[1] +
                    color3[2] * color3[2] +
                    color3[3] * color3[3]
                ).sqrt() * size / power;
                
                fill_colors[index][0] += color3[0] / d;
                fill_colors[index][1] += color3[1] / d;
                fill_colors[index][2] += color3[2] / d;
                fill_colors[index][3] += color3[3] / d;
            }
        }
        power *= scale;
    }

    let mut out_image= RgbaImage::new(target.width(), target.height());
    out_image.fill(255);
    let mut state = State::new(out_image);
    if fill_size[0] > 0.0 {
        let color = fill_colors[0];
        state.apply_command(&Command::Color(
            vec![state.get_last_node()], 
            Rgba([
                color[0].round() as u8,
                color[1].round() as u8,
                color[2].round() as u8,
                color[3].round() as u8,
            ])
        ));
    }
    for (i, rect) in rects.iter().enumerate() {
        if fill_size[i + 1] == 0.0 { continue; }
        let mut commands =  Vec::new();
        let mut id = vec![state.get_last_node()];

        if rect.x == 0 && rect.y == 0 && rect.right() == w && rect.bottom() == h {
            continue;
        } else if rect.x == 0 && rect.y == 0 && rect.right() == w {
            commands.push(Command::LineCut(id.clone(), false, rect.bottom()));
            id.push(0);
        } else if rect.x == 0 && rect.y == 0 && rect.bottom() == h {
            commands.push(Command::LineCut(id.clone(), true , rect.right()));
            id.push(0);
        } else if rect.x == 0 && rect.right() == w && rect.bottom() == h {
            commands.push(Command::LineCut(id.clone(), false, rect.y));
            id.push(1);
        } else if rect.y == 0 && rect.right() == w && rect.bottom() == h {
            commands.push(Command::LineCut(id.clone(), true, rect.x));
            id.push(1);
        } else if rect.x == 0 && rect.right() == w {
            if rect.y < h - rect.bottom() {
                commands.push(Command::LineCut(id.clone(), false, rect.y));
                id.push(1);
                commands.push(Command::LineCut(id.clone(), false, rect.bottom()));
                id.push(0);
            } else {
                commands.push(Command::LineCut(id.clone(), false, rect.bottom()));
                id.push(0);
                commands.push(Command::LineCut(id.clone(), false, rect.y));
                id.push(1);
            }
        } else if rect.y == 0 && rect.bottom() == h {
            if rect.x < h - rect.right() {
                commands.push(Command::LineCut(id.clone(), false, rect.x));
                id.push(1);
                commands.push(Command::LineCut(id.clone(), false, rect.right()));
                id.push(0);
            } else {
                commands.push(Command::LineCut(id.clone(), false, rect.right()));
                id.push(0);
                commands.push(Command::LineCut(id.clone(), false, rect.x));
                id.push(1);
            }
        } else if rect.x == 0 && rect.y == 0 {
            commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
            id.push(0);
        } else if rect.x == 0 && rect.bottom() == h {
            commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
            id.push(3);
        } else if rect.y == 0 && rect.right() == w {
            commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
            id.push(1);
        } else if rect.right() == w && rect.bottom() == h {
            commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
            id.push(2);
        } else if rect.x == 0 {
            if rect.y > h - rect.bottom() {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                id.push(0);
                commands.push(Command::LineCut(id.clone(), false, rect.y));
                id.push(1);
            } else {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                id.push(3);
                commands.push(Command::LineCut(id.clone(), false, rect.bottom()));
                id.push(0);
            }
        } else if rect.y == 0 {
            if rect.x > w - rect.right() {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                id.push(0);
                commands.push(Command::LineCut(id.clone(), true, rect.x));
                id.push(1);
            } else {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                id.push(1);
                commands.push(Command::LineCut(id.clone(), true , rect.right()));
                id.push(0);
            }
        } else if rect.right() == w {
            if rect.y > h - rect.bottom() {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                id.push(1);
                commands.push(Command::LineCut(id.clone(), false, rect.y));
                id.push(1);
            } else {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                id.push(2);
                commands.push(Command::LineCut(id.clone(), false, rect.bottom()));
                id.push(0);
            }
        } else if rect.bottom() == h {
            if rect.x > w - rect.right() {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                id.push(3);
                commands.push(Command::LineCut(id.clone(), true, rect.x));
                id.push(1);
            } else {
                commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                id.push(2);
                commands.push(Command::LineCut(id.clone(), true , rect.right()));
                id.push(0);
            }
        } else {
            let size = [
                rect.x * rect.y,
                (w - rect.right()) * rect.y,
                (w - rect.right()) * (h - rect.bottom()),
                rect.x * h - rect.bottom()
            ];
            let mut min = size[0];
            let mut index = 0;
            for i in 1..size.len() {
                if min > size[i] {
                    min = size[i];
                    index = i;
                }
            }
            match index {
                0 => {
                    if (w - rect.right()) < (h - rect.bottom()) {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                        id.push(3);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                        id.push(1);
                    } else {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                        id.push(1);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                        id.push(3);
                    }
                },
                1 => {
                    if rect.x < (h - rect.bottom()) {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                        id.push(2);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                        id.push(0);
                    } else {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                        id.push(0);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                        id.push(2);
                    }
                },
                2 => {
                    if rect.x < rect.y {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                        id.push(1);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                        id.push(3);
                    } else {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                        id.push(3);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                        id.push(1);
                    }
                },
                3 => {
                    if (w - rect.right()) < rect.y {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                        id.push(0);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                        id.push(2);
                    } else {
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                        id.push(2);
                        commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                        id.push(0);
                    }
                },

                _ => {}
            }
        }

        for command in &commands {
            state.apply_command(command);
        }
        let color = fill_colors[i + 1];
        state.apply_command(&Command::Color(
            id,
            Rgba([
                color[0].round() as u8,
                color[1].round() as u8,
                color[2].round() as u8,
                color[3].round() as u8,
            ])
        ));
        if i + 1 == rects.len() { continue; }
        commands.reverse();
        for command in &commands {
            state.revert_command(command);
        }
    }

    for j in 0..rects.len() {
        let i = rects.len() - j - 1;
        if fill_size[i + 1] == 0.0 { 
            if rng.gen_bool(0.5) { rects.remove(i); }
        }
    }

    PainterResult { 
        commands: state.commands,
        cost: state.cost,
        similarity: similarity(&state.image, target),
        image: state.image,
    }
}
