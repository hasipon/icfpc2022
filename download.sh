for i in $(seq 31 35); do \
    echo ${i}
    curl https://cdn.robovinci.xyz/imageframes/${i}.json > problems.json/${i}.json; \
    curl https://cdn.robovinci.xyz/imageframes/${i}.png > problems/${i}.png; \
    convert problems/${i}.png problems.pam/${i}.pam; \

    curl https://cdn.robovinci.xyz/imageframes/${i}.initial.png > problems/${i}.initial.png; \
    curl https://cdn.robovinci.xyz/imageframes/${i}.initial.json > problems/${i}.initial.json; \
done
