

use crate::data::*;
use crate::eval::similarity;
use image::Luma;
use image::Rgba;
use image::RgbaImage;
use image::GrayImage;
use imageproc::drawing::draw_filled_rect_mut;
use imageproc::rect::Rect;

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
    let mut best_result = PainterResult {
        commands: Vec::new(),
        cost: 0,
        similarity: i64::MAX,
        image:RgbaImage::new(1, 1)
    };
    let mut initial_state = PainterState {
        rects: Vec::new(),
        score: i64::MAX
    };
    let mut gray_image = GrayImage::new(target.width(), target.height());
    initial_state.rects.push(Rectangle { x: 0, y: 200, w: 50, h: 200 });
    eval(target, &initial_state.rects, &mut gray_image)
}

fn eval(
    target:&RgbaImage, 
    rects:&Vec<Rectangle>, 
    gray_image:&mut GrayImage,
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

    let mut power = 130.0;
    for _ in 0..15 {
        for x in 0..gray_image.width() {
            for y in 0..gray_image.height() {
                let index = gray_image.get_pixel(x, y)[0] as usize;
                let size = fill_size[index];
                let mut color = fill_colors[index];
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
        power *= 0.57;
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
        }
        //todo 

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
        commands.reverse();
        for command in &commands {
            state.revert_command(command);
        }
    }

    PainterResult { 
        commands: state.commands,
        cost: state.cost,
        similarity: similarity(&state.image, target),
        image: state.image,
    }
}
