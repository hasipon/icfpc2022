
use std::env::args;
use std::fs::File;
use std::io::BufWriter;
use image::io::Reader as ImageReader;
use std::io::prelude::*;
use serde::{Serialize, Deserialize};
use chrono::Utc;

use image::RgbImage;
use eval::similarity;
use data::*;

mod data;
mod draw;
mod writer;
mod reader;
mod eval;
mod solver;
mod painter;

fn main() -> std::io::Result<()>  {
    let arg:Vec<String> = args().collect();

    let mut problem = 6;
    if arg.len() >= 2 {
        problem = arg[1].parse().unwrap();
    }
    let mut width = 10;
    if arg.len() >= 3 {
        width = arg[2].parse().unwrap();
    }
    let mut depth = 8000;
    if arg.len() >= 4 {
        depth = arg[3].parse().unwrap();
    }
    
    let image = ImageReader::open(format!("../../problems/{}.png", problem)).unwrap().decode().unwrap().to_rgb8();

    let state = painter::solve(&image, width, depth);

    state.image.save("output.png").unwrap();
    let mut string = String::new();
    writer::write(&mut string, &state.commands);

    println!("{}: similarity:{} cost:{} total:{}", problem, state.similarity, state.cost, state.similarity + state.cost);
    let text = Utc::now().format("%Y%m%d%H%M%S%f3").to_string();
    let file = File::create(format!("../../solutions/{}-shohei13_{}_{}-{}.isl", problem, width, depth, text))?;
    let mut writer = BufWriter::new(file);
    writer.write_all(string.as_bytes())?;
    writer.flush()?;

    Ok(())
}

#[derive(Serialize, Deserialize, Debug)]
struct Result {
    moves: String,
    id: String,
}