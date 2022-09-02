package ;
import pixi.core.graphics.Graphics;
import pixi.core.math.shapes.Rectangle;

class CutState 
{
	public var borderLayer:Graphics;
	public var roots:Array<CutNode>;
	public var errorOutput:ErrorOutput;
	public var lineNumber:Int;
	public var height:Int;
	public var width:Int;
	
	public function new(
		borderLayer:Graphics, 
		errorOutput:ErrorOutput
	) 
	{
		this.errorOutput = errorOutput;
		this.borderLayer = borderLayer;
	}
	
	public function init(width:Int, height:Int):Void
	{
		this.width = width;
		this.height = height;
		this.roots = [CutNode.Leaf(new Rectangle(0, 0, width, height))];
	}
	
	public function draw():Void 
	{
		borderLayer.clear();
		_draw([], roots);
	}
	
	public function _draw(
		ids:Array<Int>, 
		nodes:Array<CutNode>
	):Void
	{
		for (i in 0...nodes.length)
		{
			ids.push(i);
			switch (nodes[i])
			{
				case CutNode.Leaf(rect):
					borderLayer.lineStyle(0.5, 0xFF0000, 0.2);
					borderLayer.drawRect(
						rect.x, 
						height - rect.bottom, 
						rect.width, 
						rect.height
					);
					
				case CutNode.Node(children):
					_draw(ids, children);
			}
			ids.pop();
		}
	}
	
	public function cut(lineNumber:Int, ids:Array<Int>, isX:Bool, pos:Float):Void 
	{
		this.lineNumber = lineNumber;
		var index = ids.pop();
		var parent = getById(ids);
		
		switch (parent[index])
		{
			case CutNode.Leaf(rect):
				var nodes = [];
				parent[index] = CutNode.Node(nodes);
				
				if (isX)
				{
					nodes.push(CutNode.Leaf(new Rectangle(
						rect.x,
						rect.y,
						pos - rect.x,
						rect.height
					)));
					nodes.push(CutNode.Leaf(new Rectangle(
						pos,
						rect.y,
						rect.right - pos,
						rect.height
					)));
				}
				else
				{
					nodes.push(CutNode.Leaf(new Rectangle(
						rect.x,
						rect.y,
						rect.width,
						pos - rect.y
					)));
					nodes.push(CutNode.Leaf(new Rectangle(
						rect.x,
						pos,
						rect.width,
						rect.bottom - pos
					)));
				}
				
			case CutNode.Node(children):
				errorOutput.add(lineNumber, "unknown id. found node:" + ids.join(".") + "." + index);
		}
	}
	
	public function getById(ids:Array<Int>):Array<CutNode>
	{
		var result = roots;
		for (id in ids)
		{
			var node = result[id];
			if (node == null)
			{
				errorOutput.add(lineNumber, "unknown id:" + ids.join("."));
			}
			switch (node)
			{
				case CutNode.Leaf(_):
					errorOutput.add(lineNumber, "unknown id. found leaf:" + ids.join("."));
					
				case CutNode.Node(children):
					result = children;
			}
			
		}
		return result;
	}
}

enum CutNode
{
	Leaf(rect:Rectangle);
	Node(children:Array<CutNode>);
}
