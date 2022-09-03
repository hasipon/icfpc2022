package ;
import js.html.CanvasRenderingContext2D;
import pixi.core.graphics.Graphics;
import pixi.core.math.Point;
import pixi.core.sprites.Sprite;
import pixi.plugins.spine.core.Color;
import tweenxcore.color.ArgbColor;
import tweenxcore.color.RgbColor;
using tweenxcore.Tools.FloatTools;

class State 
{
	public var errorOutput:ErrorOutput;
	public var cutState:CutState;
	public var outputLayer:CanvasRenderingContext2D;
	public static var NL:EReg = new EReg("\r\n|\r|\n", "g");
	public static var S:EReg = new EReg("\\s", "g");
	
	public function new(
		outputLayer:CanvasRenderingContext2D, 
		borderLayer:Graphics, 
		textLayer:Sprite,
		errorOutput:ErrorOutput
	) 
	{
		this.errorOutput = errorOutput;
		this.outputLayer = outputLayer;
		this.cutState = new CutState(borderLayer, textLayer, errorOutput);
	}
	
	public function update(value:String, initialState:InitialState):Void 
	{
		outputLayer.fillStyle = "white";
		outputLayer.fillRect(0, 0, initialState.width, initialState.height);
		for (node in initialState.nodes)
		{
			outputLayer.fillStyle = node.color.toRgbaCssString();
			outputLayer.fillRect(
				node.rect.x, 
				node.rect.y, 
				node.rect.width, 
				node.rect.height
			);
		}
		cutState.init(initialState);
		var lines = NL.split(value);
		var index = 0;
		for (line in lines)
		{
			index += 1;
			line = StringTools.trim(line);
			if (line == "") continue;
			var args = parseLine(index, line);
			
			switch (args[0])
			{
				case "cut":
					if (args.length == 3)
					{
						cutState.cutPoint(
							index, 
							parseId(args[1]), 
							parsePoint   (args[2])
						);
					}
					else if (args.length == 4)
					{
						cutState.cut(
							index, 
							parseId(args[1]), 
							args[2] == "x" || args[2] == "X", 
							Std.parseInt(args[3])
						);
					}
					else
					{
						errorOutput.add(index, "too many arguments : in " + line);
					}
				
				case "color":
					if (args.length == 3)
					{
						var rect = cutState.getRectangle(
							index, 
							parseId(args[1])
						);
						var color = parseRgba(args[2]);
						color.r = color.a.lerp(1.0, color.r);
						color.g = color.a.lerp(1.0, color.g);
						color.b = color.a.lerp(1.0, color.b);
						outputLayer.fillStyle = color.toRgbCssString();
						outputLayer.fillRect(rect.x, cutState.height - rect.bottom, rect.width, rect.height);
					}
					else
					{
						errorOutput.add(index, "too many arguments : in " + line);
					}
				case "swap":
					if (args.length == 3)
					{
						var rect0 = cutState.getRectangle(
							index, 
							parseId(args[1])
						);
						var rect1 = cutState.getRectangle(
							index, 
							parseId(args[2])
						);
						if (rect0.width != rect1.width || rect0.height != rect1.height)
						{
							errorOutput.add(index, "unmatched size : in " + line);
							return;
						}
						
						var image0 = outputLayer.getImageData(rect0.x, cutState.height - rect0.bottom, rect0.width, rect0.height);
						var image1 = outputLayer.getImageData(rect1.x, cutState.height - rect1.bottom, rect1.width, rect1.height);
						outputLayer.putImageData(image1, rect0.x, cutState.height - rect0.bottom);
						outputLayer.putImageData(image0, rect1.x, cutState.height - rect1.bottom);
					}
					else
					{
						errorOutput.add(index, "too many arguments : in " + line);
					}
				case "merge":
					if (args.length == 3)
					{
						cutState.merge(
							index, 
							parseId(args[1]),
							parseId(args[2])
						);
					}
					else
					{
						errorOutput.add(index, "too many arguments : in " + line);
					}
					
				case x:
					errorOutput.add(index, "unknown move: " + x + " : in " + line);
			}
		}
		cutState.draw();
	}
	
	public function getLineCut(isX:Bool, x:Int, y:Int):String
	{
		var id = cutState.getNodeAt(x, y);
		if (id.length == 0) {
			errorOutput.add(0, "not found parent rect");
			return ""; 
		}
		return "\ncut [" + id.join(".") + "] [" + (if (isX) "x" else "y") + "] [" + (if (isX) x else y) + "]";
	}
	public function getPointCut(x:Int, y:Int):String
	{
		var id = cutState.getNodeAt(x, y);
		if (id.length == 0) {
			errorOutput.add(0, "not found parent rect");
			return ""; 
		}
		return "\ncut [" + id.join(".") + "] [" + x + "," + y + "]";
	}
	
	
	private function parsePoint(string:String):Point
	{
		var args = string.split(",");
		return new Point(Std.parseInt(args[0]), Std.parseInt(args[1]));
	}
	
	private function parseRgba(string:String):ArgbColor
	{
		var args = string.split(",");
		return new ArgbColor(
			Std.parseInt(args[3]) / 255, 
			Std.parseInt(args[0]) / 255, 
			Std.parseInt(args[1]) / 255,
			Std.parseInt(args[2]) / 255
		);
	}
	
	private function parseLine(index:Int, line:String):Array<String>
	{
		var result = [];
		var args = S.split(line);
		result.push(args[0]);
		
		var command = null;
		for (i in 1...args.length)
		{
			var token = args[i];
			if (command == null)
			{
				if (StringTools.startsWith(token, "["))
				{
					token = token.substr(1);
					command = "";
				}
				else
				{
					errorOutput.add(index, "unknown matched [ : in " + line);
					continue;
				}
			}
			else
			{
				command += " ";
			}
			
			if (StringTools.endsWith(token, "]"))
			{
				command += token.substr(0, token.length - 1);
				result.push(command);
				command = null;
			}
			else
			{
				command += token;
			}
		}
		return result;
	}
	
	private function parseId(line:String):Array<Int>
	{
		var args = line.split(".");
		return [for (arg in args) Std.parseInt(arg)];
	}
}
