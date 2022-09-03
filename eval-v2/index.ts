import {Interpreter} from "./Interpreter";
import {Frame, Painter} from "./Painter";

import * as fs from 'fs';
import {SimilarityChecker} from "./SimilarityChecker";
import * as os from "os";

function raedProblemImage(): Frame{
  const problemId = process.env.PROBLEM_ID!.toString();
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

if(!process.env?.ISL_FILE || !process.env?.PROBLEM_ID){
  console.log("Usage: ISL_FILE=<ISL_FILE> PROBLEM_ID=<PROBLEM_ID> ts-node index.ts\n")
  process.exit(1);
}

const interpreter = new Interpreter();
const islFileName = process.env.ISL_FILE!.toString();
const input = fs.readFileSync(islFileName, "utf8");
const result = interpreter.run(input);

const painter = new Painter();

const diff = SimilarityChecker.imageDiff(painter.draw(result.canvas), raedProblemImage());
console.log(JSON.stringify({
  "cost": result.cost + diff,
  "isl_cost": result.cost,
  "sim_cost": diff,
}));
