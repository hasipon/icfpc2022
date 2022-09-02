package ;
import pixi.core.graphics.Graphics;
import pixi.core.math.Point;
import pixi.plugins.spine.core.Color;
import tweenxcore.color.ArgbColor;
import tweenxcore.color.RgbColor;

class State 
{
	public var errorOutput:ErrorOutput;
	public var cutState:CutState;
	public var outputLayer:Graphics;
	public static var NL:EReg = new EReg("\r\n|\r|\n", "g");
	public static var S:EReg = new EReg("\\s", "g");
	
	public function new(
		outputLayer:Graphics, 
		borderLayer:Graphics, 
		errorOutput:ErrorOutput
	) 
	{
		this.errorOutput = errorOutput;
		this.outputLayer = outputLayer;
		this.cutState = new CutState(borderLayer, errorOutput);
	}
	
	public function update(value:String, width:Int, height:Int):Void 
	{
		outputLayer.clear();
		cutState.init(width, height);
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
							args[2] == "x", 
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
						outputLayer.beginFill(
							color.toRgbInt(),
							color.a
						);
						outputLayer.drawRect(
							rect.x, 
							cutState.height - rect.bottom, 
							rect.width, 
							rect.height
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
