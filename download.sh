for i in $(seq 36 40); do \
    echo ${i}
    curl https://cdn.robovinci.xyz/imageframes/${i}.json > problems.json/${i}.json; \
    curl https://cdn.robovinci.xyz/imageframes/${i}.png > problems/${i}.png; \
    convert problems/${i}.png problems.pam/${i}.pam; \

    curl https://cdn.robovinci.xyz/imageframes/${i}.initial.png > problems/${i}.initial.png; \
    curl https://cdn.robovinci.xyz/imageframes/${i}.initial.json > problems/${i}.initial.json; \

    curl https://cdn.robovinci.xyz/imageframes/${i}.source.png > problems/${i}.source.png; \
    curl https://cdn.robovinci.xyz/imageframes/${i}.source.json > problems/${i}.source.json; \
    convert problems/${i}.png problems.pam/${i}.source.pam; \
done
