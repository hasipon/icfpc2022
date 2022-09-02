package ;
import pixi.core.graphics.Graphics;

class State 
{
	public var borderLayer:Graphics;
	public var cutState:CutState;
	public var outputLayer:Graphics;

	public function new(outputLayer:Graphics, borderLayer:Graphics) 
	{
		this.borderLayer = borderLayer;
		this.outputLayer = outputLayer;
		this.cutState = new CutState();
	}
	
	public function update(value:String, width:Int, height:Int):Void 
	{
		trace(value, width, height);
	}
}
