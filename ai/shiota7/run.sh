#!/usr/bin/env bash

cd $(dirname $0)

g++ -O2 -std=gnu++17 main.cpp

export LOOP=7
LOOP=7

PROBLEM_ID=26
PROBLEM_ID=26 MERGE_ISL=../../solutions.best/5.isl ./a.out > ../../solutions/$PROBLEM_ID-shiota7$LOOP-`date +%s`.isl
PROBLEM_ID=27
PROBLEM_ID=27 MERGE_ISL=../../solutions.best/2.isl ./a.out > ../../solutions/$PROBLEM_ID-shiota7$LOOP-`date +%s`.isl
PROBLEM_ID=28
PROBLEM_ID=28 MERGE_ISL=../../solutions.best/10.isl ./a.out > ../../solutions/$PROBLEM_ID-shiota7$LOOP-`date +%s`.isl
PROBLEM_ID=29
PROBLEM_ID=29 MERGE_ISL=../../solutions.best/18.isl ./a.out > ../../solutions/$PROBLEM_ID-shiota7$LOOP-`date +%s`.isl
PROBLEM_ID=30
PROBLEM_ID=30 MERGE_ISL=../../solutions.best/11.isl ./a.out > ../../solutions/$PROBLEM_ID-shiota7$LOOP-`date +%s`.isl
PROBLEM_ID=31
PROBLEM_ID=31 MERGE_ISL=../../solutions.best/24.isl ./a.out > ../../solutions/$PROBLEM_ID-shiota7$LOOP-`date +%s`.isl
PROBLEM_ID=32
PROBLEM_ID=32 MERGE_ISL=../../solutions.best/9.isl ./a.out > ../../solutions/$PROBLEM_ID-shiota7`date +%s`.isl
PROBLEM_ID=33
PROBLEM_ID=33 MERGE_ISL=../../solutions.best/15.isl  ./a.out > ../../solutions/$PROBLEM_ID-shiota7`date +%s`.isl
PROBLEM_ID=34
PROBLEM_ID=34 MERGE_ISL=../../solutions.best/7.isl  ./a.out > ../../solutions/$PROBLEM_ID-shiota7`date +%s`.isl

