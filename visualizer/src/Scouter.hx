package ;
import js.Browser;
import js.html.CanvasElement;
import js.html.CanvasRenderingContext2D;
import js.html.ImageElement;
import pixi.core.graphics.Graphics;

class Scouter 
{
	private var state:State;
	private var scouterLayer:Graphics;
	private var context:CanvasRenderingContext2D;
	private var canvas:CanvasElement;
	public var pixel:Int;
	public var left  :Int;
	public var right :Int;
	public var top   :Int;
	public var bottom:Int;
	public var x:Int;
	public var y:Int;
	
	public function new(
		state:State, 
		scouterLayer:Graphics
	) 
	{
		this.canvas = cast Browser.document.createElement("canvas");
		Browser.document.body.appendChild(canvas);
		this.canvas.width  = 400;
		this.canvas.height = 400;
		this.canvas.style.display = 'none';
		this.context = canvas.getContext('2d');
		
		this.state = state;
		this.scouterLayer = scouterLayer;
	}
	
	public function update(
		x:Int, 
		y:Int,
		image:ImageElement
	):Void 
	{
		this.x = x;
		this.y = y;
		context.fillStyle = "white";
		context.drawImage(image, 0, 0, 400, 400);
		pixel = getPixel(x, y);
		left   = x;
		right  = x;
		top    = y;
		bottom = y;
		
		while (0 < left)
		{
			left -= 1;
			var pixel2 = getPixel(left, y);
			if (pixel2 != pixel) { break; }
		}
		while (0 < top)
		{
			top -= 1;
			var pixel2 = getPixel(x, top);
			if (pixel2 != pixel) { break; }
		}
		while (400 > right)
		{
			right += 1;
			var pixel2 = getPixel(right, y);
			if (pixel2 != pixel) { break; }
		}
		while (400 > bottom)
		{
			bottom += 1;
			var pixel2 = getPixel(x, bottom);
			if (pixel2 != pixel) { break; }
		}
		
		scouterLayer.clear();
		scouterLayer.lineStyle(0.5, 0x00FF00, 0.8);
		scouterLayer.moveTo(-20, top);
		scouterLayer.lineTo(420, top);
		scouterLayer.moveTo(-20, bottom);
		scouterLayer.lineTo(420, bottom);
		scouterLayer.moveTo(left , -20);
		scouterLayer.lineTo(left , 420);
		scouterLayer.moveTo(right, -20);
		scouterLayer.lineTo(right, 420);
	}
	
	public function getPixel(x:Int, y:Int):Int
	{
		var data = context.getImageData(x, y, 1, 1).data;
		return 
			(data[3] << 24) |
			(data[0] << 16) |
			(data[1] <<  8) |
			(data[2] <<  0);
	}
}
