(function ($global) { "use strict";
var CutState = function() {
};
CutState.__name__ = true;
var Main = function() { };
Main.__name__ = true;
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
	Main.state = new State(Main.outputLayer,Main.borderLayer);
	Main.problemInput = window.document.getElementById("problem");
	Main.problemInput.onchange = Main.onProblemChanged;
	Main.problemInput.oninput = Main.onProblemChanged;
	Main.imageElement = window.document.getElementById("image");
	Main.imageElement.onload = Main.onImageLoad;
	Main.input = window.document.getElementById("input");
	Main.input.onchange = Main.onInputChanged;
	Main.input.oninput = Main.onInputChanged;
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
	Main.onInputChanged();
};
Main.onInputChanged = function() {
	Main.state.update(Main.input.value,Main.imageElement.width,Main.imageElement.height);
};
Math.__name__ = true;
var State = function(outputLayer,borderLayer) {
	this.borderLayer = borderLayer;
	this.outputLayer = outputLayer;
	this.cutState = new CutState();
};
State.__name__ = true;
State.prototype = {
	update: function(value,width,height) {
		haxe_Log.trace(value,{ fileName : "src/State.hx", lineNumber : 19, className : "State", methodName : "update", customParams : [width,height]});
	}
};
var Std = function() { };
Std.__name__ = true;
Std.string = function(s) {
	return js_Boot.__string_rec(s,"");
};
var haxe_Log = function() { };
haxe_Log.__name__ = true;
haxe_Log.formatOutput = function(v,infos) {
	var str = Std.string(v);
	if(infos == null) {
		return str;
	}
	var pstr = infos.fileName + ":" + infos.lineNumber;
	if(infos.customParams != null) {
		var _g = 0;
		var _g1 = infos.customParams;
		while(_g < _g1.length) {
			var v = _g1[_g];
			++_g;
			str += ", " + Std.string(v);
		}
	}
	return pstr + ": " + str;
};
haxe_Log.trace = function(v,infos) {
	var str = haxe_Log.formatOutput(v,infos);
	if(typeof(console) != "undefined" && console.log != null) {
		console.log(str);
	}
};
var haxe_iterators_ArrayIterator = function(array) {
	this.current = 0;
	this.array = array;
};
haxe_iterators_ArrayIterator.__name__ = true;
haxe_iterators_ArrayIterator.prototype = {
	hasNext: function() {
		return this.current < this.array.length;
	}
	,next: function() {
		return this.array[this.current++];
	}
};
var js_Boot = function() { };
js_Boot.__name__ = true;
js_Boot.__string_rec = function(o,s) {
	if(o == null) {
		return "null";
	}
	if(s.length >= 5) {
		return "<...>";
	}
	var t = typeof(o);
	if(t == "function" && (o.__name__ || o.__ename__)) {
		t = "object";
	}
	switch(t) {
	case "function":
		return "<function>";
	case "object":
		if(((o) instanceof Array)) {
			var str = "[";
			s += "\t";
			var _g = 0;
			var _g1 = o.length;
			while(_g < _g1) {
				var i = _g++;
				str += (i > 0 ? "," : "") + js_Boot.__string_rec(o[i],s);
			}
			str += "]";
			return str;
		}
		var tostr;
		try {
			tostr = o.toString;
		} catch( _g ) {
			return "???";
		}
		if(tostr != null && tostr != Object.toString && typeof(tostr) == "function") {
			var s2 = o.toString();
			if(s2 != "[object Object]") {
				return s2;
			}
		}
		var str = "{\n";
		s += "\t";
		var hasp = o.hasOwnProperty != null;
		var k = null;
		for( k in o ) {
		if(hasp && !o.hasOwnProperty(k)) {
			continue;
		}
		if(k == "prototype" || k == "__class__" || k == "__super__" || k == "__interfaces__" || k == "__properties__") {
			continue;
		}
		if(str.length != 2) {
			str += ", \n";
		}
		str += s + k + " : " + js_Boot.__string_rec(o[k],s);
		}
		s = s.substring(1);
		str += "\n" + s + "}";
		return str;
	case "string":
		return o;
	default:
		return String(o);
	}
};
String.__name__ = true;
Array.__name__ = true;
js_Boot.__toStr = ({ }).toString;
Main.main();
})({});
