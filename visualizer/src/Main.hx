package;
import js.Browser;
import js.Lib;
import js.html.CanvasElement;
import js.html.Element;
import js.html.InputElement;

class Main 
{
	public static var problemInput:InputElement;
	public static var canvas:CanvasElement;
	
	static function main()
	{
		canvas = cast Browser.document.getElementById("main");
		
		problemInput = cast Browser.document.getElementById("problem");
		problemInput.addEventListener("changed", onProblemChanged);
	}
	
	static function onProblemChanged():Void
	{
		
	}
}
