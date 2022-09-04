

use crate::data::*;
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
}

pub fn solve(target:&RgbaImage) -> PainterResult {
    
    let best_result = PainterResult {
        commands: Vec::new(),
        cost: 0,
        similarity: i64::MAX
    };
    let initial_state = PainterState {
        rects: Vec::new(),
        score: i64::MAX
    };

    best_result
}

fn eval(
    target:&RgbaImage, 
    rects:&Vec<Rectangle>, 
    grayImage:&mut GrayImage,
) -> PainterResult {
    grayImage.fill(0u8);

    for (i, rect) in rects.iter().enumerate() {
        draw_filled_rect_mut(
            grayImage, 
            Rect::at(rect.x, target.height() as i32 - rect.bottom()).of_size(rect.w as u32, rect.h as u32), 
            Luma([(i + 1) as u8])
        );
    }
    
    let outImage= RgbaImage::new(target.width(), target.height());
    outImage.fill(255);
    let state = State::new(outImage);
    

    PainterResult { 
        commands: 
        cost: 
        similarity: 
    }
}
