package ;
import pixi.core.math.shapes.Rectangle;

enum CutNode
{
	Leaf(rect:Rectangle);
	Node(children:Array<CutNode>);
	Marged;
}
