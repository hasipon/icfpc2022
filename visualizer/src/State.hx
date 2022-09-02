package ;
import pixi.core.graphics.Graphics;

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
					cutState.cut(
						index, 
						parsePosition(args[1]), 
						args[2] == "x", 
						Std.parseInt(args[3])
					);
					
				case x:
					errorOutput.add(index, "unknown move: " + x + " : in " + line);
			}
		}
		cutState.draw();
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
	
	private function parsePosition(line:String):Array<Int>
	{
		var args = line.split(".");
		return [for (arg in args) Std.parseInt(arg)];
	}
}
