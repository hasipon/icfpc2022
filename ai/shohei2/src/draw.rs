
use crate::data::*;
use image::RgbaImage;
use image::Rgba;
use rand::thread_rng;
use rand::Rng;

pub fn calc_colored_state(state:&State, image:&RgbaImage) -> State {
    let mut commands = state.commands.clone();

    insert_color_commands(&mut commands, image);
    
    let mut result = State::new(RgbaImage::new(image.width(), image.height()));
    result.apply_commands(&commands);
    result
}

pub fn insert_color_commands(commands:&mut Vec<Command>, image:&RgbaImage) {
    let len = commands.len();
    let mut rng = thread_rng();
    let mut state = State::new(RgbaImage::new(image.width(), image.height()));
    state.apply_commands(commands);

    let mut root_size = match &state.tree {
        Tree::Node(_, children) => children.len(),
        _ => panic!("failed get root"),
    };
    for j in 0..len {
        let i = len - j - 1;
        match &commands[i] {
            Command::PointCut(id, _) => {
                fill_large_rects(&mut state.tree, &mut id.clone(), 4, &mut rng, image, i + 1, commands);
            }
            Command::LineCut(id, _, _) => {
                fill_large_rects(&mut state.tree, &mut id.clone(), 2, &mut rng, image, i + 1, commands);
            }
            Command::Color(_, _) => {}
            Command::Merge(_, _) => {
                root_size -= 1;
                _fill_color(&mut state.tree, &mut [root_size].to_vec(), &mut rng, image, i + 1, commands);
            }
        }
    }

    let mut id = [0].to_vec();
    _fill_color(&mut state.tree, &mut id, &mut rng, image, 0, commands);
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

        let parent = tree.find(&id);
        parent.get_empty_rects(&mut id, &mut rects, &mut rect_ids);
        if rects.len() == 0 { continue; }
        let mut size = match &parent {
            Tree::Node(rect, _) | Tree::Leaf(rect, _) => rect.w * rect.h,
            _ => panic!("failed get rect"),
        };
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
    
    let mut color0 = Rgba::from([127.0, 127.0, 127.0, 150.0]);
    let mut power = 135.0;
    for _ in 0..18 {
        let color1 = get_rects_color(&rects, image, color0, power, rng);
        color0 = color1;
        power *= 0.65;
    }
    let result_color = to_u8_color(color0);
    tree.find_mut(id).fill_color(result_color);
    commands.insert(index, Command::Color(id.clone(), result_color));
}

fn to_u8_color(color:Rgba<f64>) -> Rgba<u8> {
    Rgba([
        color[0].round() as u8,
        color[1].round() as u8,
        color[2].round() as u8,
        color[3].round() as u8
    ])
}

fn get_rects_color<R:Rng + Sized>(rects:&Vec<Rectangle>, image:&RgbaImage, base_color:Rgba<f64>, power:f64, rng:&mut R) -> Rgba<f64> {
    let mut size = 0.0;
    for rect in rects {
        size += (rect.w * rect.h) as f64;
    }
    
    let mut color:Rgba<f64> = base_color.clone();
    for rect in rects {
        for x in rect.x..rect.right() {
            for y in rect.y..rect.bottom() {
                let mut color2 = image.get_pixel(
                    x as u32,
                    399 - y as u32
                );
                let color3 = Rgba::from([
                    color2[0] as f64 - base_color[0],
                    color2[1] as f64 - base_color[1],
                    color2[2] as f64 - base_color[2],
                    color2[3] as f64 - base_color[3]
                ]);
                let mut d = (
                    color3[0] * color3[0] +
                    color3[1] * color3[1] +
                    color3[2] * color3[2] +
                    color3[3] * color3[3]
                ).sqrt() * size / power;

                color[0] += color3[0] / d;
                color[1] += color3[1] / d;
                color[2] += color3[2] / d;
                color[3] += color3[3] / d;
            }
        }
    }
    color
}

fn get_rect_color<R:Rng + Sized>(rect:&Rectangle, image:&RgbaImage, base_color:Rgba<f64>, rng:&mut R) -> Rgba<f64> {
    let mut color = Rgba([0.0, 0.0, 0.0, 0.0]);
    let mut weight = 0.0;
    
    color
}

fn get_rects(id:Id) -> Vec<Rectangle> {
    let result = Vec::new();
    result
}
