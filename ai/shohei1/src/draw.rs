
use crate::data::*;
use image::RgbaImage;
use image::Rgba;
use rand::thread_rng;
use rand::Rng;
use rand::rngs::ThreadRng;

pub fn fill_color(commands:&mut Vec<Command>, image:&RgbaImage) {
    let len = commands.len();
    let mut rng = thread_rng();
    let mut tree = Tree::from_commands(commands);
    
    let mut root_size = match &tree {
        Tree::Node(children) => children.len(),
        _ => panic!("failed get root"),
    };
    for j in 0..len {
        let i = len - j - 1;
        match &commands[i] {
            Command::PointCut(id, point) => {
                fill_large_rects(&mut tree, &mut id.clone(), 4, &mut rng, image, i + 1, commands);
            }
            Command::LineCut(id, is_x, pos) => {
                fill_large_rects(&mut tree, &mut id.clone(), 2, &mut rng, image, i + 1, commands);
            }
            Command::Color(id, color) => {}
            Command::Merge(_, _) => {
                root_size -= 1;
                _fill_color(&mut tree, &mut [root_size].to_vec(), &mut rng, image, i + 1, commands);
            }
        }
    }

    let mut id = [0].to_vec();
    _fill_color(&mut tree, &mut id, &mut rng, image, 0, commands);
}

fn fill_large_rects<R:Rng + Sized>(
    tree:&mut Tree, 
    base_id:&mut Id, 
    len:usize,
    rng:&mut R, 
    image:&RgbaImage,
    index:usize,
    commands:&mut Vec<Command>
) {
    let mut min_id = 0;
    let mut min_size = 400 * 400;
    for i in 0..len {
        let mut id = base_id.clone();
        id.push(i);
        
        let mut rect_ids = Vec::new();
        let mut rects = Vec::new();

        tree.find(&id).get_empty_rects(&mut id, &mut rects, &mut rect_ids);
        if rects.len() == 0 { continue; }
        let mut size = 0;
        for rect in rects {
            size += rect.w * rect.h;
        }
        if size <= min_size {
            min_size = size;
            min_id = i;
        }
    }

    for i in 0..len {
        let mut id = base_id.clone();
        id.push(i);
        if min_id == i { continue; }
        _fill_color(tree, &mut id, rng, image, index, commands);
    }
}

fn _fill_color<R:Rng + Sized>(
    tree:&mut Tree, 
    id:&mut Id, 
    rng:&mut R, 
    image:&RgbaImage,
    index:usize,
    commands:&mut Vec<Command>
)  {
    let mut rect_ids = Vec::new();
    let mut rects = Vec::new();
    tree.find(&id).get_empty_rects(id, &mut rects, &mut rect_ids);
    if rects.len() == 0 { return; }
    let color = to_u8_color(get_rects_color(&rects, image, rng));
    tree.find_mut(id).fill_color(color);
    commands.insert(index, Command::Color(id.clone(), color));
}

fn to_u8_color(color:Rgba<f64>) -> Rgba<u8> {
    Rgba([
        color[0].round() as u8,
        color[1].round() as u8,
        color[2].round() as u8,
        color[3].round() as u8
    ])
}

fn get_rects_color<R:Rng + Sized>(rects:&Vec<Rectangle>, image:&RgbaImage, rng:&mut R) -> Rgba<f64> {
    let mut color = Rgba([0.0, 0.0, 0.0, 0.0]);
    let mut weight = 0.0;
    for rect in rects {
        let color2 = get_rect_color(rect, image, rng);
        let size = (rect.w * rect.h) as f64;
        color[0] = (weight * color[0] + size * color2[0]) / (weight + size);
        color[1] = (weight * color[1] + size * color2[1]) / (weight + size);
        color[2] = (weight * color[2] + size * color2[2]) / (weight + size);
        color[3] = (weight * color[3] + size * color2[3]) / (weight + size);
        weight += size;
    }
    color
}

fn get_rect_color<R:Rng + Sized>(rect:&Rectangle, image:&RgbaImage, rng:&mut R) -> Rgba<f64> {
    let mut color = Rgba([0.0, 0.0, 0.0, 0.0]);
    let mut weight = 0.0;
    for x in rect.x..rect.right() {
        for y in rect.y..rect.bottom() {
            let color2 = image.get_pixel(
                x as u32,
                399 - y as u32
            );
            color[0] = (weight * color[0] + color2[0] as f64) / (weight + 1.0);
            color[1] = (weight * color[1] + color2[1] as f64) / (weight + 1.0);
            color[2] = (weight * color[2] + color2[2] as f64) / (weight + 1.0);
            color[3] = (weight * color[3] + color2[3] as f64) / (weight + 1.0);
            weight += 1.0;
        }
    }
    color
}

fn get_rects(id:Id) -> Vec<Rectangle> {
    let result = Vec::new();
    result
}
