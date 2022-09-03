
use image::Rgba;
use std::mem;
use std::cmp;

pub enum Tree {
    Leaf(Rectangle, Option<Rgba<u8>>),
    Node(Vec<Tree>),
    Merged(usize),
}

impl Tree {
    pub fn from_commands(commands:&Vec<Command>) -> Self {
        let mut nodes = Vec::new();
        nodes.push(Tree::Leaf(Rectangle{
            x: 0,
            y: 0,
            w: 400,
            h: 400,
        }, Option::None));
        let mut root = Tree::Node(nodes);

        for command in commands {
            match command {
                Command::PointCut(id, point) => {
                    root.find_mut(&id).point_cut(*point);
                }
                Command::LineCut(id, is_x, pos) => {
                    root.find_mut(&id).line_cut(*is_x, *pos);
                }
                Command::Color(id, color) => {
                    root.find_mut(&id).color(*color);
                }
                Command::Merge(id0, id1) => {
                    let index = match &root { 
                        Tree::Node(nodes) => nodes.len(),
                        _ => panic!("cannot merge"),
                    };
                    let tree = root.find_mut(id0);
                    let old0 = mem::replace(tree, Tree::Merged(index));
                    let tree = root.find_mut(id1);
                    let old1 = mem::replace(tree, Tree::Merged(index));
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
                    let index = match &mut root { 
                        Tree::Node(nodes) => nodes.push(result),
                        _ => panic!("cannot merge"),
                    };
                }
            }
        }
        root
    }

    pub fn color(&mut self, color:Rgba<u8>) {
        
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
                *self = Tree::Node(nodes);
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
                *self = Tree::Node(nodes);
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
                Tree::Node(children) => {
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
                Tree::Node(children) => {
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
            Tree::Node(children) => {
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

    pub fn fill_color(&mut self, color:Rgba<u8>) {
        match self {
            Tree::Leaf(rect, leaf_color) => {
                match leaf_color {
                    Option::None => *leaf_color = Option::Some(color),
                    _ => {}
                }
            }
            Tree::Node(children) => {
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
    x: i32,
    y: i32,
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
pub fn color_to_string(color:&Rgba<u8>) -> String {
    format!("{},{},{},{}", color[0], color[1], color[2], color[3])
}
pub fn color_from_str(str:&str) -> Rgba<u8> {
    let args:Vec<u8> = str.split(",").map(|x| x.trim().parse().unwrap()).collect();
    Rgba::from([args[0], args[1], args[2], args[3]])
}

pub enum Command {
    PointCut(Id, Point),
    LineCut(Id, bool, i32),
    Merge(Id, Id),
    Color(Id, Rgba<u8>),
}
