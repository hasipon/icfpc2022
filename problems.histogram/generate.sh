#!/bin/bash

for image in $(ls ../problems/*.png)
do
    problemid=$(basename $image .png)
    python histogram.py --image $image --output $problemid.png &
done

wait
