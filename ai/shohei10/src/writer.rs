use crate::data::*;

pub fn write(output:&mut String, commands:&Vec<Command>) {
    for command in commands {
        output.push_str("\n");
        match command {
            Command::PointCut(id, point) => {
                output.push_str(&format!(
                    "cut [{}] [{}]",
                    id_to_string(id),
                    *point
                ));
            }
            Command::LineCut(id, is_x, pos) => {
                output.push_str(&format!(
                    "cut [{}] [{}] [{}]",
                    id_to_string(id),
                    if *is_x { "x" } else { "y" },
                    *pos
                ));
            }
            Command::Color(id, color) => {
                output.push_str(&format!(
                    "color [{}] [{}]",
                    id_to_string(id),
                    color_to_string(color)
                ));
            }
            Command::Merge(id0, id1) => {
                output.push_str(&format!(
                    "merge [{}] [{}]",
                    id_to_string(id0),
                    id_to_string(id1)
                ));
            }
        }
    }
}