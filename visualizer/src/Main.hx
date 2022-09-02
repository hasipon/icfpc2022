package;
import js.Browser;
import js.html.CanvasElement;
import js.html.Element;
import js.html.ImageElement;
import js.html.InputElement;
import js.html.TextAreaElement;
import pixi.core.Application;
import pixi.core.graphics.Graphics;
import pixi.core.sprites.Sprite;
import pixi.core.textures.Texture;

class Main 
{
	static var input       :TextAreaElement;
	static var problemInput:InputElement;
	static var imageElement:ImageElement;
	
	static var mainCanvas:CanvasElement;
	static var mainPixi:Application;
	static var borderLayer :Graphics;
	static var outputLayer :Graphics;
	static var problemLayer:Sprite;
	static var state       :State;
	
	static function main()
	{
		mainCanvas = cast Browser.document.getElementById("output");
	
		mainPixi = new Application({
			view  :mainCanvas,
			transparent: true,
			width: mainCanvas.width,
			height: mainCanvas.height,
			autoResize: true,
		});
		mainPixi.stage.interactive = true;
		mainPixi.stage.addChild(outputLayer  = new Graphics());
		mainPixi.stage.addChild(problemLayer = new Sprite());
		mainPixi.stage.addChild(borderLayer  = new Graphics());
		outputLayer .x = outputLayer .y = 20;
		problemLayer.x = problemLayer.y = 20;
		borderLayer .x = borderLayer .y = 20;
		problemLayer.alpha = 0.3;
		
		state = new State(
			outputLayer,
			borderLayer
		);
		
		problemInput = cast Browser.document.getElementById("problem");
		problemInput.onchange = onProblemChanged;
		problemInput.oninput = onProblemChanged;
		
		imageElement = cast Browser.document.getElementById("image");
		imageElement.onload = onImageLoad;
		
		input = cast Browser.document.getElementById("input");
		input.onchange = onInputChanged;
		input.oninput = onInputChanged;
		
		onProblemChanged();
	}
	
	static function onProblemChanged():Void
	{
		imageElement.src = "../problems/" + problemInput.value + ".png";
		
		if (problemLayer.texture != null)
		{
			problemLayer.texture.destroy();
		}
		problemLayer.texture = Texture.fromImage("../problems/" + problemInput.value + ".png");
	}
	
	static function onImageLoad():Void 
	{
		onInputChanged();
	}
	
	static function onInputChanged():Void
	{
		state.update(
			input.value,
			imageElement.width,
			imageElement.height
		);
	}
}
