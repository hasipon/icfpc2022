
use image::Rgb;
use image::RgbImage;
use imageproc::drawing::draw_filled_rect_mut;
use std::mem;
use std::cmp;
use std::vec;
use imageproc::rect::Rect;

#[derive(Clone)]
pub struct State {
    pub tree    : Tree,
    pub image   : RgbImage,
    pub cost    : i64,
    pub size    : f64,
    pub commands: Vec<Command>,
}

impl State {
    pub fn new(
        image:RgbImage,
    ) -> Self {
        State {
            size: (image.width() * image.height()) as f64,
            tree:Tree::Node(Rectangle{
                x: 0,
                y: 0,
                w: image.width() as i32,
                h: image.height() as i32,
            }, vec![
                Tree::Leaf(Rectangle{
                    x: 0,
                    y: 0,
                    w: image.width() as i32,
                    h: image.height() as i32,
                }, Option::None)
            ]),
            image: image,
            cost: 0,
            commands: Vec::new()
        }
    }

    pub fn apply_commands(&mut self, commands:&Vec<Command>) {
        for command in commands {
            self.apply_command(command);
        }
    }
    pub fn revert_command(&mut self, command:&Command) {

        match command {
            Command::PointCut(id, point) => {

                let mut id0 = id.clone();
                let mut id1 = id.clone();
                let mut id2 = id.clone();
                let mut id3 = id.clone();
                id0.push(0);
                id1.push(1);
                id2.push(2);
                id3.push(3);
                let mut redirect_count = 0;
                id0 = self.redirect_merge(id0, &mut redirect_count);
                id1 = self.redirect_merge(id1, &mut redirect_count);
                id2 = self.redirect_merge(id2, &mut redirect_count);
                id3 = self.redirect_merge(id3, &mut redirect_count);

                let rect = self.tree.find(id).get_rect();
                let x1 = cmp::min(point.x - rect.x, rect.right() - point.x);
                let y1 = cmp::min(point.y - rect.y, rect.bottom() - point.y);
                if x1 < y1 {
                    self.apply_command(&Command::Merge(id0, id1));
                    self.apply_command(&Command::Merge(id2, id3));
                } else {
                    self.apply_command(&Command::Merge(id0, id3));
                    self.apply_command(&Command::Merge(id1, id2));
                }
                let last = self.get_last_node();
                self.apply_command(&Command::Merge(vec![last], vec![last - 1]));
            }
            Command::LineCut(id, is_x, pos) => {
                let mut id0 = id.clone();
                let mut id1 = id.clone();
                id0.push(0);
                id1.push(1);
                let mut redirect_count = 0;
                id0 = self.redirect_merge(id0, &mut redirect_count);
                id1 = self.redirect_merge(id1, &mut redirect_count);
                self.apply_command(&Command::Merge(id0, id1));
            }
            _ => panic!("cant revert"),
        }
    }
    pub fn redirect_merge(&mut self, id:Id, redirect_count:&mut usize) -> Id {
        match &self.tree.find(&id) {
            Tree::Leaf(_, _) => {
                id
            }
            _ => {
                *redirect_count += 1;
                vec![&self.get_last_node() - *redirect_count + 1]
            }
        }
    }

    pub fn apply_command(&mut self, command:&Command) {
        self.commands.push(command.clone());
        self.process_command(command);
    }
    pub fn process_command(&mut self, command:&Command) {
        match command {
            Command::PointCut(id, point) => {
                let target = self.tree.find_mut(&id);
                self.cost += (10.0 * (self.size / target.size() as f64)).round() as i64;

                target.point_cut(*point);    
            }
            Command::LineCut(id, is_x, pos) => {
                let target = self.tree.find_mut(&id);
                self.cost += (7.0 * (self.size / target.size() as f64)).round() as i64;

                target.line_cut(*is_x, *pos);
                
            }
            Command::Color(id, color) => {
                let target = self.tree.find_mut(&id);
                self.cost += (5.0 * (self.size / target.size() as f64)).round() as i64;

                target.color(&mut self.image, *color);
            }
            Command::Merge(id0, id1) => {
                let index = match &self.tree { 
                    Tree::Node(_, nodes) => nodes.len(),
                    _ => panic!("cannot merge"),
                };
                let tree = self.tree.find_mut(id0);
                let old0 = mem::replace(tree, Tree::Merged(index));
                let tree = self.tree.find_mut(id1);
                let old1 = mem::replace(tree, Tree::Merged(index));
                let size = cmp::max(old0.size(), old1.size()) as f64;
                self.cost += (1.0 * (self.size / size).round()) as i64;
                let result = match [old0, old1] {
                    [Tree::Leaf(rect0, _), Tree::Leaf(rect1, _)] => {
                        let x = cmp::min(rect0.x, rect1.x);
                        let y = cmp::min(rect0.y, rect1.y);
                        let r = cmp::max(rect0.right(), rect1.right());
                        let b = cmp::max(rect0.bottom(), rect1.bottom());
                        Tree::Leaf(Rectangle{
                            x: x,
                            y: y,
                            w: r - x,
                            h: b - y,
                        }, Option::None)
                    }
                    _ => {  
                        panic!("cannot merge: {:?} {:?}", id0, id1);
                    }
                };
                match &mut self.tree { 
                    Tree::Node(_, nodes) => nodes.push(result),
                    _ => panic!("cannot merge"),
                };
            }
        }
    }
    pub fn get_last_node(&self) -> usize {
        match &self.tree {
            Tree::Node(_, children) => children.len() - 1,
            _ => panic!("not root"),
        }
    }

}


