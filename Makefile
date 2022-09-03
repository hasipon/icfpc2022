
.PHONY: problems.json
problems.json:
	for i in {1..25}; do \
	  curl https://cdn.robovinci.xyz/imageframes/$${i}.json > problems.json/$${i}.json; \
	done
