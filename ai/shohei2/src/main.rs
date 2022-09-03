
use std::env::args;
use std::fs::File;
use std::io::BufReader;
use image::io::Reader as ImageReader;
use std::io::prelude::*;

mod data;
mod draw;
mod writer;
mod reader;


fn main() -> std::io::Result<()>  {
    let arg:Vec<String> = args().collect();

    let mut problem = 18;
    let mut initial_file_path = format!("initial/{}.isl", problem).to_owned();
    if arg.len() >= 2 {
        problem = arg[1].parse().unwrap();
    }
    if arg.len() >= 3 {
        initial_file_path = arg[2].to_owned();
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
    
    println!("{}", string);

    Ok(())
}
