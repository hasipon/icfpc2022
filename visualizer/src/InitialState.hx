package ;

class InitialState 
{
	public var nodes:Array<InitialNode>;
	public var height:Int;
	public var width :Int;
	
	public function new(
		width:Int,
		height:Int,
		nodes:Array<InitialNode>
	)
	{
		this.width  = width ;
		this.height = height;
		this.nodes  = nodes ;
	}
}