#[derive(Clone)]
pub enum Tree {
    Leaf(Rectangle, Option<Rgb<u8>>),
    Node(Rectangle, Vec<Tree>),
    Merged(usize),
}

impl Tree {
    
    pub fn size(&self) -> i32 {
        match self {
            Tree::Leaf(rect, _) => (rect.w * rect.h),
            Tree::Node(rect, _) => (rect.w * rect.h),
            Tree::Merged(_) => 0,
        }
    }
    pub fn get_leaf_num(&self) -> i32 {
        match self {
            Tree::Node(_, nodes) => {
                nodes.iter().fold(0, |value, node| value + node.get_leaf_num())
            },
            Tree::Leaf(_, _) => 1,
            Tree::Merged(_) => 0,
        }
    }
    
    pub fn leaf_at(&self, mut index:i32) -> Id {
        let mut id = Id::new();
        self._leaf_at(&mut index, &mut id);
        id
    }

    fn _leaf_at(&self, index:&mut i32, id:&mut Id) {
        match self {
            Tree::Node(_, nodes) => {
                for (i,node) in nodes.iter().enumerate() {
                    id.push(i);
                    node._leaf_at(index, id);
                    if *index < 0 { return; }
                    id.pop();
                }
            },
            Tree::Leaf(_, _) => {
                *index -= 1;
            },
            Tree::Merged(_) => {}
        }
    }
    pub fn get_rect(&self) -> Rectangle {
        match self {
            Tree::Leaf(rect, _) | Tree::Node(rect, _)=> {
                rect.clone()
            }
            _ => {
                panic!("cant rect");
            }
    }
    }
    pub fn color(&mut self, image:&mut RgbImage, color:Rgb<u8>) {
        match self {
            Tree::Leaf(rect, _) => {
                
                draw_filled_rect_mut(
                    image, 
                    Rect::at(rect.x, image.height() as i32 - rect.bottom()).of_size(rect.w as u32, rect.h as u32), color);
                    
            }
            _ => panic!("cannot color"),
        }
    }

    pub fn line_cut(&mut self, is_x:bool, pos:i32) {
        match self {
            Tree::Leaf(rect, _) => {
                let mut nodes = Vec::new();
                if is_x {
                    nodes.push(Tree::Leaf(Rectangle{
                        x:rect.x,
                        y:rect.y,
                        w:pos - rect.x,
                        h:rect.h
                    }, Option::None));
                    nodes.push(Tree::Leaf(Rectangle{
                        x:pos,
                        y:rect.y,
                        w:rect.right() - pos,
                        h:rect.h
                    }, Option::None));
                } else {
                    nodes.push(Tree::Leaf(Rectangle{
                        x:rect.x,
                        y:rect.y,
                        w:rect.w,
                        h:pos - rect.y
                    }, Option::None));
                    nodes.push(Tree::Leaf(Rectangle{
                        x:rect.x,
                        y:pos,
                        w:rect.w,
                        h:rect.bottom() - pos
                    }, Option::None));
                }
                if nodes.len() == 0 { panic!("a") }
                *self = Tree::Node(*rect, nodes);
            }
            _ => {
                panic!("cant line cut: {} {}", is_x, pos);
            }
        }
    }
    
