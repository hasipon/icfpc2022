#!/bin/bash

for i in `ls ../problems.pam/* | grep -v txt`
do
    echo $i
    problemid=$(basename $i .pam)
    cat $i | ./a.out > $problemid.txt &
done

wait
