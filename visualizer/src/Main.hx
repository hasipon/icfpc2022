package;
import js.Browser;
import js.html.CanvasElement;
import js.html.Element;
import js.html.ImageElement;
import js.html.InputElement;
import js.html.TextAreaElement;
import pixi.core.Application;
import pixi.core.graphics.Graphics;
import pixi.core.math.Point;
import pixi.core.sprites.Sprite;
import pixi.core.textures.Texture;
import pixi.interaction.InteractionEvent;

class Main 
{
	static var input       :TextAreaElement;
	static var problemInput:InputElement;
	static var imageElement:ImageElement;
	
	static var mainCanvas:CanvasElement;
	static var outputCanvas:CanvasElement;
	static var mainPixi:Application;
	static var borderLayer :Graphics;
	static var problemLayer:Sprite;
	static var state       :State;
	static var errorOutput :ErrorOutput;
	
	static function main()
	{
		outputCanvas = cast Browser.document.getElementById("output");
		mainCanvas = cast Browser.document.getElementById("pixi");
	
		mainPixi = new Application({
			view  :mainCanvas,
			transparent: true,
			width: mainCanvas.width,
			height: mainCanvas.height,
			autoResize: true,
		});
		mainPixi.stage.interactive = true;
		mainPixi.stage.addChild(problemLayer = new Sprite());
		mainPixi.stage.addChild(borderLayer  = new Graphics());
		mainPixi.stage.scale.x = 2.0;
		mainPixi.stage.scale.y = 2.0;
		mainPixi.stage.on("mousemove", onMouseMove);
		problemLayer.x = problemLayer.y = 20;
		borderLayer .x = borderLayer .y = 20;
		problemLayer.alpha = 0.3;
		
		state = new State(
			outputCanvas.getContext2d(),
			borderLayer,
			errorOutput = new ErrorOutput()
		);
		
		problemInput = cast Browser.document.getElementById("problem");
		problemInput.onchange = onProblemChanged;
		problemInput.oninput = onProblemChanged;
		
		imageElement = cast Browser.document.getElementById("image");
		imageElement.onload = onImageLoad;
		
		input = cast Browser.document.getElementById("input");
		input.onchange = onInputChanged;
		input.oninput = onInputChanged;
		Browser.window.addEventListener('hashchange', readHash, false);
		readHash();
	}
	
	static function readHash():Void
	{
		var hash = Browser.location.hash;
		var index = hash.indexOf(";");
		if (index > 0)
		{
			var inputText = hash.substr(index + 1);
			input.value = StringTools.urlDecode(inputText);
		}
		else
		{
			index = hash.length;
		}
		var problem = Std.parseInt(hash.substring(1, index));
		if (problem != null && problem > 0)
		{
			problemInput.value = "" + problem;
		}
		
		onProblemChanged();
		setHash();
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
		try
		{
			errorOutput.text = "";
			state.update(
				input.value,
				imageElement.width,
				imageElement.height
			);
		}
		catch (d:Dynamic)
		{
		}
		
		var error = cast Browser.document.getElementById("error");
		error.innerText = errorOutput.text;
		setHash();
	}
	
	private static function setHash():Void 
	{
		Browser.location.hash = "#" + Std.parseInt(problemInput.value) + ";" + StringTools.urlEncode(input.value);
	}
	
	public static function onMouseMove(e:InteractionEvent):Void
	{
		var point = problemLayer.toLocal(new Point(e.data.global.x, e.data.global.y));
		
		var text = cast Browser.document.getElementById("point");
		text.innerText = Math.round(point.x) + "," + Math.round(state.cutState.height - point.y);
	}
}
