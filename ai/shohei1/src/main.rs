
use std::env::args;
use std::fs::File;
use std::io::BufWriter;
use image::io::Reader as ImageReader;
use std::io::prelude::*;
use serde::{Serialize, Deserialize};
use chrono::Utc;

mod data;
mod draw;
mod writer;
mod reader;


fn main() -> std::io::Result<()>  {
    let arg:Vec<String> = args().collect();

    let mut problem = 25;
    if arg.len() >= 2 {
        problem = arg[1].parse().unwrap();
    }
    
    let mut initial_file_path = format!("initial/{}.isl", problem).to_owned();
    if arg.len() >= 3 {
        initial_file_path = arg[2].to_owned();
    }
    let mut program_name = "shohei1".to_owned();
    if arg.len() >= 4 {
        program_name = arg[3].to_owned();
    }
    let mut contents = String::new();
    if std::path::Path::new(&initial_file_path).exists() {
        let mut f = File::open(initial_file_path).expect("file not found");
        f.read_to_string(&mut contents);
    }
    
    let mut commands = reader::read(&contents);
    let image = ImageReader::open(format!("../../problems/{}.png", problem)).unwrap().decode().unwrap().to_rgba8();

    draw::fill_color(&mut commands, &image);
    let mut string = String::new();
    writer::write(&mut string, &commands);

    let text = Utc::now().format("%Y%m%d%H%M%S%f3").to_string();
    let mut file = File::create(format!("../../solutions/{}-{}-{}.isl", problem, program_name, text))?;
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