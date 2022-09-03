
.PHONY: problems.json
problems.json:
	for i in {1..25}; do \
	  curl https://cdn.robovinci.xyz/imageframes/$${i}.json > problems.json/$${i}.json; \
	done

.PHONY: result-by-api
result-by-api:
	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/results/user > result_by_api.json

# .PHONY: submitted
# submitted:
# 	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/submissions/6710
# 	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/submissions/6709
