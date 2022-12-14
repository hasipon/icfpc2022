package;
import haxe.Http;
import haxe.Json;
import js.Browser;
import js.html.CanvasElement;
import js.html.Element;
import js.html.ImageElement;
import js.html.InputElement;
import js.html.KeyboardEvent;
import js.html.TextAreaElement;
import pixi.core.Application;
import pixi.core.graphics.Graphics;
import pixi.core.math.Point;
import pixi.core.math.shapes.Rectangle;
import pixi.core.sprites.Sprite;
import pixi.core.textures.Texture;
import pixi.interaction.InteractionEvent;
import tweenxcore.color.ArgbColor;

class Main 
{
	static var input       :TextAreaElement;
	static var problemInput:InputElement;
	static var imageElement:ImageElement;
	static var initialState:InitialState;
	static var mainCanvas  :CanvasElement;
	static var outputCanvas:CanvasElement;
	static var mainPixi    :Application;
	static var borderLayer :Graphics;
	static var problemLayer:Sprite;
	static var scouterLayer:Graphics;
	static var state       :State;
	static var errorOutput :ErrorOutput;
	static var scouter     :Scouter;
	static var history:Array<String> = [];
	static var textLayer:Sprite;
	
	
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
		mainPixi.stage.addChild(problemLayer  = new Sprite());
		mainPixi.stage.addChild(borderLayer   = new Graphics());
		mainPixi.stage.addChild(textLayer     = new Sprite());
		mainPixi.stage.addChild(scouterLayer  = new Graphics());
		mainPixi.stage.scale.x = 2.0;
		mainPixi.stage.scale.y = 2.0;
		mainPixi.stage.on("mousedown", onMouseDown);
		problemLayer.x = problemLayer.y = 5;
		borderLayer .x = borderLayer .y = 5;
		scouterLayer.x = scouterLayer.y = 5;
		textLayer   .x = textLayer   .y = 5;
		problemLayer.alpha = 0.3;
		
		state = new State(
			outputCanvas.getContext2d(),
			borderLayer,
			textLayer,
			errorOutput = new ErrorOutput()
		);
		scouter = new Scouter(
			state,
			scouterLayer
		);
		
		problemInput = cast Browser.document.getElementById("problem");
		problemInput.onchange = onProblemChanged;
		problemInput.oninput = onProblemChanged;
		
		imageElement = cast Browser.document.getElementById("image");
		imageElement.onload = onImageLoad;
		
		input = cast Browser.document.getElementById("input");
		input.onchange = onInputChanged;
		input.oninput = onInputChanged;
		
		Browser.window.addEventListener("keydown", onKey, false);
		Browser.window.addEventListener('hashchange', readHash, false);
		readHash();
	}
	
	static function onKey(e:KeyboardEvent):Void 
	{
		if (initialState == null) return;
		if (
			Browser.document.activeElement == problemInput ||
			Browser.document.activeElement == input
		)
		{
			return;
		}
		if (e.ctrlKey)
		{
			switch (e.keyCode)
			{
				case KeyboardEvent.DOM_VK_Z: 
					var result = State.NL.split(input.value);
					history.push(result.pop());
					input.value = result.join("\n");
					
				case KeyboardEvent.DOM_VK_Y:
					if (history.length > 0)
					{
						var result = State.NL.split(input.value);
						result.push(history.pop());
						input.value = result.join("\n");
					}
				case _: return;
			}
			onInputChanged();
			
			e.preventDefault();
			var error = cast Browser.document.getElementById("error");
			error.innerText = errorOutput.text;
		}
		else
		{
			switch (e.keyCode)
			{
				case KeyboardEvent.DOM_VK_W: input.value += state.getLineCut(false, scouter.x    , initialState.height - scouter.top   );
				case KeyboardEvent.DOM_VK_X: input.value += state.getLineCut(false, scouter.x    , initialState.height - scouter.bottom);
				case KeyboardEvent.DOM_VK_D: input.value += state.getLineCut(true , scouter.right, initialState.height - scouter.y);
				case KeyboardEvent.DOM_VK_A: input.value += state.getLineCut(true , scouter.left , initialState.height - scouter.y);
				case KeyboardEvent.DOM_VK_Q: input.value += state.getPointCut(scouter.left , initialState.height - scouter.top   );
				case KeyboardEvent.DOM_VK_E: input.value += state.getPointCut(scouter.right, initialState.height - scouter.top   );
				case KeyboardEvent.DOM_VK_Z: input.value += state.getPointCut(scouter.left , initialState.height - scouter.bottom);
				case KeyboardEvent.DOM_VK_C: input.value += state.getPointCut(scouter.right, initialState.height - scouter.bottom);
				case KeyboardEvent.DOM_VK_S: input.value += state.getPointCut(scouter.x, initialState.height - scouter.y);
				case _: return;
			}
			
			_onInputChanged();
			
			e.preventDefault();
			var error = cast Browser.document.getElementById("error");
			error.innerText = errorOutput.text;
		}
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
		var num = Std.parseInt(problemInput.value);
		if (num > 25)
		{
			var url = "../problems/" + num + ".initial.json";
			var http = new Http(url);
			http.onData = onJsonLoaded;
			http.request();
		}
		else
		{
			initialState = new InitialState(
				400, 
				400,
				[
					new InitialNode(new Rectangle(0, 0, 400, 400), ArgbColor.of(0x000000)),
				]
			);
			onInputChanged();
		}
	}
	static function onJsonLoaded(data:String):Void
	{
		var data = Json.parse(data);
		var nodes = [];
		for (block in (data.blocks:Array<Dynamic>))
		{
			trace(
					block.color[3],
					block.color[0],
					block.color[1],
					block.color[2]
			);
			nodes[Std.parseInt(block.blockId)] = new InitialNode(
				new Rectangle(
					block.bottomLeft[0],
					block.bottomLeft[1],
					block.topRight[0] - block.bottomLeft[0],
					block.topRight[1] - block.bottomLeft[1]
				),
				new ArgbColor(
					block.color[3] / 255,
					block.color[0] / 255,
					block.color[1] / 255,
					block.color[2] / 255
				)
			);
		}
		initialState = new InitialState(
			data.width,
			data.height,
			nodes
		);
		onInputChanged();
	}
	static function _onInputChanged():Void
	{
		history = [];
		onInputChanged();
	}
	static function onInputChanged():Void
	{
		if (initialState == null) return;
		try
		{
			errorOutput.text = "";
			state.update(
				input.value,
				initialState
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
	
	public static function onMouseDown(e:InteractionEvent):Void
	{
		var point = problemLayer.toLocal(new Point(e.data.global.x, e.data.global.y));
		
		scouter.update(Math.round(point.x), Math.round(point.y), imageElement);
		
		var color = ArgbColor.of(scouter.pixel);
		var text = cast Browser.document.getElementById("point");
		text.innerText = 
			Math.round(point.x) + "," + 
			Math.round(state.cutState.height - point.y) + "," + 
			"[" + color.r * 255 +
			"," + color.g * 255 +
			"," + color.b * 255 +
			"," + color.a * 255 +
			"]";
	}
}
