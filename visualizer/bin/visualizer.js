(function ($global) { "use strict";
var Main = function() { };
Main.main = function() {
	Main.mainCanvas = window.document.getElementById("output");
	Main.mainPixi = new PIXI.Application({ view : Main.mainCanvas, transparent : true, width : Main.mainCanvas.width, height : Main.mainCanvas.height, autoResize : true});
	Main.mainPixi.stage.interactive = true;
	Main.mainPixi.stage.addChild(Main.outputLayer = new PIXI.Graphics());
	Main.mainPixi.stage.addChild(Main.problemLayer = new PIXI.Sprite());
	Main.mainPixi.stage.addChild(Main.borderLayer = new PIXI.Graphics());
	Main.outputLayer.x = Main.outputLayer.y = 20;
	Main.problemLayer.x = Main.problemLayer.y = 20;
	Main.borderLayer.x = Main.borderLayer.y = 20;
	Main.problemLayer.alpha = 0.3;
	Main.problemInput = window.document.getElementById("problem");
	Main.problemInput.onchange = Main.onProblemChanged;
	Main.problemInput.oninput = Main.onProblemChanged;
	Main.imageElement = window.document.getElementById("image");
	Main.imageElement.onload = Main.onImageLoad;
	Main.onProblemChanged();
};
Main.onProblemChanged = function() {
	Main.imageElement.src = "../problems/" + Main.problemInput.value + ".png";
	if(Main.problemLayer.texture != null) {
		Main.problemLayer.texture.destroy();
	}
	Main.problemLayer.texture = PIXI.Texture.fromImage("../problems/" + Main.problemInput.value + ".png");
};
Main.onImageLoad = function() {
};
Main.write = function() {
};
var haxe_iterators_ArrayIterator = function(array) {
	this.current = 0;
	this.array = array;
};
haxe_iterators_ArrayIterator.prototype = {
	hasNext: function() {
		return this.current < this.array.length;
	}
	,next: function() {
		return this.array[this.current++];
	}
};
Main.main();
})({});
