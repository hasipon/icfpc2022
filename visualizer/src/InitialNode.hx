package ;
import pixi.core.math.shapes.Rectangle;
import tweenxcore.color.ArgbColor;

class InitialNode 
{
	public var color:ArgbColor;
	public var rect:Rectangle;

	public function new(rect:Rectangle, color:ArgbColor) 
	{
		this.rect = rect;
		this.color = color;
	}
}