    pub fn point_cut(&mut self, pos:Point) {
        match self {
            Tree::Leaf(rect, _) => {
                let mut nodes = Vec::new();
                nodes.push(Tree::Leaf(Rectangle{
                    x:rect.x,
                    y:rect.y,
                    w:pos.x - rect.x,
                    h:pos.y - rect.y
                }, Option::None));
                nodes.push(Tree::Leaf(Rectangle{
                    x:pos.x,
                    y:rect.y,
                    w:rect.right() - pos.x,
                    h:pos.y - rect.y
                }, Option::None));
                nodes.push(Tree::Leaf(Rectangle{
                    x:pos.x,
                    y:pos.y,
                    w:rect.right() - pos.x,
                    h:rect.bottom() - pos.y
                }, Option::None));
                nodes.push(Tree::Leaf(Rectangle{
                    x:rect.x,
                    y:pos.y,
                    w:pos.x - rect.x,
                    h:rect.bottom() - pos.y
                }, Option::None));
                if nodes.len() == 0 { panic!("b") }
                *self = Tree::Node(*rect, nodes);
            }
            _ => {
                panic!("cant point cut: {}", pos);
            }
        }
    }
    
    pub fn find_mut(&mut self, id:&Id) -> &mut Self {
        self._find_mut(id, 0)
    }

    fn _find_mut(&mut self, id:&Id, index:usize) -> &mut Self {
        let current = self;
        if id.len() > index {
            match current {
                Tree::Node(_, children) => {
                    children[id[index]]._find_mut(id, index + 1)
                }
                _ => {
                    panic!("unmatched find: {:?}", id);
                }
            }
        }
        else
        {
            current
        }
    }
    pub fn find(&self, id:&Id) -> &Self {
        self._find(id, 0)
    }
    fn _find(&self, id:&Id, index:usize) -> &Self {
        let current = self;
        if id.len() > index {
            match current {
                Tree::Node(_, children) => {
                    children[id[index]]._find(id, index + 1)
                }
                _ => {
                    panic!("unmatched find: {:?}", id);
                }
            }
        }
        else
        {
            current
        }
    }

    pub fn get_empty_rects(&self, id:&mut Id, rects:&mut Vec<Rectangle>, ids:&mut Vec<Id>) {
        match self {
            Tree::Leaf(rect, Option::None) => {
                rects.push(*rect);
                ids  .push(id.clone());
            }
            Tree::Node(_, children) => {
                for i in 0..children.len() {
                    id.push(i);
                    children[i].get_empty_rects(id, rects, ids);
                    id.pop();
                }
            }
            Tree::Leaf(rect, Option::Some(_)) => {}
            Tree::Merged(_) => {}
        }
    }

    pub fn fill_color(&mut self, color:Rgb<u8>) {
        match self {
            Tree::Leaf(rect, leaf_color) => {
                match leaf_color {
                    Option::None => *leaf_color = Option::Some(color),
                    _ => {}
                }
            }
            Tree::Node(_, children) => {
                for child in children {
                    child.fill_color(color)
                }
            }
            Tree::Merged(_) => {}
        }
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Point {
    pub x: i32,
    pub y: i32,
}

impl Point {
    pub fn from_str(str:&str) -> Point {
        let args:Vec<i32> = str.split(",").map(|x| x.trim().parse().unwrap()).collect();
        Point {
            x: args[0],
            y: args[1],
        }
    }
}

impl std::fmt::Display for Point {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}, {}", self.x, self.y)
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Rectangle {
    pub x: i32,
    pub y: i32,
    pub w: i32,
    pub h: i32,
}

impl Rectangle {
    pub fn right(&self) -> i32 {
        self.x + self.w
    }
    pub fn bottom(&self) -> i32 {
        self.y + self.h
    }
}

pub type Id = Vec<usize>;

pub fn id_to_string(id:&Id) -> String {
    let dst: Vec<String> = id.iter().map(|x| x.to_string()).collect();
    dst.join(".")
}
pub fn id_from_str(str:&str) -> Id {
    str.split(".").map(|x| x.trim().parse().unwrap()).collect()
}
pub fn color_to_string<T:std::fmt::Display>(color:&Rgb<T>) -> String {
    format!("{},{},{},{}", color[0], color[1], color[2], 255)
}
pub fn color_from_str(str:&str) -> Rgb<u8> {
    let args:Vec<u8> = str.split(",").map(|x| x.trim().parse().unwrap()).collect();
    Rgb::from([args[0], args[1], args[2]])
}

#[derive(Debug, Clone)]
pub enum Command {
    PointCut(Id, Point),
    LineCut(Id, bool, i32),
    Merge(Id, Id),
    Color(Id, Rgb<u8>),
}
