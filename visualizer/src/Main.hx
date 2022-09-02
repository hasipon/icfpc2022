package;
import js.Browser;
import js.Lib;
import js.html.CanvasElement;
import js.html.Element;
import js.html.Image;
import js.html.ImageElement;
import js.html.InputElement;

class Main 
{
	public static var problemInput:InputElement;
	public static var imageElement:ImageElement;
	public static var canvas:CanvasElement;
	public static var image:Image;
	
	static function main()
	{
		canvas = cast Browser.document.getElementById("main");
		
		problemInput = cast Browser.document.getElementById("problem");
		problemInput.onchange = onProblemChanged;
		problemInput.oninput = onProblemChanged;
		
		imageElement = cast Browser.document.getElementById("image");
		imageElement.onload = onImageLoad;
		
		onProblemChanged();
	}
	
	static function onProblemChanged():Void
	{
		imageElement.src = "../problems/" + problemInput.value + ".png";
	}
	
	static function onImageLoad():Void 
	{
	}
}
