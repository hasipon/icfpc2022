import {Interpreter} from "./Interpreter";
import * as fs from 'fs';


const code= "cut [0] [x] [204]\n" +
  "merge [0.1] [0.0]\n" +
  "\n" +
  "\n" +
  "\n" +
  "color [1] [148, 50, 190, 129]\n" +
  "color [1] [9, 153, 227, 183]"

const interpreter = new Interpreter();
const input = fs.readFileSync("/dev/stdin", "utf8");
const result = interpreter.run(input);
console.log({"score": result.cost});
