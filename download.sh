for i in $(seq 26 30); do \
    echo ${i}
    curl https://cdn.robovinci.xyz/imageframes/${i}.json > problems.json/${i}.json; \
    curl https://cdn.robovinci.xyz/imageframes/${i}.png > problems/${i}.png; \
    convert problems/${i}.png problems.pam/${i}.pam; \

    curl https://cdn.robovinci.xyz/imageframes/${i}.initial.json > problems/${i}.initial.json; \
done