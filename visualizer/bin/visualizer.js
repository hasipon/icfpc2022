(function ($global) { "use strict";
var Main = function() { };
Main.main = function() {
	Main.canvas = window.document.getElementById("main");
	Main.problemInput = window.document.getElementById("problem");
	Main.problemInput.onchange = Main.onProblemChanged;
	Main.problemInput.oninput = Main.onProblemChanged;
	Main.imageElement = window.document.getElementById("image");
	Main.imageElement.onload = Main.onImageLoad;
	Main.onProblemChanged();
};
Main.onProblemChanged = function() {
	Main.imageElement.src = "../problems/" + Main.problemInput.value + ".png";
};
Main.onImageLoad = function() {
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
