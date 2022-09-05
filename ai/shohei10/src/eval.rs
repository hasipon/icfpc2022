
use image::RgbaImage;

pub fn similarity(a:&RgbaImage, b:&RgbaImage) -> i64 {
    let mut result = 0.0;
    for x in 0..a.width() {
        for y in 0..a.height() {
            let p0 = a.get_pixel(x as u32, y as u32);
            let p1 = b.get_pixel(x as u32, y as u32);
            let r = p0[0] as f64 - p1[0] as f64;
            let g = p0[1] as f64 - p1[1] as f64;
            let b = p0[2] as f64 - p1[2] as f64;
            let a = p0[3] as f64 - p1[3] as f64;
            result += (a * a + b * b + r * r + g * g).sqrt();
        }
    }
    (result * 0.005).round() as i64
}
