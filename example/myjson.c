#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

 static void printTokens(jsmntok_t *t, int count){
 	int i;
 	for(i = 0; i < count; i++){
 		printf("%d: start:%d, end:%d, size:%d, type:%d, parent:%d \n",i
 		,t[i].start,t[i].end,t[i].size,t[i].type,t[i].parent);
 	}
 }


static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	#ifdef DEBUG_MODE
		printf("Tokens count: %d\n", r);
	#endif

	printTokens(t, r);

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	/*for(i = 1; i < r; i++)
	{
		printf("[%d] %.*s\n",i ,t[i].end-t[i].start,
			JSON_STRING + t[i].start);
	}*/

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			/* We may use strndup() to fetch string value */
			printf("%d  %d\n",t[i+1].start,t[i+1].end);
			printf("[%d]- User: %.*s  parent:%d\n",i+1 ,t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start,t[i+1].parent);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("%d  %d\n",t[i+1].start,t[i+1].end);
			printf("[%d]- Admin: %.*s  parent:%d\n",i+1, t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start,t[i+1].parent);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("%d  %d\n",t[i+1].start,t[i+1].end);
			printf("[%d]- UID: %.*s  parent:%d\n",i+1, t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start,t[i+1].parent);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			printf("%d  %d\n",t[i+1].start,t[i+1].end);
			printf("- Groups:  parent:%d\n",t[i].parent);
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("[%d]  * %.*s  parent:%d\n",i+j+2, g->end - g->start, JSON_STRING + g->start,t[i+j+2].parent);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	return EXIT_SUCCESS;
}
