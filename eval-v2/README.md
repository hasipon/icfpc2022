# eval-v2

## install

node, npmとかでいい感じにする

```
nvm install node
npm install 
```


## How to run

````
> ISL_FILE=in.isl PROBLEM_ID=1 OUT_IMAGE_PATH=1.png npx ts-node index.ts
...
cost:   233     cut [0.1.1.1.1.0] [x] [280]
cost:   350     cut [0.1.1.1.1.0.0] [x] [240]
cost:   500     color [0.1.1.1.1.0.0.0] [13, 14, 15, 255]
cost:   500     color [0.1.1.1.1.0.0.1] [236, 237, 238, 255]
cost:   500     color [0.1.1.1.1.0.1] [20, 20, 21, 255]
cost:   350     cut [0.1.1.1.1.1] [x] [360]
cost:   500     color [0.1.1.1.1.1.0] [233, 239, 247, 255]
cost:   500     color [0.1.1.1.1.1.1] [0, 74, 173, 255]
{ cost: 72381, similarity: 35196, sum: 107577 }
````
