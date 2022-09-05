

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
    let mut initial_state = PainterState {
        rects: vec![
            Rectangle::new(80, 103, 208, 208),
            Rectangle::new(73, 148, 225, 128),
            Rectangle::new(118, 103, 142, 222),
            Rectangle::new(100, 133, 168, 168),
            Rectangle::new(30, 26, 61, 64),
            Rectangle::new(243, 14, 86, 82),
            Rectangle::new(325, 144, 48, 66),
            Rectangle::new(283, 342, 79, 37),
            Rectangle::new(10, 309, 80, 64),
            Rectangle::new(148, 164, 70, 102),
            Rectangle::new(144, 164, 65, 52),
            Rectangle::new(231, 22, 65, 73),
            Rectangle::new(52, 60, 52, 60),
        ],
        score: i64::MAX
    };
    let mut gray_image = GrayImage::new(target.width(), target.height());

    let mut current = vec![initial_state];

    let beam_w = 25;
    let w = target.width() as i32;
    let h = target.height() as i32;
    for step in 0..700 {
        println!("step {}", step);
        let mut next = Vec::new();
        let size = usize::min(current.len(), (beam_w as f64 / 2.5) as usize);
        if size == 0 { break; }
        for i in 0..size {
            next.push(current[i].clone());
        }
        for i in 0..beam_w {
            let source = &current[i % size];
            let mut rects = source.rects.clone();

            match rng.gen_range(0, 2) {
                0 => {
                    let x0 = rng.gen_range(0, target.width () as i32);
                    let x1 = rng.gen_range(0, target.width () as i32);
                    let y0 = rng.gen_range(0, target.height() as i32);
                    let y1 = rng.gen_range(0, target.height() as i32);
                    let ax = i32::min(x0, x1);
                    let bx = i32::max(x0, x1);
                    let ay = i32::min(y0, y1);
                    let by = i32::max(y0, y1);
                    
                    if ax == bx { continue; }
                    if ay == by { continue; }
                    if (by - ay) * (bx - ax) < 20 { continue; }
                    rects.insert(rng.gen_range(0, rects.len() + 1), Rectangle { 
                        x: ax,
                        y: ay,
                        w: bx - ax,
                        h: by - ay,
                    });
                }
                1 => {
                    if rects.len() < 1 { continue; }
                    let mut rect = rects[rng.gen_range(0, rects.len())];
                    let mut offset = rng.gen_range(1, 20);
                    if rng.gen_bool(0.5) {
                        if rng.gen_bool(0.5) {
                            if rng.gen_bool(0.5) {
                                if rect.y + offset >= rect.bottom() - 1 { continue; }
                                rect.y += offset;
                            } else {
                                if rect.y - offset < 0 { continue; }
                                rect.y -= offset;
                            }
                        } else {
                            if rng.gen_bool(0.5) {
                                if rect.x + offset > rect.right() { continue; }
                                rect.x += 1;
                            } else {
                                if rect.x - offset < 0 { continue; }
                                rect.x -= offset;
                            }
                        }
                    } else {
                        if rng.gen_bool(0.5) {
                            if rng.gen_bool(0.5) {
                                if rect.bottom() + offset > h { continue; }
                                rect.h += offset;
                            } else {
                                if rect.bottom() - offset <= rect.y { continue; }
                                rect.h -= offset;
                            }
                        } else {
                            if rng.gen_bool(0.5) {
                                if rect.right() + offset > w { continue; }
                                rect.w += offset;
                            } else {
                                if rect.right() - offset <= rect.x { continue; }
                                rect.w -= offset;
                            }
                        }
                    }
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
        fill_colors.push([127.5, 127.5, 127.5, 255.0]);
        fill_size.push(0.0);
    }
    for x in 0..gray_image.width() {
        for y in 0..gray_image.height() {
            fill_size[gray_image.get_pixel(x, y)[0] as usize] += 1.0;
        }
    }

    let mut power = 130.0;
    let (len, scale) = if fast { (4, 0.33) } else { (20, 0.65) };
    
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
            let mut max = size[0];
            let mut index = 0;
            for i in 1..size.len() {
                if max < size[i] {
                    max = size[i];
                    index = i;
                }
            }
            let mut vec = Vec::new();
            for i in 0..size.len() {
                if index as usize != i { vec.push(i); }
            }
            match vec[rng.gen_range(0, 3)] {
                0 => {
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                    id.push(2);
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                    id.push(0);
                },
                1 => {
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                    id.push(3);
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                    id.push(1);
                },
                2 => {
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.bottom()}));
                    id.push(0);
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.y}));
                    id.push(2);
                },
                3 => {
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.x, y:rect.bottom()}));
                    id.push(1);
                    commands.push(Command::PointCut(id.clone(), Point{x:rect.right(), y:rect.y}));
                    id.push(3);
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

    let len = rects.len();
    for j in 0..len {
        let i =  len - j - 1;
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

fn find_x_boundary<R:Rng>(x:i32, y:i32, target:&RgbaImage, rng:&mut R) -> (f64, i32) {
    let mut min = 0;
    let mut max: i32 = x;
    let pixel = target.get_pixel(x as u32, y as u32);
    let mut max_diff = 0.0;
    while min < max {
        let value = (min as f64 + ((max - min) as f64) * rng.gen_range(0.1, 0.6)) as i32;
        let diff = compare(target.get_pixel(value as u32, y as u32), pixel);
        if diff > max_diff {  max_diff = diff; }
        if rng.gen_range(0.0, 1.0) * diff / rng.gen_range(1.0, 200.0) < 1.0 {
            max = value - 1;
        } else {
            min = value + 1;
        }
    }
    (max_diff, min)
}

fn find_right_boundary<R:Rng>(x:i32, y:i32, target:&RgbaImage, rng:&mut R) -> (f64, i32)   {
    let mut min = x;
    let mut max: i32 = target.width() as i32 - 1;
    let pixel = target.get_pixel(x as u32, y as u32);
    let mut max_diff = 0.0;
    while min < max {
        let value = (min as f64 + ((max - min) as f64) * rng.gen_range(0.4, 0.9)) as i32;
        let diff = compare(target.get_pixel(value as u32, y as u32), pixel);
        if diff > max_diff {  max_diff = diff; }
        if rng.gen_range(0.0, 1.0) * diff / rng.gen_range(1.0, 200.0) < 1.0 {
            min = value + 1;
        } else {
            max = value - 1;
        }
    }
    (max_diff, min)
}

fn find_y_boundary<R:Rng>(x:i32, y:i32, target:&RgbaImage, rng:&mut R) -> (f64, i32) {
    let mut min = 0;
    let mut max: i32 = y;
    let pixel = target.get_pixel(x as u32, y as u32);
    let mut max_diff = 0.0;
    while min < max {
        let value = (min as f64 + ((max - min) as f64) * rng.gen_range(0.1, 0.6)) as i32;
        let diff = compare(target.get_pixel(x as u32, value as u32), pixel);
        if diff > max_diff {  max_diff = diff; }
        if rng.gen_range(0.0, 1.0) * diff / rng.gen_range(1.0, 200.0) < 1.0 {
            max = value - 1;
        } else {
            min = value + 1;
        }
    }
    (max_diff, min)
}

fn find_bottom_boundary<R:Rng>(x:i32, y:i32, target:&RgbaImage, rng:&mut R) -> (f64, i32)   {
    let mut min = y;
    let mut max: i32 = target.height() as i32 - 1;
    let pixel = target.get_pixel(x as u32, y as u32);
    let mut max_diff = 0.0;
    while min < max {
        let value = (min as f64 + ((max - min) as f64) * rng.gen_range(0.4, 0.9)) as i32;
        let diff = compare(target.get_pixel(x as u32, value as u32), pixel);
        if diff > max_diff {  max_diff = diff; }
        if rng.gen_range(0.0, 1.0) * diff / rng.gen_range(1.0, 200.0) < 1.0 {
            min = value + 1;
        } else {
            max = value - 1;
        }
    }
    (max_diff, min)
}

fn compare(p0:&Rgba<u8>, p1:&Rgba<u8>) -> f64 {
    let r = p0[0] as f64 - p1[0] as f64;
    let g = p0[1] as f64 - p1[1] as f64;
    let b = p0[2] as f64 - p1[2] as f64;
    let a = p0[3] as f64 - p1[3] as f64;
    r * r + g * g + b * b + a * a
}
