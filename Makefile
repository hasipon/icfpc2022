.PHONY: result-by-api
result-by-api: api-token
	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/results/user | jq . > result_by_api.json

.PHONY: submissions-list
submissions-list: api-token
	curl -H "Authorization: Bearer $$API_TOKEN" https://robovinci.xyz/api/submissions > submissions/list.json
	cd submissions && go run main.go

.PHONY: api-token
api-token:
	if [ -z "$$API_TOKEN" ]; then \
	 echo "API_TOKEN not found"; \
	 exit 1; \
	fi
