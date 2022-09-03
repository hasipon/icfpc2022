
.PHONY: problems.json
problems.json:
	for i in {1..25}; do \
	  curl https://cdn.robovinci.xyz/imageframes/$${i}.json > problems.json/$${i}.json; \
	done

.PHONY: result-by-api
result-by-api:
	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/results/user > result_by_api.json

.PHONY: submissions
submissions:
	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/submissions > submissions/list.json
# 	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/submissions/6709
