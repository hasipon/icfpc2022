(function ($global) { "use strict";
var $estr = function() { return js_Boot.__string_rec(this,''); },$hxEnums = $hxEnums || {},$_;
function $extend(from, fields) {
	var proto = Object.create(from);
	for (var name in fields) proto[name] = fields[name];
	if( fields.toString !== Object.prototype.toString ) proto.toString = fields.toString;
	return proto;
}
var CutNode = $hxEnums["CutNode"] = { __ename__:true,__constructs__:null
	,Leaf: ($_=function(rect) { return {_hx_index:0,rect:rect,__enum__:"CutNode",toString:$estr}; },$_._hx_name="Leaf",$_.__params__ = ["rect"],$_)
	,Node: ($_=function(children) { return {_hx_index:1,children:children,__enum__:"CutNode",toString:$estr}; },$_._hx_name="Node",$_.__params__ = ["children"],$_)
	,Marged: {_hx_name:"Marged",_hx_index:2,__enum__:"CutNode",toString:$estr}
};
CutNode.__constructs__ = [CutNode.Leaf,CutNode.Node,CutNode.Marged];
var CutState = function(borderLayer,textLayer,errorOutput) {
	this.textLayer = textLayer;
	this.errorOutput = errorOutput;
	this.borderLayer = borderLayer;
	this.textFields = [];
	this.textPool = [];
};
CutState.__name__ = true;
CutState.prototype = {
	init: function(initialState) {
		this.width = initialState.width;
		this.height = initialState.height;
		var _g = [];
		var _g1 = 0;
		var _g2 = initialState.nodes;
		while(_g1 < _g2.length) {
			var node = _g2[_g1];
			++_g1;
			_g.push(CutNode.Leaf(node.rect));
		}
		this.roots = _g;
	}
	,draw: function() {
		this.borderLayer.clear();
		this.textLayer.removeChildren();
		while(this.textFields.length > 0) this.textPool.push(this.textFields.pop());
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
				var text = this.textPool.length > 0 ? this.textPool.pop() : new PIXI.Text("",{ fill : 16711680});
				text.text = ids.join(".");
				text.x = rect.x + 2;
				text.y = this.height - rect.bottom;
				text.alpha = 0.6;
				text.scale.x = 0.25;
				text.scale.y = 0.25;
				this.textLayer.addChild(text);
				this.textFields.push(text);
				break;
			case 1:
				var children = _g2.children;
				this._draw(ids,children);
				break;
			case 2:
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
			var _g1 = _g.children;
			this.errorOutput.add(lineNumber,"unknown id. found node:" + ids.join(".") + "." + index);
			break;
		case 2:
			this.errorOutput.add(lineNumber,"unknown id. marged:" + ids.join(".") + "." + index);
			break;
		}
	}
	,cutPoint: function(lineNumber,ids,pos) {
		this.lineNumber = lineNumber;
		var index = ids.pop();
		var parent = this.getById(ids);
		var _g = parent[index];
		switch(_g._hx_index) {
		case 0:
			var rect = _g.rect;
			var nodes = [];
			parent[index] = CutNode.Node(nodes);
			nodes.push(CutNode.Leaf(new PIXI.Rectangle(rect.x,rect.y,pos.x - rect.x,pos.y - rect.y)));
			nodes.push(CutNode.Leaf(new PIXI.Rectangle(pos.x,rect.y,rect.right - pos.x,pos.y - rect.y)));
			nodes.push(CutNode.Leaf(new PIXI.Rectangle(pos.x,pos.y,rect.right - pos.x,rect.bottom - pos.y)));
			nodes.push(CutNode.Leaf(new PIXI.Rectangle(rect.x,pos.y,pos.x - rect.x,rect.bottom - pos.y)));
			break;
		case 1:
			var _g1 = _g.children;
			this.errorOutput.add(lineNumber,"unknown id. found node:" + ids.join(".") + "." + index);
			break;
		case 2:
			this.errorOutput.add(lineNumber,"unknown id. marged:" + ids.join(".") + "." + index);
			break;
		}
	}
	,merge: function(lineNumber,id0,id1) {
		var index0 = id0.pop();
		var parent0 = this.getById(id0);
		var index1 = id1.pop();
		var parent1 = this.getById(id1);
		var node0 = parent0[index0];
		var node1 = parent1[index1];
		if(node0 == null) {
			this.errorOutput.add(lineNumber,"unknown id:" + id0.join(".") + "." + index0);
		}
		if(node1 == null) {
			this.errorOutput.add(lineNumber,"unknown id:" + id1.join(".") + "." + index1);
		}
		if(node0._hx_index == 0) {
			if(node1._hx_index == 0) {
				var rect1 = node1.rect;
				var rect0 = node0.rect;
				parent0[index0] = CutNode.Marged;
				parent1[index1] = CutNode.Marged;
				var result = new PIXI.Rectangle();
				result.x = Math.min(rect0.x,rect1.x);
				result.y = Math.min(rect0.y,rect1.y);
				result.width = Math.max(rect0.right,rect1.right) - result.x;
				result.height = Math.max(rect0.bottom,rect1.bottom) - result.y;
				if(rect0.width != rect1.width && rect0.height != rect1.height || rect0.width * rect0.height + rect1.width * rect1.height != result.width * result.height) {
					this.errorOutput.add(lineNumber,"cannot marge:" + id0.join(".") + "." + index0 + ", " + id1.join(".") + "." + index1);
					throw haxe_Exception.thrown("error");
				}
				this.roots.push(CutNode.Leaf(result));
			} else {
				this.errorOutput.add(lineNumber,"unmatched node:" + id0.join(".") + "." + index0 + ", " + id1.join(".") + "." + index1);
			}
		} else {
			this.errorOutput.add(lineNumber,"unmatched node:" + id0.join(".") + "." + index0 + ", " + id1.join(".") + "." + index1);
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
			case 2:
				this.errorOutput.add(this.lineNumber,"unknown id. marged:" + ids.join("."));
				break;
			}
		}
		return result;
	}
	,getRectangle: function(lineNumber,ids) {
		this.lineNumber = lineNumber;
		var index = ids.pop();
		var parent = this.getById(ids);
		var _g = parent[index];
		switch(_g._hx_index) {
		case 0:
			var rect = _g.rect;
			return rect;
		case 1:
			var _g1 = _g.children;
			this.errorOutput.add(lineNumber,"unknown id. found node:" + ids.join(".") + "." + index);
			throw haxe_Exception.thrown("error");
		case 2:
			this.errorOutput.add(lineNumber,"unknown id. marged:" + ids.join(".") + "." + index);
			throw haxe_Exception.thrown("error");
		}
	}
	,getNodeAt: function(x,y) {
		var ids = [];
		this._getNodeAt(x,y,ids,this.roots);
		return ids;
	}
	,_getNodeAt: function(x,y,ids,nodes) {
		var _g = 0;
		var _g1 = nodes.length;
		while(_g < _g1) {
			var i = _g++;
			ids.push(i);
			var _g2 = nodes[i];
			switch(_g2._hx_index) {
			case 0:
				var rect = _g2.rect;
				if(rect.x < x && x < rect.right && rect.y < y && y < rect.bottom) {
					return true;
				}
				break;
			case 1:
				var children = _g2.children;
				if(this._getNodeAt(x,y,ids,children)) {
					return true;
				}
				break;
			case 2:
				break;
			}
			ids.pop();
		}
		return false;
	}
};
var EReg = function(r,opt) {
	this.r = new RegExp(r,opt.split("u").join(""));
};
EReg.__name__ = true;
EReg.prototype = {
	match: function(s) {
		if(this.r.global) {
			this.r.lastIndex = 0;
		}
		this.r.m = this.r.exec(s);
		this.r.s = s;
		return this.r.m != null;
	}
	,split: function(s) {
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
var InitialNode = function(rect,color) {
	this.rect = rect;
	this.color = color;
};
InitialNode.__name__ = true;
var InitialState = function(width,height,nodes) {
	this.width = width;
	this.height = height;
	this.nodes = nodes;
};
InitialState.__name__ = true;
var Lambda = function() { };
Lambda.__name__ = true;
Lambda.exists = function(it,f) {
	var x = $getIterator(it);
	while(x.hasNext()) {
		var x1 = x.next();
		if(f(x1)) {
			return true;
		}
	}
	return false;
};
var Main = function() { };
Main.__name__ = true;
Main.main = function() {
	Main.outputCanvas = window.document.getElementById("output");
	Main.mainCanvas = window.document.getElementById("pixi");
	Main.mainPixi = new PIXI.Application({ view : Main.mainCanvas, transparent : true, width : Main.mainCanvas.width, height : Main.mainCanvas.height, autoResize : true});
	Main.mainPixi.stage.interactive = true;
	Main.mainPixi.stage.addChild(Main.problemLayer = new PIXI.Sprite());
	Main.mainPixi.stage.addChild(Main.borderLayer = new PIXI.Graphics());
	Main.mainPixi.stage.addChild(Main.textLayer = new PIXI.Sprite());
	Main.mainPixi.stage.addChild(Main.scouterLayer = new PIXI.Graphics());
	Main.mainPixi.stage.scale.x = 2.0;
	Main.mainPixi.stage.scale.y = 2.0;
	Main.mainPixi.stage.on("mousedown",Main.onMouseDown);
	Main.problemLayer.x = Main.problemLayer.y = 5;
	Main.borderLayer.x = Main.borderLayer.y = 5;
	Main.scouterLayer.x = Main.scouterLayer.y = 5;
	Main.textLayer.x = Main.textLayer.y = 5;
	Main.problemLayer.alpha = 0.3;
	Main.state = new State(Main.outputCanvas.getContext("2d",null),Main.borderLayer,Main.textLayer,Main.errorOutput = new ErrorOutput());
	Main.scouter = new Scouter(Main.state,Main.scouterLayer);
	Main.problemInput = window.document.getElementById("problem");
	Main.problemInput.onchange = Main.onProblemChanged;
	Main.problemInput.oninput = Main.onProblemChanged;
	Main.imageElement = window.document.getElementById("image");
	Main.imageElement.onload = Main.onImageLoad;
	Main.input = window.document.getElementById("input");
	Main.input.onchange = Main.onInputChanged;
	Main.input.oninput = Main.onInputChanged;
	window.addEventListener("keydown",Main.onKey,false);
	window.addEventListener("hashchange",Main.readHash,false);
	Main.readHash();
};
Main.onKey = function(e) {
	if(Main.initialState == null) {
		return;
	}
	if(window.document.activeElement == Main.problemInput || window.document.activeElement == Main.input) {
		return;
	}
	if(e.ctrlKey) {
		switch(e.keyCode) {
		case 89:
			if(Main.history.length > 0) {
				var result = State.NL.split(Main.input.value);
				result.push(Main.history.pop());
				Main.input.value = result.join("\n");
			}
			break;
		case 90:
			var result = State.NL.split(Main.input.value);
			Main.history.push(result.pop());
			Main.input.value = result.join("\n");
			break;
		default:
			return;
		}
		Main.onInputChanged();
		e.preventDefault();
		var error = window.document.getElementById("error");
		error.innerText = Main.errorOutput.text;
	} else {
		switch(e.keyCode) {
		case 65:
			Main.input.value += Main.state.getLineCut(true,Main.scouter.left,Main.initialState.height - Main.scouter.y);
			break;
		case 67:
			Main.input.value += Main.state.getPointCut(Main.scouter.right,Main.initialState.height - Main.scouter.bottom);
			break;
		case 68:
			Main.input.value += Main.state.getLineCut(true,Main.scouter.right,Main.initialState.height - Main.scouter.y);
			break;
		case 69:
			Main.input.value += Main.state.getPointCut(Main.scouter.right,Main.initialState.height - Main.scouter.top);
			break;
		case 81:
			Main.input.value += Main.state.getPointCut(Main.scouter.left,Main.initialState.height - Main.scouter.top);
			break;
		case 83:
			Main.input.value += Main.state.getPointCut(Main.scouter.x,Main.initialState.height - Main.scouter.y);
			break;
		case 87:
			Main.input.value += Main.state.getLineCut(false,Main.scouter.x,Main.initialState.height - Main.scouter.top);
			break;
		case 88:
			Main.input.value += Main.state.getLineCut(false,Main.scouter.x,Main.initialState.height - Main.scouter.bottom);
			break;
		case 90:
			Main.input.value += Main.state.getPointCut(Main.scouter.left,Main.initialState.height - Main.scouter.bottom);
			break;
		default:
			return;
		}
		Main._onInputChanged();
		e.preventDefault();
		var error = window.document.getElementById("error");
		error.innerText = Main.errorOutput.text;
	}
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
	var num = Std.parseInt(Main.problemInput.value);
	if(num > 25) {
		var url = "../problems/" + num + ".initial.json";
		var http = new haxe_http_HttpJs(url);
		http.onData = Main.onJsonLoaded;
		http.request();
	} else {
		Main.initialState = new InitialState(400,400,[new InitialNode(new PIXI.Rectangle(0,0,400,400),new tweenxcore_color_ArgbColor(0.,0.,0.,0.))]);
		Main.onInputChanged();
	}
};
Main.onJsonLoaded = function(data) {
	var data1 = JSON.parse(data);
	var nodes = [];
	var _g = 0;
	var _g1 = data1.blocks;
	while(_g < _g1.length) {
		var block = _g1[_g];
		++_g;
		haxe_Log.trace(block.color[3],{ fileName : "src/Main.hx", lineNumber : 212, className : "Main", methodName : "onJsonLoaded", customParams : [block.color[0],block.color[1],block.color[2]]});
		nodes[Std.parseInt(block.blockId)] = new InitialNode(new PIXI.Rectangle(block.bottomLeft[1],block.bottomLeft[0],block.topRight[1] - block.bottomLeft[1],block.topRight[0] - block.bottomLeft[0]),new tweenxcore_color_ArgbColor(block.color[3] / 255,block.color[0] / 255,block.color[1] / 255,block.color[2] / 255));
	}
	Main.initialState = new InitialState(data1.width,data1.height,nodes);
	Main.onInputChanged();
};
Main._onInputChanged = function() {
	Main.history = [];
	Main.onInputChanged();
};
Main.onInputChanged = function() {
	if(Main.initialState == null) {
		return;
	}
	try {
		Main.errorOutput.text = "";
		Main.state.update(Main.input.value,Main.initialState);
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
Main.onMouseDown = function(e) {
	var point = Main.problemLayer.toLocal(new PIXI.Point(e.data.global.x,e.data.global.y));
	Main.scouter.update(Math.round(point.x),Math.round(point.y),Main.imageElement);
	var color = Main.scouter.pixel;
	var color1 = new tweenxcore_color_ArgbColor((color >>> 24 & 255) / 255,(color >> 16 & 255) / 255,(color >> 8 & 255) / 255,(color & 255) / 255);
	var text = window.document.getElementById("point");
	text.innerText = Math.round(point.x) + "," + Math.round(Main.state.cutState.height - point.y) + "," + "[" + color1.r * 255 + "," + color1.g * 255 + "," + color1.b * 255 + "," + color1.a * 255 + "]";
};
Math.__name__ = true;
var Reflect = function() { };
Reflect.__name__ = true;
Reflect.isFunction = function(f) {
	if(typeof(f) == "function") {
		return !(f.__name__ || f.__ename__);
	} else {
		return false;
	}
};
Reflect.compareMethods = function(f1,f2) {
	if(f1 == f2) {
		return true;
	}
	if(!Reflect.isFunction(f1) || !Reflect.isFunction(f2)) {
		return false;
	}
	if(f1.scope == f2.scope && f1.method == f2.method) {
		return f1.method != null;
	} else {
		return false;
	}
};
var Scouter = function(state,scouterLayer) {
	this.canvas = window.document.createElement("canvas");
	window.document.body.appendChild(this.canvas);
	this.canvas.width = 400;
	this.canvas.height = 400;
	this.canvas.style.display = "none";
	this.context = this.canvas.getContext("2d");
	this.state = state;
	this.scouterLayer = scouterLayer;
};
Scouter.__name__ = true;
Scouter.prototype = {
	update: function(x,y,image) {
		this.x = x;
		this.y = y;
		this.context.fillStyle = "white";
		this.context.drawImage(image,0,0,400,400);
		this.pixel = this.getPixel(x,y);
		this.left = x;
		this.right = x;
		this.top = y;
		this.bottom = y;
		while(0 < this.left) {
			this.left -= 1;
			var pixel2 = this.getPixel(this.left,y);
			if(pixel2 != this.pixel) {
				break;
			}
		}
		while(0 < this.top) {
			this.top -= 1;
			var pixel2 = this.getPixel(x,this.top);
			if(pixel2 != this.pixel) {
				break;
			}
		}
		while(400 > this.right) {
			this.right += 1;
			var pixel2 = this.getPixel(this.right,y);
			if(pixel2 != this.pixel) {
				break;
			}
		}
		while(400 > this.bottom) {
			this.bottom += 1;
			var pixel2 = this.getPixel(x,this.bottom);
			if(pixel2 != this.pixel) {
				break;
			}
		}
		this.scouterLayer.clear();
		this.scouterLayer.lineStyle(0.5,65280,0.8);
		this.scouterLayer.moveTo(-20,this.top);
		this.scouterLayer.lineTo(420,this.top);
		this.scouterLayer.moveTo(-20,this.bottom);
		this.scouterLayer.lineTo(420,this.bottom);
		this.scouterLayer.moveTo(this.left,-20);
		this.scouterLayer.lineTo(this.left,420);
		this.scouterLayer.moveTo(this.right,-20);
		this.scouterLayer.lineTo(this.right,420);
	}
	,getPixel: function(x,y) {
		var data = this.context.getImageData(x,y,1,1).data;
		return data[3] << 24 | data[0] << 16 | data[1] << 8 | data[2];
	}
};
var State = function(outputLayer,borderLayer,textLayer,errorOutput) {
	this.errorOutput = errorOutput;
	this.outputLayer = outputLayer;
	this.cutState = new CutState(borderLayer,textLayer,errorOutput);
};
State.__name__ = true;
State.prototype = {
	update: function(value,initialState) {
		this.outputLayer.fillStyle = "white";
		this.outputLayer.fillRect(0,0,initialState.width,initialState.height);
		var _g = 0;
		var _g1 = initialState.nodes;
		while(_g < _g1.length) {
			var node = _g1[_g];
			++_g;
			var _this = node.color;
			this.outputLayer.fillStyle = "rgba(" + (_this.r * 255 | 0) + "," + (_this.g * 255 | 0) + "," + (_this.b * 255 | 0) + "," + _this.a + ")";
			this.outputLayer.fillRect(node.rect.x,node.rect.y,node.rect.width,node.rect.height);
		}
		this.cutState.init(initialState);
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
			switch(_g1) {
			case "color":
				if(args.length == 3) {
					var rect = this.cutState.getRectangle(index,this.parseId(args[1]));
					var color = this.parseRgba(args[2]);
					var rate = color.a;
					color.r = 1 - rate + color.r * rate;
					var rate1 = color.a;
					color.g = 1 - rate1 + color.g * rate1;
					var rate2 = color.a;
					color.b = 1 - rate2 + color.b * rate2;
					this.outputLayer.fillStyle = "rgb(" + (color.r * 255 | 0) + "," + (color.g * 255 | 0) + "," + (color.b * 255 | 0) + ")";
					this.outputLayer.fillRect(rect.x,this.cutState.height - rect.bottom,rect.width,rect.height);
				} else {
					this.errorOutput.add(index,"too many arguments : in " + line);
				}
				break;
			case "cut":
				if(args.length == 3) {
					this.cutState.cutPoint(index,this.parseId(args[1]),this.parsePoint(args[2]));
				} else if(args.length == 4) {
					this.cutState.cut(index,this.parseId(args[1]),args[2] == "x" || args[2] == "X",Std.parseInt(args[3]));
				} else {
					this.errorOutput.add(index,"too many arguments : in " + line);
				}
				break;
			case "merge":
				if(args.length == 3) {
					this.cutState.merge(index,this.parseId(args[1]),this.parseId(args[2]));
				} else {
					this.errorOutput.add(index,"too many arguments : in " + line);
				}
				break;
			case "swap":
				if(args.length == 3) {
					var rect0 = this.cutState.getRectangle(index,this.parseId(args[1]));
					var rect1 = this.cutState.getRectangle(index,this.parseId(args[2]));
					if(rect0.width != rect1.width || rect0.height != rect1.height) {
						this.errorOutput.add(index,"unmatched size : in " + line);
						return;
					}
					var image0 = this.outputLayer.getImageData(rect0.x,this.cutState.height - rect0.bottom,rect0.width,rect0.height);
					var image1 = this.outputLayer.getImageData(rect1.x,this.cutState.height - rect1.bottom,rect1.width,rect1.height);
					this.outputLayer.putImageData(image1,rect0.x,this.cutState.height - rect0.bottom);
					this.outputLayer.putImageData(image0,rect1.x,this.cutState.height - rect1.bottom);
				} else {
					this.errorOutput.add(index,"too many arguments : in " + line);
				}
				break;
			default:
				var x = _g1;
				this.errorOutput.add(index,"unknown move: " + x + " : in " + line);
			}
		}
		this.cutState.draw();
	}
	,getLineCut: function(isX,x,y) {
		var id = this.cutState.getNodeAt(x,y);
		if(id.length == 0) {
			this.errorOutput.add(0,"not found parent rect");
			return "";
		}
		return "\ncut [" + id.join(".") + "] [" + (isX ? "x" : "y") + "] [" + (isX ? x : y) + "]";
	}
	,getPointCut: function(x,y) {
		var id = this.cutState.getNodeAt(x,y);
		if(id.length == 0) {
			this.errorOutput.add(0,"not found parent rect");
			return "";
		}
		return "\ncut [" + id.join(".") + "] [" + x + "," + y + "]";
	}
	,parsePoint: function(string) {
		var args = string.split(",");
		return new PIXI.Point(Std.parseInt(args[0]),Std.parseInt(args[1]));
	}
	,parseRgba: function(string) {
		var args = string.split(",");
		return new tweenxcore_color_ArgbColor(Std.parseInt(args[3]) / 255,Std.parseInt(args[0]) / 255,Std.parseInt(args[1]) / 255,Std.parseInt(args[2]) / 255);
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
	,parseId: function(line) {
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
Std.string = function(s) {
	return js_Boot.__string_rec(s,"");
};
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
StringTools.hex = function(n,digits) {
	var s = "";
	var hexChars = "0123456789ABCDEF";
	while(true) {
		s = hexChars.charAt(n & 15) + s;
		n >>>= 4;
		if(!(n > 0)) {
			break;
		}
	}
	if(digits != null) {
		while(s.length < digits) s = "0" + s;
	}
	return s;
};
var haxe_Exception = function(message,previous,native) {
	Error.call(this,message);
	this.message = message;
	this.__previousException = previous;
	this.__nativeException = native != null ? native : this;
};
haxe_Exception.__name__ = true;
haxe_Exception.caught = function(value) {
	if(((value) instanceof haxe_Exception)) {
		return value;
	} else if(((value) instanceof Error)) {
		return new haxe_Exception(value.message,null,value);
	} else {
		return new haxe_ValueException(value,null,value);
	}
};
haxe_Exception.thrown = function(value) {
	if(((value) instanceof haxe_Exception)) {
		return value.get_native();
	} else if(((value) instanceof Error)) {
		return value;
	} else {
		var e = new haxe_ValueException(value);
		return e;
	}
};
haxe_Exception.__super__ = Error;
haxe_Exception.prototype = $extend(Error.prototype,{
	unwrap: function() {
		return this.__nativeException;
	}
	,get_native: function() {
		return this.__nativeException;
	}
});
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
var haxe_ValueException = function(value,previous,native) {
	haxe_Exception.call(this,String(value),previous,native);
	this.value = value;
};
haxe_ValueException.__name__ = true;
haxe_ValueException.__super__ = haxe_Exception;
haxe_ValueException.prototype = $extend(haxe_Exception.prototype,{
	unwrap: function() {
		return this.value;
	}
});
var haxe_http_HttpBase = function(url) {
	this.url = url;
	this.headers = [];
	this.params = [];
	this.emptyOnData = $bind(this,this.onData);
};
haxe_http_HttpBase.__name__ = true;
haxe_http_HttpBase.prototype = {
	onData: function(data) {
	}
	,onBytes: function(data) {
	}
	,onError: function(msg) {
	}
	,onStatus: function(status) {
	}
	,hasOnData: function() {
		return !Reflect.compareMethods($bind(this,this.onData),this.emptyOnData);
	}
	,success: function(data) {
		this.responseBytes = data;
		this.responseAsString = null;
		if(this.hasOnData()) {
			this.onData(this.get_responseData());
		}
		this.onBytes(this.responseBytes);
	}
	,get_responseData: function() {
		if(this.responseAsString == null && this.responseBytes != null) {
			this.responseAsString = this.responseBytes.getString(0,this.responseBytes.length,haxe_io_Encoding.UTF8);
		}
		return this.responseAsString;
	}
};
var haxe_http_HttpJs = function(url) {
	this.async = true;
	this.withCredentials = false;
	haxe_http_HttpBase.call(this,url);
};
haxe_http_HttpJs.__name__ = true;
haxe_http_HttpJs.__super__ = haxe_http_HttpBase;
haxe_http_HttpJs.prototype = $extend(haxe_http_HttpBase.prototype,{
	request: function(post) {
		var _gthis = this;
		this.responseAsString = null;
		this.responseBytes = null;
		var r = this.req = js_Browser.createXMLHttpRequest();
		var onreadystatechange = function(_) {
			if(r.readyState != 4) {
				return;
			}
			var s;
			try {
				s = r.status;
			} catch( _g ) {
				s = null;
			}
			if(s == 0 && js_Browser.get_supported() && $global.location != null) {
				var protocol = $global.location.protocol.toLowerCase();
				var rlocalProtocol = new EReg("^(?:about|app|app-storage|.+-extension|file|res|widget):$","");
				var isLocal = rlocalProtocol.match(protocol);
				if(isLocal) {
					s = r.response != null ? 200 : 404;
				}
			}
			if(s == undefined) {
				s = null;
			}
			if(s != null) {
				_gthis.onStatus(s);
			}
			if(s != null && s >= 200 && s < 400) {
				_gthis.req = null;
				_gthis.success(haxe_io_Bytes.ofData(r.response));
			} else if(s == null || s == 0 && r.response == null) {
				_gthis.req = null;
				_gthis.onError("Failed to connect or resolve host");
			} else if(s == null) {
				_gthis.req = null;
				var onreadystatechange = r.response != null ? haxe_io_Bytes.ofData(r.response) : null;
				_gthis.responseBytes = onreadystatechange;
				_gthis.onError("Http Error #" + r.status);
			} else {
				switch(s) {
				case 12007:
					_gthis.req = null;
					_gthis.onError("Unknown host");
					break;
				case 12029:
					_gthis.req = null;
					_gthis.onError("Failed to connect to host");
					break;
				default:
					_gthis.req = null;
					var onreadystatechange = r.response != null ? haxe_io_Bytes.ofData(r.response) : null;
					_gthis.responseBytes = onreadystatechange;
					_gthis.onError("Http Error #" + r.status);
				}
			}
		};
		if(this.async) {
			r.onreadystatechange = onreadystatechange;
		}
		var uri;
		var _g = this.postData;
		var _g1 = this.postBytes;
		if(_g == null) {
			if(_g1 == null) {
				uri = null;
			} else {
				var bytes = _g1;
				uri = new Blob([bytes.b.bufferValue]);
			}
		} else if(_g1 == null) {
			var str = _g;
			uri = str;
		} else {
			uri = null;
		}
		if(uri != null) {
			post = true;
		} else {
			var _g = 0;
			var _g1 = this.params;
			while(_g < _g1.length) {
				var p = _g1[_g];
				++_g;
				if(uri == null) {
					uri = "";
				} else {
					uri = (uri == null ? "null" : Std.string(uri)) + "&";
				}
				var s = p.name;
				var value = (uri == null ? "null" : Std.string(uri)) + encodeURIComponent(s) + "=";
				var s1 = p.value;
				uri = value + encodeURIComponent(s1);
			}
		}
		try {
			if(post) {
				r.open("POST",this.url,this.async);
			} else if(uri != null) {
				var question = this.url.split("?").length <= 1;
				r.open("GET",this.url + (question ? "?" : "&") + (uri == null ? "null" : Std.string(uri)),this.async);
				uri = null;
			} else {
				r.open("GET",this.url,this.async);
			}
			r.responseType = "arraybuffer";
		} catch( _g ) {
			var e = haxe_Exception.caught(_g).unwrap();
			this.req = null;
			this.onError(e.toString());
			return;
		}
		r.withCredentials = this.withCredentials;
		if(!Lambda.exists(this.headers,function(h) {
			return h.name == "Content-Type";
		}) && post && this.postData == null) {
			r.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		}
		var _g = 0;
		var _g1 = this.headers;
		while(_g < _g1.length) {
			var h = _g1[_g];
			++_g;
			r.setRequestHeader(h.name,h.value);
		}
		r.send(uri);
		if(!this.async) {
			onreadystatechange(null);
		}
	}
});
var haxe_io_Bytes = function(data) {
	this.length = data.byteLength;
	this.b = new Uint8Array(data);
	this.b.bufferValue = data;
	data.hxBytes = this;
	data.bytes = this.b;
};
haxe_io_Bytes.__name__ = true;
haxe_io_Bytes.ofData = function(b) {
	var hb = b.hxBytes;
	if(hb != null) {
		return hb;
	}
	return new haxe_io_Bytes(b);
};
haxe_io_Bytes.prototype = {
	getString: function(pos,len,encoding) {
		if(pos < 0 || len < 0 || pos + len > this.length) {
			throw haxe_Exception.thrown(haxe_io_Error.OutsideBounds);
		}
		if(encoding == null) {
			encoding = haxe_io_Encoding.UTF8;
		}
		var s = "";
		var b = this.b;
		var i = pos;
		var max = pos + len;
		switch(encoding._hx_index) {
		case 0:
			var debug = pos > 0;
			while(i < max) {
				var c = b[i++];
				if(c < 128) {
					if(c == 0) {
						break;
					}
					s += String.fromCodePoint(c);
				} else if(c < 224) {
					var code = (c & 63) << 6 | b[i++] & 127;
					s += String.fromCodePoint(code);
				} else if(c < 240) {
					var c2 = b[i++];
					var code1 = (c & 31) << 12 | (c2 & 127) << 6 | b[i++] & 127;
					s += String.fromCodePoint(code1);
				} else {
					var c21 = b[i++];
					var c3 = b[i++];
					var u = (c & 15) << 18 | (c21 & 127) << 12 | (c3 & 127) << 6 | b[i++] & 127;
					s += String.fromCodePoint(u);
				}
			}
			break;
		case 1:
			while(i < max) {
				var c = b[i++] | b[i++] << 8;
				s += String.fromCodePoint(c);
			}
			break;
		}
		return s;
	}
};
var haxe_io_Encoding = $hxEnums["haxe.io.Encoding"] = { __ename__:true,__constructs__:null
	,UTF8: {_hx_name:"UTF8",_hx_index:0,__enum__:"haxe.io.Encoding",toString:$estr}
	,RawNative: {_hx_name:"RawNative",_hx_index:1,__enum__:"haxe.io.Encoding",toString:$estr}
};
haxe_io_Encoding.__constructs__ = [haxe_io_Encoding.UTF8,haxe_io_Encoding.RawNative];
var haxe_io_Error = $hxEnums["haxe.io.Error"] = { __ename__:true,__constructs__:null
	,Blocked: {_hx_name:"Blocked",_hx_index:0,__enum__:"haxe.io.Error",toString:$estr}
	,Overflow: {_hx_name:"Overflow",_hx_index:1,__enum__:"haxe.io.Error",toString:$estr}
	,OutsideBounds: {_hx_name:"OutsideBounds",_hx_index:2,__enum__:"haxe.io.Error",toString:$estr}
	,Custom: ($_=function(e) { return {_hx_index:3,e:e,__enum__:"haxe.io.Error",toString:$estr}; },$_._hx_name="Custom",$_.__params__ = ["e"],$_)
};
haxe_io_Error.__constructs__ = [haxe_io_Error.Blocked,haxe_io_Error.Overflow,haxe_io_Error.OutsideBounds,haxe_io_Error.Custom];
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
var js_Browser = function() { };
js_Browser.__name__ = true;
js_Browser.get_supported = function() {
	if(typeof(window) != "undefined" && typeof(window.location) != "undefined") {
		return typeof(window.location.protocol) == "string";
	} else {
		return false;
	}
};
js_Browser.createXMLHttpRequest = function() {
	if(typeof XMLHttpRequest != "undefined") {
		return new XMLHttpRequest();
	}
	if(typeof ActiveXObject != "undefined") {
		return new ActiveXObject("Microsoft.XMLHTTP");
	}
	throw haxe_Exception.thrown("Unable to create XMLHttpRequest object.");
};
var tweenxcore_Easing = function() { };
tweenxcore_Easing.__name__ = true;
tweenxcore_Easing.linear = function(t) {
	return t;
};
tweenxcore_Easing.sineIn = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		return 1 - Math.cos(t * 1.5707963267948966);
	}
};
tweenxcore_Easing.sineOut = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		return Math.sin(t * 1.5707963267948966);
	}
};
tweenxcore_Easing.sineInOut = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		return -0.5 * (Math.cos(3.1415926535897932384626433832795 * t) - 1);
	}
};
tweenxcore_Easing.sineOutIn = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else if(t < 0.5) {
		return 0.5 * Math.sin(t * 2 * 1.5707963267948966);
	} else {
		return -0.5 * Math.cos((t * 2 - 1) * 1.5707963267948966) + 1;
	}
};
tweenxcore_Easing.quadIn = function(t) {
	return t * t;
};
tweenxcore_Easing.quadOut = function(t) {
	return -t * (t - 2);
};
tweenxcore_Easing.quadInOut = function(t) {
	if(t < 0.5) {
		return 2 * t * t;
	} else {
		return -2 * (--t * t) + 1;
	}
};
tweenxcore_Easing.quadOutIn = function(t) {
	if(t < 0.5) {
		return -0.5 * (t *= 2) * (t - 2);
	} else {
		t = t * 2 - 1;
		return 0.5 * t * t + 0.5;
	}
};
tweenxcore_Easing.cubicIn = function(t) {
	return t * t * t;
};
tweenxcore_Easing.cubicOut = function(t) {
	return --t * t * t + 1;
};
tweenxcore_Easing.cubicInOut = function(t) {
	if((t *= 2) < 1) {
		return 0.5 * t * t * t;
	} else {
		return 0.5 * ((t -= 2) * t * t + 2);
	}
};
tweenxcore_Easing.cubicOutIn = function(t) {
	t = t * 2 - 1;
	return 0.5 * (t * t * t + 1);
};
tweenxcore_Easing.quartIn = function(t) {
	return (t *= t) * t;
};
tweenxcore_Easing.quartOut = function(t) {
	t = --t * t;
	return 1 - t * t;
};
tweenxcore_Easing.quartInOut = function(t) {
	if((t *= 2) < 1) {
		return 0.5 * (t *= t) * t;
	} else {
		t = (t -= 2) * t;
		return -0.5 * (t * t - 2);
	}
};
tweenxcore_Easing.quartOutIn = function(t) {
	if(t < 0.5) {
		t = t * 2 - 1;
		return -0.5 * (t *= t) * t + 0.5;
	} else {
		t = t * 2 - 1;
		return 0.5 * (t *= t) * t + 0.5;
	}
};
tweenxcore_Easing.quintIn = function(t) {
	return t * (t *= t) * t;
};
tweenxcore_Easing.quintOut = function(t) {
	return --t * (t *= t) * t + 1;
};
tweenxcore_Easing.quintInOut = function(t) {
	if((t *= 2) < 1) {
		return 0.5 * t * (t *= t) * t;
	} else {
		return 0.5 * (t -= 2) * (t *= t) * t + 1;
	}
};
tweenxcore_Easing.quintOutIn = function(t) {
	t = t * 2 - 1;
	return 0.5 * (t * (t *= t) * t + 1);
};
tweenxcore_Easing.expoIn = function(t) {
	if(t == 0) {
		return 0;
	} else {
		return Math.exp(6.931471805599453 * (t - 1));
	}
};
tweenxcore_Easing.expoOut = function(t) {
	if(t == 1) {
		return 1;
	} else {
		return 1 - Math.exp(-6.9314718055994531 * t);
	}
};
tweenxcore_Easing.expoInOut = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else if((t *= 2) < 1) {
		return 0.5 * Math.exp(6.931471805599453 * (t - 1));
	} else {
		return 0.5 * (2 - Math.exp(-6.9314718055994531 * (t - 1)));
	}
};
tweenxcore_Easing.expoOutIn = function(t) {
	if(t < 0.5) {
		return 0.5 * (1 - Math.exp(-13.862943611198906 * t));
	} else if(t == 0.5) {
		return 0.5;
	} else {
		return 0.5 * (Math.exp(13.862943611198906 * (t - 1)) + 1);
	}
};
tweenxcore_Easing.circIn = function(t) {
	if(t < -1 || 1 < t) {
		return 0;
	} else {
		return 1 - Math.sqrt(1 - t * t);
	}
};
tweenxcore_Easing.circOut = function(t) {
	if(t < 0 || 2 < t) {
		return 0;
	} else {
		return Math.sqrt(t * (2 - t));
	}
};
tweenxcore_Easing.circInOut = function(t) {
	if(t < -0.5 || 1.5 < t) {
		return 0.5;
	} else if((t *= 2) < 1) {
		return -0.5 * (Math.sqrt(1 - t * t) - 1);
	} else {
		return 0.5 * (Math.sqrt(1 - (t -= 2) * t) + 1);
	}
};
tweenxcore_Easing.circOutIn = function(t) {
	if(t < 0) {
		return 0;
	} else if(1 < t) {
		return 1;
	} else if(t < 0.5) {
		t = t * 2 - 1;
		return 0.5 * Math.sqrt(1 - t * t);
	} else {
		t = t * 2 - 1;
		return -0.5 * (Math.sqrt(1 - t * t) - 1 - 1);
	}
};
tweenxcore_Easing.bounceIn = function(t) {
	t = 1 - t;
	if(t < 0.36363636363636365) {
		return 1 - 7.5625 * t * t;
	} else if(t < 0.72727272727272729) {
		return 1 - (7.5625 * (t -= 0.54545454545454541) * t + 0.75);
	} else if(t < 0.90909090909090906) {
		return 1 - (7.5625 * (t -= 0.81818181818181823) * t + 0.9375);
	} else {
		return 1 - (7.5625 * (t -= 0.95454545454545459) * t + 0.984375);
	}
};
tweenxcore_Easing.bounceOut = function(t) {
	if(t < 0.36363636363636365) {
		return 7.5625 * t * t;
	} else if(t < 0.72727272727272729) {
		return 7.5625 * (t -= 0.54545454545454541) * t + 0.75;
	} else if(t < 0.90909090909090906) {
		return 7.5625 * (t -= 0.81818181818181823) * t + 0.9375;
	} else {
		return 7.5625 * (t -= 0.95454545454545459) * t + 0.984375;
	}
};
tweenxcore_Easing.bounceInOut = function(t) {
	if(t < 0.5) {
		t = 1 - t * 2;
		if(t < 0.36363636363636365) {
			return (1 - 7.5625 * t * t) * 0.5;
		} else if(t < 0.72727272727272729) {
			return (1 - (7.5625 * (t -= 0.54545454545454541) * t + 0.75)) * 0.5;
		} else if(t < 0.90909090909090906) {
			return (1 - (7.5625 * (t -= 0.81818181818181823) * t + 0.9375)) * 0.5;
		} else {
			return (1 - (7.5625 * (t -= 0.95454545454545459) * t + 0.984375)) * 0.5;
		}
	} else {
		t = t * 2 - 1;
		if(t < 0.36363636363636365) {
			return 7.5625 * t * t * 0.5 + 0.5;
		} else if(t < 0.72727272727272729) {
			return (7.5625 * (t -= 0.54545454545454541) * t + 0.75) * 0.5 + 0.5;
		} else if(t < 0.90909090909090906) {
			return (7.5625 * (t -= 0.81818181818181823) * t + 0.9375) * 0.5 + 0.5;
		} else {
			return (7.5625 * (t -= 0.95454545454545459) * t + 0.984375) * 0.5 + 0.5;
		}
	}
};
tweenxcore_Easing.bounceOutIn = function(t) {
	if(t < 0.5) {
		if((t *= 2) < 0.36363636363636365) {
			return 0.5 * (7.5625 * t * t);
		} else if(t < 0.72727272727272729) {
			return 0.5 * (7.5625 * (t -= 0.54545454545454541) * t + 0.75);
		} else if(t < 0.90909090909090906) {
			return 0.5 * (7.5625 * (t -= 0.81818181818181823) * t + 0.9375);
		} else {
			return 0.5 * (7.5625 * (t -= 0.95454545454545459) * t + 0.984375);
		}
	} else {
		t = 1 - (t * 2 - 1);
		if(t < 0.36363636363636365) {
			return 0.5 - 0.5 * (7.5625 * t * t) + 0.5;
		} else if(t < 0.72727272727272729) {
			return 0.5 - 0.5 * (7.5625 * (t -= 0.54545454545454541) * t + 0.75) + 0.5;
		} else if(t < 0.90909090909090906) {
			return 0.5 - 0.5 * (7.5625 * (t -= 0.81818181818181823) * t + 0.9375) + 0.5;
		} else {
			return 0.5 - 0.5 * (7.5625 * (t -= 0.95454545454545459) * t + 0.984375) + 0.5;
		}
	}
};
tweenxcore_Easing.backIn = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		return t * t * (2.70158 * t - 1.70158);
	}
};
tweenxcore_Easing.backOut = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		return --t * t * (2.70158 * t + 1.70158) + 1;
	}
};
tweenxcore_Easing.backInOut = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else if((t *= 2) < 1) {
		return 0.5 * (t * t * (3.5949095 * t - 2.5949095));
	} else {
		return 0.5 * ((t -= 2) * t * (3.5949095 * t + 2.5949095) + 2);
	}
};
tweenxcore_Easing.backOutIn = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else if(t < 0.5) {
		t = t * 2 - 1;
		return 0.5 * (t * t * (2.70158 * t + 1.70158) + 1);
	} else {
		t = t * 2 - 1;
		return 0.5 * t * t * (2.70158 * t - 1.70158) + 0.5;
	}
};
tweenxcore_Easing.elasticIn = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		var s = 7.5e-005;
		return -(Math.exp(6.931471805599453 * --t) * Math.sin((t * 0.001 - s) * 6.2831853071795862 / 0.0003));
	}
};
tweenxcore_Easing.elasticOut = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		var s = 7.5e-005;
		return Math.exp(-6.9314718055994531 * t) * Math.sin((t * 0.001 - s) * 6.2831853071795862 / 0.0003) + 1;
	}
};
tweenxcore_Easing.elasticInOut = function(t) {
	if(t == 0) {
		return 0;
	} else if(t == 1) {
		return 1;
	} else {
		var s = 7.5e-005;
		if((t *= 2) < 1) {
			return -0.5 * (Math.exp(6.931471805599453 * --t) * Math.sin((t * 0.001 - s) * 6.2831853071795862 / 0.0003));
		} else {
			return Math.exp(-6.9314718055994531 * --t) * Math.sin((t * 0.001 - s) * 6.2831853071795862 / 0.0003) * 0.5 + 1;
		}
	}
};
tweenxcore_Easing.elasticOutIn = function(t) {
	if(t < 0.5) {
		if((t *= 2) == 0) {
			return 0;
		} else {
			var s = 7.5e-005;
			return 0.5 * Math.exp(-6.9314718055994531 * t) * Math.sin((t * 0.001 - s) * 6.2831853071795862 / 0.0003) + 0.5;
		}
	} else if(t == 0.5) {
		return 0.5;
	} else if(t == 1) {
		return 1;
	} else {
		t = t * 2 - 1;
		var s = 7.5e-005;
		return -(0.5 * Math.exp(6.931471805599453 * --t) * Math.sin((t * 0.001 - s) * 6.2831853071795862 / 0.0003)) + 0.5;
	}
};
tweenxcore_Easing.warpOut = function(t) {
	if(t <= 0) {
		return 0;
	} else {
		return 1;
	}
};
tweenxcore_Easing.warpIn = function(t) {
	if(t < 1) {
		return 0;
	} else {
		return 1;
	}
};
tweenxcore_Easing.warpInOut = function(t) {
	if(t < 0.5) {
		return 0;
	} else {
		return 1;
	}
};
tweenxcore_Easing.warpOutIn = function(t) {
	if(t <= 0) {
		return 0;
	} else if(t < 1) {
		return 0.5;
	} else {
		return 1;
	}
};
var tweenxcore_FloatTools = function() { };
tweenxcore_FloatTools.__name__ = true;
tweenxcore_FloatTools.revert = function(rate) {
	return 1 - rate;
};
tweenxcore_FloatTools.clamp = function(value,min,max) {
	if(max == null) {
		max = 1.0;
	}
	if(min == null) {
		min = 0.0;
	}
	if(value <= min) {
		return min;
	} else if(max <= value) {
		return max;
	} else {
		return value;
	}
};
tweenxcore_FloatTools.lerp = function(rate,from,to) {
	return from * (1 - rate) + to * rate;
};
tweenxcore_FloatTools.inverseLerp = function(value,from,to) {
	return (value - from) / (to - from);
};
tweenxcore_FloatTools.repeat = function(value,from,to) {
	if(to == null) {
		to = 1.0;
	}
	if(from == null) {
		from = 0.0;
	}
	var p = (value - from) / (to - from);
	return p - Math.floor(p);
};
tweenxcore_FloatTools.shake = function(rate,center,randomFunc) {
	if(center == null) {
		center = 0.0;
	}
	if(randomFunc == null) {
		randomFunc = Math.random;
	}
	var rate1 = randomFunc();
	return center + (-rate * (1 - rate1) + rate * rate1);
};
tweenxcore_FloatTools.spread = function(rate,scale) {
	return -scale * (1 - rate) + scale * rate;
};
tweenxcore_FloatTools.sinByRate = function(rate) {
	return Math.sin(rate * 2 * Math.PI);
};
tweenxcore_FloatTools.cosByRate = function(rate) {
	return Math.cos(rate * 2 * Math.PI);
};
tweenxcore_FloatTools.yoyo = function(rate,easing) {
	return easing((rate < 0.5 ? rate : 1 - rate) * 2);
};
tweenxcore_FloatTools.zigzag = function(rate,easing) {
	if(rate < 0.5) {
		return easing(rate * 2);
	} else {
		return 1 - easing((rate - 0.5) * 2);
	}
};
tweenxcore_FloatTools.mixEasing = function(rate,easing1,easing2,easing2Strength) {
	if(easing2Strength == null) {
		easing2Strength = 0.5;
	}
	return easing1(rate) * (1 - easing2Strength) + easing2(rate) * easing2Strength;
};
tweenxcore_FloatTools.crossfadeEasing = function(rate,easing1,easing2,easing2StrengthEasing,easing2StrengthStart,easing2StrengthEnd) {
	if(easing2StrengthEnd == null) {
		easing2StrengthEnd = 1;
	}
	if(easing2StrengthStart == null) {
		easing2StrengthStart = 0;
	}
	var rate1 = easing2StrengthEasing(rate);
	var rate2 = easing2StrengthStart * (1 - rate1) + easing2StrengthEnd * rate1;
	return easing1(rate) * (1 - rate2) + easing2(rate) * rate2;
};
tweenxcore_FloatTools.connectEasing = function(time,easing1,easing2,switchTime,switchValue) {
	if(switchValue == null) {
		switchValue = 0.5;
	}
	if(switchTime == null) {
		switchTime = 0.5;
	}
	if(time < switchTime) {
		var rate = easing1(time / switchTime);
		return 0 * (1 - rate) + switchValue * rate;
	} else {
		var rate = easing2((time - switchTime) / (1 - switchTime));
		return switchValue * (1 - rate) + rate;
	}
};
tweenxcore_FloatTools.oneTwoEasing = function(time,easingOne,easingTwo,switchTime) {
	if(switchTime == null) {
		switchTime = 0.5;
	}
	if(time < switchTime) {
		return easingOne(time / switchTime);
	} else {
		return easingTwo((time - switchTime) / (1 - switchTime));
	}
};
tweenxcore_FloatTools.binarySearch = function(sortedValues,value,boundaryMode) {
	if(boundaryMode == null) {
		boundaryMode = 0;
	}
	var min = 0;
	var max = sortedValues.length;
	if(boundaryMode == 0) {
		while(true) {
			var next = ((max - min) / 2 | 0) + min;
			var dv = sortedValues[next];
			if(dv <= value) {
				min = next + 1;
			} else {
				max = next;
			}
			if(min == max) {
				break;
			}
		}
	} else {
		while(true) {
			var next = ((max - min) / 2 | 0) + min;
			var dv = sortedValues[next];
			if(dv < value) {
				min = next + 1;
			} else {
				max = next;
			}
			if(min == max) {
				break;
			}
		}
	}
	return min;
};
tweenxcore_FloatTools.polyline = function(rate,values) {
	if(values.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else {
		var max = values.length - 1;
		var scaledRate = rate * max;
		var min = 0;
		var max1 = max - 1;
		if(max1 == null) {
			max1 = 1.0;
		}
		if(min == null) {
			min = 0.0;
		}
		var index = Math.floor(scaledRate <= min ? min : max1 <= scaledRate ? max1 : scaledRate);
		var innerRate = scaledRate - index;
		return values[index] * (1 - innerRate) + values[index + 1] * innerRate;
	}
};
tweenxcore_FloatTools.bezier2 = function(rate,from,control,to) {
	return (from * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + to * rate) * rate;
};
tweenxcore_FloatTools.bezier3 = function(rate,from,control1,control2,to) {
	var control = control1 * (1 - rate) + control2 * rate;
	return ((from * (1 - rate) + control1 * rate) * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + (control2 * (1 - rate) + to * rate) * rate) * rate;
};
tweenxcore_FloatTools.bezier = function(rate,values) {
	if(values.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else if(values.length == 2) {
		return values[0] * (1 - rate) + values[1] * rate;
	} else if(values.length == 3) {
		var control = values[1];
		return (values[0] * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + values[2] * rate) * rate;
	} else {
		return tweenxcore_FloatTools._bezier(rate,values);
	}
};
tweenxcore_FloatTools._bezier = function(rate,values) {
	if(values.length == 4) {
		var control1 = values[1];
		var control2 = values[2];
		var control = control1 * (1 - rate) + control2 * rate;
		return ((values[0] * (1 - rate) + control1 * rate) * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + (control2 * (1 - rate) + values[3] * rate) * rate) * rate;
	}
	var _g = [];
	var _g1 = 0;
	var _g2 = values.length - 1;
	while(_g1 < _g2) {
		var i = _g1++;
		_g.push(values[i] * (1 - rate) + values[i + 1] * rate);
	}
	return tweenxcore_FloatTools._bezier(rate,_g);
};
tweenxcore_FloatTools.uniformQuadraticBSpline = function(rate,values) {
	if(values.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else if(values.length == 2) {
		return values[0] * (1 - rate) + values[1] * rate;
	} else {
		var max = values.length - 2;
		var scaledRate = rate * max;
		var min = 0;
		var max1 = max - 1;
		if(max1 == null) {
			max1 = 1.0;
		}
		if(min == null) {
			min = 0.0;
		}
		var index = Math.floor(scaledRate <= min ? min : max1 <= scaledRate ? max1 : scaledRate);
		var innerRate = scaledRate - index;
		var p0 = values[index];
		var p1 = values[index + 1];
		var p2 = values[index + 2];
		return innerRate * innerRate * (p0 / 2 - p1 + p2 / 2) + innerRate * (-p0 + p1) + p0 / 2 + p1 / 2;
	}
};
tweenxcore_FloatTools.frameToSecond = function(frame,fps) {
	return frame / fps;
};
tweenxcore_FloatTools.secondToFrame = function(second,fps) {
	return second * fps;
};
tweenxcore_FloatTools.degreeToRate = function(degree) {
	return degree / 360;
};
tweenxcore_FloatTools.rateToDegree = function(rate) {
	return rate * 360;
};
tweenxcore_FloatTools.radianToRate = function(radian) {
	return radian / (2 * Math.PI);
};
tweenxcore_FloatTools.rateToRadian = function(rate) {
	return rate * 2 * Math.PI;
};
tweenxcore_FloatTools.millisecondToBeat = function(millisecond,bpm) {
	return millisecond * bpm / 60000;
};
tweenxcore_FloatTools.beatToMillisecond = function(beat,bpm) {
	return beat * 60000 / bpm;
};
var tweenxcore_PointTools = function() { };
tweenxcore_PointTools.__name__ = true;
tweenxcore_PointTools.polyline = function(outputPoint,rate,points) {
	var xs = [];
	var ys = [];
	var p = $getIterator(points);
	while(p.hasNext()) {
		var p1 = p.next();
		xs.push(p1.x);
		ys.push(p1.y);
	}
	var tmp;
	if(xs.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else {
		var max = xs.length - 1;
		var scaledRate = rate * max;
		var min = 0;
		var max1 = max - 1;
		if(max1 == null) {
			max1 = 1.0;
		}
		if(min == null) {
			min = 0.0;
		}
		var index = Math.floor(scaledRate <= min ? min : max1 <= scaledRate ? max1 : scaledRate);
		var innerRate = scaledRate - index;
		tmp = xs[index] * (1 - innerRate) + xs[index + 1] * innerRate;
	}
	outputPoint.x = tmp;
	var tmp;
	if(ys.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else {
		var max = ys.length - 1;
		var scaledRate = rate * max;
		var min = 0;
		var max1 = max - 1;
		if(max1 == null) {
			max1 = 1.0;
		}
		if(min == null) {
			min = 0.0;
		}
		var index = Math.floor(scaledRate <= min ? min : max1 <= scaledRate ? max1 : scaledRate);
		var innerRate = scaledRate - index;
		tmp = ys[index] * (1 - innerRate) + ys[index + 1] * innerRate;
	}
	outputPoint.y = tmp;
};
tweenxcore_PointTools.bezier2 = function(outputPoint,rate,from,control,to) {
	var control1 = control.x;
	outputPoint.x = (from.x * (1 - rate) + control1 * rate) * (1 - rate) + (control1 * (1 - rate) + from.x * rate) * rate;
	var control1 = control.y;
	outputPoint.y = (from.y * (1 - rate) + control1 * rate) * (1 - rate) + (control1 * (1 - rate) + from.y * rate) * rate;
};
tweenxcore_PointTools.bezier3 = function(outputPoint,rate,from,control1,control2,to) {
	var control11 = control1.x;
	var control21 = control2.x;
	var control = control11 * (1 - rate) + control21 * rate;
	outputPoint.x = ((from.x * (1 - rate) + control11 * rate) * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + (control21 * (1 - rate) + from.x * rate) * rate) * rate;
	var control11 = control1.y;
	var control21 = control2.y;
	var control = control11 * (1 - rate) + control21 * rate;
	outputPoint.y = ((from.y * (1 - rate) + control11 * rate) * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + (control21 * (1 - rate) + from.y * rate) * rate) * rate;
};
tweenxcore_PointTools.bezier = function(outputPoint,rate,points) {
	var xs = [];
	var ys = [];
	var p = $getIterator(points);
	while(p.hasNext()) {
		var p1 = p.next();
		xs.push(p1.x);
		ys.push(p1.y);
	}
	var tmp;
	if(xs.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else if(xs.length == 2) {
		tmp = xs[0] * (1 - rate) + xs[1] * rate;
	} else if(xs.length == 3) {
		var control = xs[1];
		tmp = (xs[0] * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + xs[2] * rate) * rate;
	} else {
		tmp = tweenxcore_FloatTools._bezier(rate,xs);
	}
	outputPoint.x = tmp;
	var tmp;
	if(ys.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else if(ys.length == 2) {
		tmp = ys[0] * (1 - rate) + ys[1] * rate;
	} else if(ys.length == 3) {
		var control = ys[1];
		tmp = (ys[0] * (1 - rate) + control * rate) * (1 - rate) + (control * (1 - rate) + ys[2] * rate) * rate;
	} else {
		tmp = tweenxcore_FloatTools._bezier(rate,ys);
	}
	outputPoint.y = tmp;
};
tweenxcore_PointTools.uniformQuadraticBSpline = function(outputPoint,rate,points) {
	var xs = [];
	var ys = [];
	var p = $getIterator(points);
	while(p.hasNext()) {
		var p1 = p.next();
		xs.push(p1.x);
		ys.push(p1.y);
	}
	var tmp;
	if(xs.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else if(xs.length == 2) {
		tmp = xs[0] * (1 - rate) + xs[1] * rate;
	} else {
		var max = xs.length - 2;
		var scaledRate = rate * max;
		var min = 0;
		var max1 = max - 1;
		if(max1 == null) {
			max1 = 1.0;
		}
		if(min == null) {
			min = 0.0;
		}
		var index = Math.floor(scaledRate <= min ? min : max1 <= scaledRate ? max1 : scaledRate);
		var innerRate = scaledRate - index;
		var p0 = xs[index];
		var p1 = xs[index + 1];
		var p2 = xs[index + 2];
		tmp = innerRate * innerRate * (p0 / 2 - p1 + p2 / 2) + innerRate * (-p0 + p1) + p0 / 2 + p1 / 2;
	}
	outputPoint.x = tmp;
	var tmp;
	if(ys.length < 2) {
		throw haxe_Exception.thrown("points length must be more than 2");
	} else if(ys.length == 2) {
		tmp = ys[0] * (1 - rate) + ys[1] * rate;
	} else {
		var max = ys.length - 2;
		var scaledRate = rate * max;
		var min = 0;
		var max1 = max - 1;
		if(max1 == null) {
			max1 = 1.0;
		}
		if(min == null) {
			min = 0.0;
		}
		var index = Math.floor(scaledRate <= min ? min : max1 <= scaledRate ? max1 : scaledRate);
		var innerRate = scaledRate - index;
		var p0 = ys[index];
		var p1 = ys[index + 1];
		var p2 = ys[index + 2];
		tmp = innerRate * innerRate * (p0 / 2 - p1 + p2 / 2) + innerRate * (-p0 + p1) + p0 / 2 + p1 / 2;
	}
	outputPoint.y = tmp;
};
var tweenxcore_MatrixTools = function() { };
tweenxcore_MatrixTools.__name__ = true;
tweenxcore_MatrixTools.createSimilarityTransform = function(outputMatrix,fromX,fromY,toX,toY) {
	var dx = toX - fromX;
	var dy = toY - fromY;
	var rot = Math.atan2(dy,dx);
	var d = Math.sqrt(dx * dx + dy * dy);
	outputMatrix.a = d * Math.cos(rot);
	outputMatrix.b = d * Math.sin(rot);
	outputMatrix.c = -d * Math.sin(rot);
	outputMatrix.d = d * Math.cos(rot);
	outputMatrix.tx = fromX;
	outputMatrix.ty = fromY;
};
var tweenxcore_color_HsvColor = function(hue,saturation,value) {
	this.h = hue;
	this.s = saturation;
	this.v = value;
};
tweenxcore_color_HsvColor.__name__ = true;
tweenxcore_color_HsvColor.hsvToRgbInt = function(h,s,v) {
	h = (h - Math.floor(h)) * 6;
	var hi = Math.floor(h);
	if(s <= 0.0) {
		s = 0.0;
	} else if(1.0 <= s) {
		s = 1.0;
	}
	if(v <= 0.0) {
		v = 0.0;
	} else if(1.0 <= v) {
		v = 1.0;
	}
	var m = v * (1 - s);
	var f = h - hi;
	var r = 0.0;
	var g = 0.0;
	var b = 0.0;
	switch(hi) {
	case 0:
		r = v;
		g = v * (1 - s * (1 - f));
		b = m;
		break;
	case 1:
		r = v * (1 - s * f);
		g = v;
		b = m;
		break;
	case 2:
		r = m;
		g = v;
		b = v * (1 - s * (1 - f));
		break;
	case 3:
		r = m;
		g = v * (1 - s * f);
		b = v;
		break;
	case 4:
		r = v * (1 - s * (1 - f));
		g = m;
		b = v;
		break;
	case 5:
		r = v;
		g = m;
		b = v * (1 - s * f);
		break;
	}
	return (r * 255 | 0) << 16 | (g * 255 | 0) << 8 | (b * 255 | 0);
};
tweenxcore_color_HsvColor.of = function(color,hueIndex) {
	if(hueIndex == null) {
		hueIndex = 0;
	}
	var r = (color >> 16 & 255) / 255;
	var g = (color >> 8 & 255) / 255;
	var b = (color & 255) / 255;
	return tweenxcore_color_HsvColor.fromRgb(r,g,b,hueIndex);
};
tweenxcore_color_HsvColor.fromRgb = function(r,g,b,hueIndex) {
	if(hueIndex == null) {
		hueIndex = 0;
	}
	var max;
	var min;
	var diff;
	var h;
	if(r < g) {
		if(g < b) {
			max = b;
			min = r;
			diff = max - min;
			h = (4 + (r - g) / diff) / 6;
		} else {
			max = g;
			min = r < b ? r : b;
			diff = max - min;
			h = (2 + (b - r) / diff) / 6;
		}
	} else if(r < b) {
		max = b;
		min = g;
		diff = max - min;
		h = (4 + (r - g) / diff) / 6;
	} else {
		max = r;
		min = g < b ? g : b;
		diff = max - min;
		h = (g - b) / diff / 6;
	}
	if(h < 0) {
		++h;
	}
	var s = diff / max;
	if(isNaN(h)) {
		h = 0;
	}
	if(isNaN(s)) {
		s = 0;
	}
	return new tweenxcore_color_HsvColor(h + hueIndex,s,max);
};
tweenxcore_color_HsvColor.prototype = {
	getRed: function() {
		return tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v).r;
	}
	,getGreen: function() {
		return tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v).g;
	}
	,getBlue: function() {
		return tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v).b;
	}
	,getHue: function() {
		return this.h;
	}
	,getSaturation: function() {
		return this.s;
	}
	,getBrightness: function() {
		return this.v;
	}
	,toRgb: function() {
		return tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v);
	}
	,toHsv: function() {
		return new tweenxcore_color_HsvColor(this.h,this.s,this.v);
	}
	,toHsvWithAlpha: function(alpha) {
		return new tweenxcore_color_AhsvColor(alpha,this.h,this.s,this.v);
	}
	,toRgbWithAlpha: function(alpha) {
		var _this = tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v);
		return new tweenxcore_color_ArgbColor(alpha,_this.r,_this.g,_this.b);
	}
	,toRgbInt: function() {
		return tweenxcore_color_HsvColor.hsvToRgbInt(this.h,this.s,this.v);
	}
	,toRgbHexString: function() {
		return StringTools.hex(tweenxcore_color_HsvColor.hsvToRgbInt(this.h,this.s,this.v),6);
	}
	,toRgbCssString: function() {
		var _this = tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v);
		return "rgb(" + (_this.r * 255 | 0) + "," + (_this.g * 255 | 0) + "," + (_this.b * 255 | 0) + ")";
	}
};
var tweenxcore_color_AhsvColor = function(alpha,hue,saturation,value) {
	this.a = alpha;
	tweenxcore_color_HsvColor.call(this,hue,saturation,value);
};
tweenxcore_color_AhsvColor.__name__ = true;
tweenxcore_color_AhsvColor.ahsvToArgbInt = function(a,h,s,v) {
	return ((a <= 0.0 ? 0.0 : 1.0 <= a ? 1.0 : a) * 255 | 0) << 24 | tweenxcore_color_HsvColor.hsvToRgbInt(h,s,v);
};
tweenxcore_color_AhsvColor.of = function(color,hueIndex) {
	if(hueIndex == null) {
		hueIndex = 0;
	}
	var a = (color >>> 24 & 255) / 255;
	var color1 = color & 16777215;
	var hueIndex1 = hueIndex;
	if(hueIndex1 == null) {
		hueIndex1 = 0;
	}
	var r = (color1 >> 16 & 255) / 255;
	var g = (color1 >> 8 & 255) / 255;
	var b = (color1 & 255) / 255;
	var _this = tweenxcore_color_HsvColor.fromRgb(r,g,b,hueIndex1);
	return new tweenxcore_color_AhsvColor(a,_this.h,_this.s,_this.v);
};
tweenxcore_color_AhsvColor.fromArgb = function(a,r,g,b,hueIndex) {
	if(hueIndex == null) {
		hueIndex = 0;
	}
	var _this = tweenxcore_color_HsvColor.fromRgb(r,g,b,hueIndex);
	return new tweenxcore_color_AhsvColor(a,_this.h,_this.s,_this.v);
};
tweenxcore_color_AhsvColor.__super__ = tweenxcore_color_HsvColor;
tweenxcore_color_AhsvColor.prototype = $extend(tweenxcore_color_HsvColor.prototype,{
	getAlpha: function() {
		return this.a;
	}
	,toArgb: function() {
		var a = this.a;
		var _this = tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v);
		return new tweenxcore_color_ArgbColor(a,_this.r,_this.g,_this.b);
	}
	,toAhsv: function() {
		return new tweenxcore_color_AhsvColor(this.a,this.h,this.s,this.v);
	}
	,toArgbInt: function() {
		var a = this.a;
		return ((a <= 0.0 ? 0.0 : 1.0 <= a ? 1.0 : a) * 255 | 0) << 24 | tweenxcore_color_HsvColor.hsvToRgbInt(this.h,this.s,this.v);
	}
	,toArgbHexString: function() {
		var a = this.a;
		return StringTools.hex(((a <= 0.0 ? 0.0 : 1.0 <= a ? 1.0 : a) * 255 | 0) << 24 | tweenxcore_color_HsvColor.hsvToRgbInt(this.h,this.s,this.v),8);
	}
	,toRgbaCssString: function() {
		var a = this.a;
		var _this = tweenxcore_color_RgbColor.fromHsv(this.h,this.s,this.v);
		var _this1 = new tweenxcore_color_ArgbColor(a,_this.r,_this.g,_this.b);
		return "rgba(" + (_this1.r * 255 | 0) + "," + (_this1.g * 255 | 0) + "," + (_this1.b * 255 | 0) + "," + _this1.a + ")";
	}
});
var tweenxcore_color_RgbColor = function(red,green,blue) {
	this.r = red;
	this.g = green;
	this.b = blue;
};
tweenxcore_color_RgbColor.__name__ = true;
tweenxcore_color_RgbColor.rgbToInt = function(r,g,b) {
	if(r <= 0.0) {
		r = 0.0;
	} else if(1.0 <= r) {
		r = 1.0;
	}
	if(g <= 0.0) {
		g = 0.0;
	} else if(1.0 <= g) {
		g = 1.0;
	}
	if(b <= 0.0) {
		b = 0.0;
	} else if(1.0 <= b) {
		b = 1.0;
	}
	return (r * 255 | 0) << 16 | (g * 255 | 0) << 8 | (b * 255 | 0);
};
tweenxcore_color_RgbColor.of = function(color) {
	return new tweenxcore_color_RgbColor((color >> 16 & 255) / 255,(color >> 8 & 255) / 255,(color & 255) / 255);
};
tweenxcore_color_RgbColor.fromHsv = function(h,s,v) {
	h = (h - Math.floor(h)) * 6;
	var hi = Math.floor(h);
	if(s <= 0.0) {
		s = 0.0;
	} else if(1.0 <= s) {
		s = 1.0;
	}
	if(v <= 0.0) {
		v = 0.0;
	} else if(1.0 <= v) {
		v = 1.0;
	}
	var m = v * (1 - s);
	var f = h - hi;
	var r = 0.0;
	var g = 0.0;
	var b = 0.0;
	switch(hi) {
	case 0:
		r = v;
		g = v * (1 - s * (1 - f));
		b = m;
		break;
	case 1:
		r = v * (1 - s * f);
		g = v;
		b = m;
		break;
	case 2:
		r = m;
		g = v;
		b = v * (1 - s * (1 - f));
		break;
	case 3:
		r = m;
		g = v * (1 - s * f);
		b = v;
		break;
	case 4:
		r = v * (1 - s * (1 - f));
		g = m;
		b = v;
		break;
	case 5:
		r = v;
		g = m;
		b = v * (1 - s * f);
		break;
	}
	return new tweenxcore_color_RgbColor(r,g,b);
};
tweenxcore_color_RgbColor.prototype = {
	getRed: function() {
		return this.r;
	}
	,getGreen: function() {
		return this.g;
	}
	,getBlue: function() {
		return this.b;
	}
	,getHue: function() {
		return tweenxcore_color_HsvColor.fromRgb(this.r,this.g,this.b).h;
	}
	,getSaturation: function() {
		return tweenxcore_color_HsvColor.fromRgb(this.r,this.g,this.b).s;
	}
	,getBrightness: function() {
		return tweenxcore_color_HsvColor.fromRgb(this.r,this.g,this.b).v;
	}
	,toRgb: function() {
		return new tweenxcore_color_RgbColor(this.r,this.g,this.b);
	}
	,toHsv: function() {
		return tweenxcore_color_HsvColor.fromRgb(this.r,this.g,this.b);
	}
	,toRgbWithAlpha: function(alpha) {
		return new tweenxcore_color_ArgbColor(alpha,this.r,this.g,this.b);
	}
	,toHsvWithAlpha: function(alpha) {
		var _this = tweenxcore_color_HsvColor.fromRgb(this.r,this.g,this.b,0);
		return new tweenxcore_color_AhsvColor(alpha,_this.h,_this.s,_this.v);
	}
	,toRgbInt: function() {
		var r = this.r;
		var g = this.g;
		var b = this.b;
		if(r <= 0.0) {
			r = 0.0;
		} else if(1.0 <= r) {
			r = 1.0;
		}
		if(g <= 0.0) {
			g = 0.0;
		} else if(1.0 <= g) {
			g = 1.0;
		}
		if(b <= 0.0) {
			b = 0.0;
		} else if(1.0 <= b) {
			b = 1.0;
		}
		return (r * 255 | 0) << 16 | (g * 255 | 0) << 8 | (b * 255 | 0);
	}
	,toRgbHexString: function() {
		var r = this.r;
		var g = this.g;
		var b = this.b;
		if(r <= 0.0) {
			r = 0.0;
		} else if(1.0 <= r) {
			r = 1.0;
		}
		if(g <= 0.0) {
			g = 0.0;
		} else if(1.0 <= g) {
			g = 1.0;
		}
		if(b <= 0.0) {
			b = 0.0;
		} else if(1.0 <= b) {
			b = 1.0;
		}
		return StringTools.hex((r * 255 | 0) << 16 | (g * 255 | 0) << 8 | (b * 255 | 0),6);
	}
	,toRgbCssString: function() {
		return "rgb(" + (this.r * 255 | 0) + "," + (this.g * 255 | 0) + "," + (this.b * 255 | 0) + ")";
	}
};
var tweenxcore_color_ArgbColor = function(alpha,red,green,blue) {
	this.a = alpha;
	tweenxcore_color_RgbColor.call(this,red,green,blue);
};
tweenxcore_color_ArgbColor.__name__ = true;
tweenxcore_color_ArgbColor.argbToInt = function(a,r,g,b) {
	var r1 = r;
	var g1 = g;
	var b1 = b;
	if(r1 <= 0.0) {
		r1 = 0.0;
	} else if(1.0 <= r1) {
		r1 = 1.0;
	}
	if(g1 <= 0.0) {
		g1 = 0.0;
	} else if(1.0 <= g1) {
		g1 = 1.0;
	}
	if(b1 <= 0.0) {
		b1 = 0.0;
	} else if(1.0 <= b1) {
		b1 = 1.0;
	}
	return ((a <= 0.0 ? 0.0 : 1.0 <= a ? 1.0 : a) * 255 | 0) << 24 | ((r1 * 255 | 0) << 16 | (g1 * 255 | 0) << 8 | (b1 * 255 | 0));
};
tweenxcore_color_ArgbColor.of = function(color) {
	return new tweenxcore_color_ArgbColor((color >>> 24 & 255) / 255,(color >> 16 & 255) / 255,(color >> 8 & 255) / 255,(color & 255) / 255);
};
tweenxcore_color_ArgbColor.fromAhsv = function(a,h,s,v,hueIndex) {
	if(hueIndex == null) {
		hueIndex = 0;
	}
	var _this = tweenxcore_color_RgbColor.fromHsv(h,s,v);
	return new tweenxcore_color_ArgbColor(a,_this.r,_this.g,_this.b);
};
tweenxcore_color_ArgbColor.__super__ = tweenxcore_color_RgbColor;
tweenxcore_color_ArgbColor.prototype = $extend(tweenxcore_color_RgbColor.prototype,{
	getAlpha: function() {
		return this.a;
	}
	,toArgb: function() {
		return new tweenxcore_color_ArgbColor(this.a,this.r,this.g,this.b);
	}
	,toAhsv: function() {
		var a = this.a;
		var _this = tweenxcore_color_HsvColor.fromRgb(this.r,this.g,this.b,0);
		return new tweenxcore_color_AhsvColor(a,_this.h,_this.s,_this.v);
	}
	,toArgbInt: function() {
		var a = this.a;
		var r = this.r;
		var g = this.g;
		var b = this.b;
		if(r <= 0.0) {
			r = 0.0;
		} else if(1.0 <= r) {
			r = 1.0;
		}
		if(g <= 0.0) {
			g = 0.0;
		} else if(1.0 <= g) {
			g = 1.0;
		}
		if(b <= 0.0) {
			b = 0.0;
		} else if(1.0 <= b) {
			b = 1.0;
		}
		return ((a <= 0.0 ? 0.0 : 1.0 <= a ? 1.0 : a) * 255 | 0) << 24 | ((r * 255 | 0) << 16 | (g * 255 | 0) << 8 | (b * 255 | 0));
	}
	,toArgbHexString: function() {
		var a = this.a;
		var r = this.r;
		var g = this.g;
		var b = this.b;
		if(r <= 0.0) {
			r = 0.0;
		} else if(1.0 <= r) {
			r = 1.0;
		}
		if(g <= 0.0) {
			g = 0.0;
		} else if(1.0 <= g) {
			g = 1.0;
		}
		if(b <= 0.0) {
			b = 0.0;
		} else if(1.0 <= b) {
			b = 1.0;
		}
		return StringTools.hex(((a <= 0.0 ? 0.0 : 1.0 <= a ? 1.0 : a) * 255 | 0) << 24 | ((r * 255 | 0) << 16 | (g * 255 | 0) << 8 | (b * 255 | 0)),8);
	}
	,toRgbaCssString: function() {
		return "rgba(" + (this.r * 255 | 0) + "," + (this.g * 255 | 0) + "," + (this.b * 255 | 0) + "," + this.a + ")";
	}
});
function $getIterator(o) { if( o instanceof Array ) return new haxe_iterators_ArrayIterator(o); else return o.iterator(); }
function $bind(o,m) { if( m == null ) return null; if( m.__id__ == null ) m.__id__ = $global.$haxeUID++; var f; if( o.hx__closures__ == null ) o.hx__closures__ = {}; else f = o.hx__closures__[m.__id__]; if( f == null ) { f = m.bind(o); o.hx__closures__[m.__id__] = f; } return f; }
$global.$haxeUID |= 0;
if(typeof(performance) != "undefined" ? typeof(performance.now) == "function" : false) {
	HxOverrides.now = performance.now.bind(performance);
}
if( String.fromCodePoint == null ) String.fromCodePoint = function(c) { return c < 0x10000 ? String.fromCharCode(c) : String.fromCharCode((c>>10)+0xD7C0)+String.fromCharCode((c&0x3FF)+0xDC00); }
String.__name__ = true;
Array.__name__ = true;
js_Boot.__toStr = ({ }).toString;
Main.history = [];
State.NL = new EReg("\r\n|\r|\n","g");
State.S = new EReg("\\s","g");
tweenxcore_Easing.PI = 3.1415926535897932384626433832795;
tweenxcore_Easing.PI_H = 1.5707963267948966;
tweenxcore_Easing.LN_2 = 0.6931471805599453;
tweenxcore_Easing.LN_2_10 = 6.931471805599453;
tweenxcore_Easing.overshoot = 1.70158;
tweenxcore_Easing.amplitude = 1;
tweenxcore_Easing.period = 0.0003;
Main.main();
})(typeof window != "undefined" ? window : typeof global != "undefined" ? global : typeof self != "undefined" ? self : this);
