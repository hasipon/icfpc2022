package ;
import js.html.svg.Rect;
import pixi.core.graphics.Graphics;
import pixi.core.math.Point;
import pixi.core.math.shapes.Rectangle;
import pixi.core.sprites.Sprite;
import pixi.core.text.Text;

class CutState 
{
	public var textLayer:Sprite;
	public var borderLayer:Graphics;
	public var roots:Array<CutNode>;
	public var errorOutput:ErrorOutput;
	public var lineNumber:Int;
	public var height:Int;
	public var width:Int;
	public var textFields:Array<Text>;
	public var textPool:Array<Text>;
	
	public function new(
		borderLayer:Graphics, 
		textLayer:Sprite, 
		errorOutput:ErrorOutput
	) 
	{
		this.textLayer = textLayer;
		this.errorOutput = errorOutput;
		this.borderLayer = borderLayer;
		textFields = [];
		textPool = [];
	}
	
	public function init(initialState:InitialState):Void
	{
		this.width  = initialState.width;
		this.height = initialState.height;
		this.roots = [for (node in initialState.nodes) CutNode.Leaf(node.rect)];
	}
	
	public function draw():Void 
	{
		borderLayer.clear();
		textLayer.removeChildren();
		while (textFields.length > 0)
		{
			textPool.push(textFields.pop());
		}
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
					
					var text = if (textPool.length > 0)
					{
						textPool.pop();
					}
					else
					{
						new Text("", { fill: 0xFF0000 });
					}
					text.text = ids.join(".");
					text.x = rect.x + 2;
					text.y = height - rect.bottom;
					text.alpha = 0.6;
					text.scale.x = 0.25;
					text.scale.y = 0.25;
					textLayer.addChild(text);
					textFields.push(text);
					
				case CutNode.Node(children):
					_draw(ids, children);
					
				case CutNode.Marged:
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
				
			case CutNode.Node(_):
				errorOutput.add(lineNumber, "unknown id. found node:" + ids.join(".") + "." + index);
				
			case CutNode.Marged:
				errorOutput.add(lineNumber, "unknown id. marged:" + ids.join(".") + "." + index);
		}
	}
	
	public function cutPoint(lineNumber:Int, ids:Array<Int>, pos:Point):Void 
	{
		this.lineNumber = lineNumber;
		var index = ids.pop();
		var parent = getById(ids);
		
		switch (parent[index])
		{
			case CutNode.Leaf(rect):
				var nodes = [];
				parent[index] = CutNode.Node(nodes);
				
					nodes.push(CutNode.Leaf(new Rectangle(
						rect.x,
						rect.y,
						pos.x - rect.x,
						pos.y - rect.y
					)));
					nodes.push(CutNode.Leaf(new Rectangle(
						pos.x,
						rect.y,
						rect.right - pos.x,
						pos.y - rect.y
					)));
					nodes.push(CutNode.Leaf(new Rectangle(
						pos.x,
						pos.y,
						rect.right - pos.x,
						rect.bottom - pos.y
					)));
					nodes.push(CutNode.Leaf(new Rectangle(
						rect.x,
						pos.y,
						pos.x - rect.x,
						rect.bottom - pos.y
					)));
				
			case CutNode.Node(_):
				errorOutput.add(lineNumber, "unknown id. found node:" + ids.join(".") + "." + index);
				
			case CutNode.Marged:
				errorOutput.add(lineNumber, "unknown id. marged:" + ids.join(".") + "." + index);
		}
	}
	
	public function merge(lineNumber:Int, id0:Array<Int>, id1:Array<Int>):Void 
	{
		var index0 = id0.pop();
		var parent0 = getById(id0);
		var index1 = id1.pop();
		var parent1 = getById(id1);
		var node0 = parent0[index0];
		var node1 = parent1[index1];
		if (node0 == null)
		{
			errorOutput.add(lineNumber, "unknown id:" + id0.join(".") + "." + index0);
		}
		if (node1 == null)
		{
			errorOutput.add(lineNumber, "unknown id:" + id1.join(".") + "." + index1);
		}		
		switch [node0, node1]
		{
			case [CutNode.Leaf(rect0), CutNode.Leaf(rect1)]:
				parent0[index0] = CutNode.Marged;
				parent1[index1] = CutNode.Marged;
				var result = new Rectangle();
				result.x = Math.min(rect0.x, rect1.x);
				result.y = Math.min(rect0.y, rect1.y);
				result.width  = Math.max(rect0.right , rect1.right ) - result.x;
				result.height = Math.max(rect0.bottom, rect1.bottom) - result.y;
				
				if (
					(rect0.width != rect1.width && rect0.height != rect1.height) ||
					rect0.width * rect0.height + rect1.width * rect1.height != result.width * result.height
				)
				{
					errorOutput.add(lineNumber, "cannot marge:" + id0.join(".") + "." + index0 + ", " + id1.join(".") + "." + index1);
					throw "error";
				}
				roots.push(CutNode.Leaf(result));
				
			case _:
				errorOutput.add(lineNumber, "unmatched node:" + id0.join(".") + "." + index0 + ", " + id1.join(".") + "." + index1);
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
					
				case CutNode.Marged:
					errorOutput.add(lineNumber, "unknown id. marged:" + ids.join("."));
			}
			
		}
		return result;
	}
	
	public function getRectangle(lineNumber:Int, ids:Array<Int>):Rectangle
	{
		this.lineNumber = lineNumber;
		var index = ids.pop();
		var parent = getById(ids);
		
		return switch (parent[index])
		{
			case CutNode.Leaf(rect):
				rect;
				
			case CutNode.Node(_):
				errorOutput.add(lineNumber, "unknown id. found node:" + ids.join(".") + "." + index);
				throw "error";
				
			case CutNode.Marged:
				errorOutput.add(lineNumber, "unknown id. marged:" + ids.join(".") + "." + index);
				throw "error";
				
		}
	}
	
	public function getNodeAt(x:Int, y:Int):Array<Int>
	{
		var ids = [];
		_getNodeAt(x, y, ids, roots);
		return ids;
	}
	
	private function _getNodeAt(x:Int, y:Int, ids:Array<Int>, nodes:Array<CutNode>):Bool
	{
		for (i in 0...nodes.length)
		{
			ids.push(i);
			switch (nodes[i])
			{
				case CutNode.Leaf(rect):
					if (
						rect.x < x && x < rect.right &&
						rect.y < y && y < rect.bottom
					)
					{
						return true;
					}
					
				case CutNode.Node(children):
					if (_getNodeAt(x, y, ids, children))
					{
						return true;
					}
					
				case CutNode.Marged:
					
			}
			ids.pop();
		}
		return false;
	}
}
