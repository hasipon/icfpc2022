(function ($global) { "use strict";
var $estr = function() { return js_Boot.__string_rec(this,''); },$hxEnums = $hxEnums || {},$_;
var CutState = function(borderLayer,errorOutput) {
	this.errorOutput = errorOutput;
	this.borderLayer = borderLayer;
};
CutState.__name__ = true;
CutState.prototype = {
	init: function(width,height) {
		this.width = width;
		this.height = height;
		this.roots = [CutNode.Leaf(new PIXI.Rectangle(0,0,width,height))];
	}
	,draw: function() {
		this.borderLayer.clear();
		this._draw([],this.roots);
	}
	,_draw: function(ids,nodes) {
		var _g = 0;
		var _g1 = nodes.length;
		while(_g < _g1) {
			var i = _g++;
			ids.push(i);
			var _g2 = nodes[i];
			switch(_g2._hx_index) {
			case 0:
				var rect = _g2.rect;
				this.borderLayer.lineStyle(0.5,16711680,0.2);
				this.borderLayer.drawRect(rect.x,this.height - rect.bottom,rect.width,rect.height);
				break;
			case 1:
				var children = _g2.children;
				this._draw(ids,children);
				break;
			}
			ids.pop();
		}
	}
	,cut: function(lineNumber,ids,isX,pos) {
		this.lineNumber = lineNumber;
		var index = ids.pop();
		var parent = this.getById(ids);
		var _g = parent[index];
		switch(_g._hx_index) {
		case 0:
			var rect = _g.rect;
			var nodes = [];
			parent[index] = CutNode.Node(nodes);
			if(isX) {
				nodes.push(CutNode.Leaf(new PIXI.Rectangle(rect.x,rect.y,pos - rect.x,rect.height)));
				nodes.push(CutNode.Leaf(new PIXI.Rectangle(pos,rect.y,rect.right - pos,rect.height)));
			} else {
				nodes.push(CutNode.Leaf(new PIXI.Rectangle(rect.x,rect.y,rect.width,pos - rect.y)));
				nodes.push(CutNode.Leaf(new PIXI.Rectangle(rect.x,pos,rect.width,rect.bottom - pos)));
			}
			break;
		case 1:
			var children = _g.children;
			this.errorOutput.add(lineNumber,"unknown id. found node:" + ids.join(".") + "." + index);
			break;
		}
	}
	,getById: function(ids) {
		var result = this.roots;
		var _g = 0;
		while(_g < ids.length) {
			var id = ids[_g];
			++_g;
			var node = result[id];
			if(node == null) {
				this.errorOutput.add(this.lineNumber,"unknown id:" + ids.join("."));
			}
			switch(node._hx_index) {
			case 0:
				var _g1 = node.rect;
				this.errorOutput.add(this.lineNumber,"unknown id. found leaf:" + ids.join("."));
				break;
			case 1:
				var children = node.children;
				result = children;
				break;
			}
		}
		return result;
	}
};
var CutNode = $hxEnums["CutNode"] = { __ename__:true,__constructs__:null
	,Leaf: ($_=function(rect) { return {_hx_index:0,rect:rect,__enum__:"CutNode",toString:$estr}; },$_._hx_name="Leaf",$_.__params__ = ["rect"],$_)
	,Node: ($_=function(children) { return {_hx_index:1,children:children,__enum__:"CutNode",toString:$estr}; },$_._hx_name="Node",$_.__params__ = ["children"],$_)
};
CutNode.__constructs__ = [CutNode.Leaf,CutNode.Node];
var EReg = function(r,opt) {
	this.r = new RegExp(r,opt.split("u").join(""));
};
EReg.__name__ = true;
EReg.prototype = {
	split: function(s) {
		var d = "#__delim__#";
		return s.replace(this.r,d).split(d);
	}
};
var ErrorOutput = function() {
	this.text = "";
};
ErrorOutput.__name__ = true;
ErrorOutput.prototype = {
	add: function(lineNumber,message) {
		this.text += lineNumber + ": " + message + "\n";
	}
};
var HxOverrides = function() { };
HxOverrides.__name__ = true;
HxOverrides.cca = function(s,index) {
	var x = s.charCodeAt(index);
	if(x != x) {
		return undefined;
	}
	return x;
};
HxOverrides.substr = function(s,pos,len) {
	if(len == null) {
		len = s.length;
	} else if(len < 0) {
		if(pos == 0) {
			len = s.length + len;
		} else {
			return "";
		}
	}
	return s.substr(pos,len);
};
HxOverrides.now = function() {
	return Date.now();
};
var Main = function() { };
Main.__name__ = true;
Main.main = function() {
	Main.mainCanvas = window.document.getElementById("output");
	Main.mainPixi = new PIXI.Application({ view : Main.mainCanvas, transparent : true, width : Main.mainCanvas.width, height : Main.mainCanvas.height, autoResize : true});
	Main.mainPixi.stage.interactive = true;
	Main.mainPixi.stage.addChild(Main.outputLayer = new PIXI.Graphics());
	Main.mainPixi.stage.addChild(Main.problemLayer = new PIXI.Sprite());
	Main.mainPixi.stage.addChild(Main.borderLayer = new PIXI.Graphics());
	Main.mainPixi.stage.scale.x = 2.0;
	Main.mainPixi.stage.scale.y = 2.0;
	Main.mainPixi.stage.on("mousemove",Main.onMouseMove);
	Main.outputLayer.x = Main.outputLayer.y = 20;
	Main.problemLayer.x = Main.problemLayer.y = 20;
	Main.borderLayer.x = Main.borderLayer.y = 20;
	Main.problemLayer.alpha = 0.3;
	Main.state = new State(Main.outputLayer,Main.borderLayer,Main.errorOutput = new ErrorOutput());
	Main.problemInput = window.document.getElementById("problem");
	Main.problemInput.onchange = Main.onProblemChanged;
	Main.problemInput.oninput = Main.onProblemChanged;
	Main.imageElement = window.document.getElementById("image");
	Main.imageElement.onload = Main.onImageLoad;
	Main.input = window.document.getElementById("input");
	Main.input.onchange = Main.onInputChanged;
	Main.input.oninput = Main.onInputChanged;
	window.addEventListener("hashchange",Main.readHash,false);
	Main.readHash();
};
Main.readHash = function() {
	var hash = $global.location.hash;
	var index = hash.indexOf(";");
	if(index > 0) {
		var inputText = HxOverrides.substr(hash,index + 1,null);
		Main.input.value = decodeURIComponent(inputText.split("+").join(" "));
	} else {
		index = hash.length;
	}
	var problem = Std.parseInt(hash.substring(1,index));
	if(problem != null && problem > 0) {
		Main.problemInput.value = "" + problem;
	}
	Main.onProblemChanged();
	Main.setHash();
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
	try {
		Main.errorOutput.text = "";
		Main.state.update(Main.input.value,Main.imageElement.width,Main.imageElement.height);
	} catch( _g ) {
	}
	var error = window.document.getElementById("error");
	error.innerText = Main.errorOutput.text;
	Main.setHash();
};
Main.setHash = function() {
	var tmp = "#" + Std.parseInt(Main.problemInput.value) + ";";
	var s = Main.input.value;
	var tmp1 = encodeURIComponent(s);
	$global.location.hash = tmp + tmp1;
};
Main.onMouseMove = function(e) {
	var point = Main.problemLayer.toLocal(new PIXI.Point(e.data.global.x,e.data.global.y));
	var text = window.document.getElementById("point");
	text.innerText = Math.round(point.x) + "," + Math.round(Main.state.cutState.height - point.y);
};
Math.__name__ = true;
var State = function(outputLayer,borderLayer,errorOutput) {
	this.errorOutput = errorOutput;
	this.outputLayer = outputLayer;
	this.cutState = new CutState(borderLayer,errorOutput);
};
State.__name__ = true;
State.prototype = {
	update: function(value,width,height) {
		this.cutState.init(width,height);
		var lines = State.NL.split(value);
		var index = 0;
		var _g = 0;
		while(_g < lines.length) {
			var line = lines[_g];
			++_g;
			++index;
			line = StringTools.trim(line);
			if(line == "") {
				continue;
			}
			var args = this.parseLine(index,line);
			var _g1 = args[0];
			if(_g1 == "cut") {
				this.cutState.cut(index,this.parsePosition(args[1]),args[2] == "x",Std.parseInt(args[3]));
			} else {
				var x = _g1;
				this.errorOutput.add(index,"unknown move: " + x + " : in " + line);
			}
		}
		this.cutState.draw();
	}
	,parseLine: function(index,line) {
		var result = [];
		var args = State.S.split(line);
		result.push(args[0]);
		var command = null;
		var _g = 1;
		var _g1 = args.length;
		while(_g < _g1) {
			var i = _g++;
			var token = args[i];
			if(command == null) {
				if(StringTools.startsWith(token,"[")) {
					token = HxOverrides.substr(token,1,null);
					command = "";
				} else {
					this.errorOutput.add(index,"unknown matched [ : in " + line);
					continue;
				}
			} else {
				command += " ";
			}
			if(StringTools.endsWith(token,"]")) {
				command += HxOverrides.substr(token,0,token.length - 1);
				result.push(command);
				command = null;
			} else {
				command += token;
			}
		}
		return result;
	}
	,parsePosition: function(line) {
		var args = line.split(".");
		var _g = [];
		var _g1 = 0;
		while(_g1 < args.length) {
			var arg = args[_g1];
			++_g1;
			_g.push(Std.parseInt(arg));
		}
		return _g;
	}
};
var Std = function() { };
Std.__name__ = true;
Std.parseInt = function(x) {
	if(x != null) {
		var _g = 0;
		var _g1 = x.length;
		while(_g < _g1) {
			var i = _g++;
			var c = x.charCodeAt(i);
			if(c <= 8 || c >= 14 && c != 32 && c != 45) {
				var nc = x.charCodeAt(i + 1);
				var v = parseInt(x,nc == 120 || nc == 88 ? 16 : 10);
				if(isNaN(v)) {
					return null;
				} else {
					return v;
				}
			}
		}
	}
	return null;
};
var StringTools = function() { };
StringTools.__name__ = true;
StringTools.startsWith = function(s,start) {
	if(s.length >= start.length) {
		return s.lastIndexOf(start,0) == 0;
	} else {
		return false;
	}
};
StringTools.endsWith = function(s,end) {
	var elen = end.length;
	var slen = s.length;
	if(slen >= elen) {
		return s.indexOf(end,slen - elen) == slen - elen;
	} else {
		return false;
	}
};
StringTools.isSpace = function(s,pos) {
	var c = HxOverrides.cca(s,pos);
	if(!(c > 8 && c < 14)) {
		return c == 32;
	} else {
		return true;
	}
};
StringTools.ltrim = function(s) {
	var l = s.length;
	var r = 0;
	while(r < l && StringTools.isSpace(s,r)) ++r;
	if(r > 0) {
		return HxOverrides.substr(s,r,l - r);
	} else {
		return s;
	}
};
StringTools.rtrim = function(s) {
	var l = s.length;
	var r = 0;
	while(r < l && StringTools.isSpace(s,l - r - 1)) ++r;
	if(r > 0) {
		return HxOverrides.substr(s,0,l - r);
	} else {
		return s;
	}
};
StringTools.trim = function(s) {
	return StringTools.ltrim(StringTools.rtrim(s));
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
		if(o.__enum__) {
			var e = $hxEnums[o.__enum__];
			var con = e.__constructs__[o._hx_index];
			var n = con._hx_name;
			if(con.__params__) {
				s = s + "\t";
				return n + "(" + ((function($this) {
					var $r;
					var _g = [];
					{
						var _g1 = 0;
						var _g2 = con.__params__;
						while(true) {
							if(!(_g1 < _g2.length)) {
								break;
							}
							var p = _g2[_g1];
							_g1 = _g1 + 1;
							_g.push(js_Boot.__string_rec(o[p],s));
						}
					}
					$r = _g;
					return $r;
				}(this))).join(",") + ")";
			} else {
				return n;
			}
		}
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
if(typeof(performance) != "undefined" ? typeof(performance.now) == "function" : false) {
	HxOverrides.now = performance.now.bind(performance);
}
String.__name__ = true;
Array.__name__ = true;
js_Boot.__toStr = ({ }).toString;
State.NL = new EReg("\r\n|\r|\n","g");
State.S = new EReg("\\s","g");
Main.main();
})(typeof window != "undefined" ? window : typeof global != "undefined" ? global : typeof self != "undefined" ? self : this);
