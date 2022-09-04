import {getColored, Interpreter, InterpreterResult} from "./Interpreter";
import {Frame, Painter} from "./Painter";

import * as fs from 'fs';
import {SimilarityChecker} from "./SimilarityChecker";
import * as os from "os";
import {createImageData} from "canvas";
import * as buffer from "buffer";
import {Block, SimpleBlock} from "./Block";
import {Point} from "./Point";
import {RGBA} from "./Color";
import {InstructionCostCalculator} from "./InstructionCostCalculator";
import {InstructionType} from "./Instruction";

function readProblemImage(problemId: string): Frame{
  const fileContent = fs.readFileSync(`../problems.json/${problemId}.json`, 'utf8');

  // for pam file
  // const data: [number, number, number, number][] = new Array(400*400);
  // for(var i = 0; i <input.length; i+=4){
  //   const r = input.readUInt8(i);
  //   const g = input.readUInt8(i+1);
  //   const b = input.readUInt8(i+2);
  //   const a = input.readUInt8(i+3);
  //   const id = i / 4;
  //   data[id] = [r, g, b, a];
  // }
  return SimilarityChecker.dataToFrame(JSON.parse(fileContent));
}
function createImage(frames: Frame, outputFile: string){
  const { createCanvas, loadImage } = require('canvas')
  const canvas = createCanvas(400, 400)
  const ctx = canvas.getContext('2d')
  const arr = new Uint8ClampedArray(frames.length * 4);
  for(var i = 0; i<400*400; i++ ){
    arr[i*4] = frames[i].r;
    arr[i*4+1] = frames[i].g;
    arr[i*4+2] = frames[i].b;
    arr[i*4+3] = frames[i].a;
  }
  const imageData = createImageData(arr, 400, 400);
  ctx.putImageData(imageData, 0, 0);
  const buffer = canvas.toBuffer('image/png')
  fs.writeFileSync(outputFile, buffer)
}

function main(){
  if(!process.env?.ISL_FILE || !process.env?.PROBLEM_ID){
    console.log("Usage: ISL_FILE=<ISL_FILE> PROBLEM_ID=<PROBLEM_ID> OUT_IMAGE_PATH=<PATH>.png ts-node index.ts\n")
    process.exit(1);
  }

  const interpreter = new Interpreter();
  const islFileName = process.env.ISL_FILE!.toString();
  const input = fs.readFileSync(islFileName, "utf8");

  var blocks: Map<string, Block> = new Map();
  const problemId = process.env.PROBLEM_ID!.toString();

  if(Number(problemId) >= 26) {
    const input = fs.readFileSync(`../problems/${problemId}.initial.json`, 'utf8');
    const parsedInput =JSON.parse(input);
    blocks = new Map();

    parsedInput["blocks"].forEach((b: any) => {
      const id = b["blockId"];
      const bottomLeft = new Point(b["bottomLeft"]);
      const topRight = new Point(b["topRight"]);
      const color = new RGBA(b["color"]);
      blocks.set(b["blockId"], new SimpleBlock(
        id,
        bottomLeft,
        topRight,
        color,
      ));
    })
  }
  if(Number(problemId) >= 36) {
    InstructionCostCalculator.baseCostMap.set(InstructionType.VerticalCutInstructionType, 2);
    InstructionCostCalculator.baseCostMap.set(InstructionType.HorizontalCutInstructionType, 2);
    InstructionCostCalculator.baseCostMap.set(InstructionType.PointCutInstructionType, 3);
  }

  const result = interpreter.run(input, blocks);

  const painter = new Painter();
  const frames = painter.draw(result.canvas);

  if(process.env?.OUT_IMAGE_PATH){
    createImage(frames, process.env?.OUT_IMAGE_PATH);
  }
  const diff = SimilarityChecker.imageDiff(frames, readProblemImage(problemId));
  console.log(JSON.stringify({
    "cost": result.cost + diff,
    "isl_cost": result.cost,
    "sim_cost": diff,
  }));

  if(process.env?.BLOCK_ID_INFO){
    const ids = new Array(result.canvas.width * result.canvas.height);
    const coloered = getColored();
    result.canvas.blocks.forEach(block => {
      const frameTopLeft = new Point([block.bottomLeft.px, result.canvas.height - block.topRight.py]);
      const frameBottomRight = new Point([block.topRight.px, result.canvas.height - block.bottomLeft.py]);
      var id = block.id;
      while(!coloered.has(id)){
        if(id == ""){
          return ;
        }
        const ids = id.split(".");
        ids.pop();
        id = ids.join(".")
      }
      for(let y = frameTopLeft.py ; y < frameBottomRight.py ; y++) {
        for(let x = frameTopLeft.px; x < frameBottomRight.px ; x++) {
          ids[y * result.canvas.width + x] = id;
        }
      }
    })
    const idsStr = ids.join("\n");
    fs.writeFileSync(process.env.BLOCK_ID_INFO.toString(), idsStr);
  }

}

main();
