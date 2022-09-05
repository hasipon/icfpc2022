
set -eux
g++ -std=gnu++17 -Wall -Wextra -O2 main.cpp
for i in {1..40} ; do
  export PROBLEM_ID=$i
  PROBLEM_ID=${PROBLEM_ID}
  cd ../../eval-v2
  BLOCK_ID_INFO=../ai/shiota5/info ISL_FILE=../solutions.best/${PROBLEM_ID}.isl npx ts-node index.ts
  cd -
  BLOCK_ID_INFO=info MERGE_ISL=../../solutions.best/${PROBLEM_ID}.isl ./a.out > ../../solutions/${PROBLEM_ID}-shiota5-`date +%s`.isl
done
