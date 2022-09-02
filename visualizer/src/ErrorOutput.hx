package ;

class ErrorOutput 
{
	public var text:String;
	
	public function new() 
	{
		text = "";
	}
	
	public function add(lineNumber:Int, message:String):Void 
	{
		text += lineNumber + ": " + message + "\n";
	}
}
