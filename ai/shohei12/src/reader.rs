
use crate::data::*;
pub fn read(str:&str) -> Vec<Command> {
    let mut result = Vec::new();

    for (line_number, line) in str.lines().enumerate() {
        let mut tokens:Vec<String> = line.split("[").map(|x| x.trim().to_owned()).collect();
        if tokens.len() <= 0 {
            continue;  
        }
        let title = tokens.remove(0);
        for token in &mut tokens {
            token.pop();
        }
        let command = match title.as_str() {
            "cut" => {
                if tokens.len() == 2 {
                    Command::PointCut(
                        id_from_str(&tokens[0]),
                        Point::from_str(&tokens[1])
                    )
                } else {
                    Command::LineCut(
                        id_from_str(&tokens[0]),
                        tokens[1] == "x" || tokens[1] == "X",
                        tokens[2].parse().unwrap()
                    )
                }
            }
            "merge" => {
                Command::Merge(
                    id_from_str(&tokens[0]),
                    id_from_str(&tokens[1])
                )
            }
            "color" => {
                continue;
                /*
                Command::Color(
                    id_from_str(&tokens[0]),
                    color_from_str(&tokens[1])
                )*/
            }
            _ => continue,
        };
        result.push(command);
    }

    result
}