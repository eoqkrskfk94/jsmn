#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

/*static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";*/

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char *readJSONFile() {
		FILE* file;
		char filename[128];
		printf("원하는 파일명 입력: ");
		scanf("%s",filename);
		file = fopen(filename, "r");
		if(file == NULL) {
			printf("Error opening file. \n");
			return NULL;
		}

		int count = 0;
		char line[256];
		char *JSON_STRING = (char*)malloc(sizeof(line));


		while(1){
			fgets(line,sizeof(line),file);
			if(feof(file)) break;
			count += strlen(line);
			//realloc(JSON_STRING,count+1);
			JSON_STRING = (char*)realloc(JSON_STRING,count+1);
			strcat(JSON_STRING,line);
		}

		return JSON_STRING;
	}

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex, int* sizeOfObject){
	int i;
	int count = 1;
	int count2 = 1;
	int select = 0;
	sizeOfObject[0] = 0;

	//case1: 전체 토큰이 object일때
	if(t[0].type == JSMN_OBJECT){
		for(i = 1; i < tokcount; i++)
		{
			if(t[i].size > 0 && t[i].type == JSMN_STRING)
					nameTokIndex[count++] = i;
		}
		nameTokIndex[0] = count;
	}
	//case2: 전체 토큰이 array일때
	if(t[0].type == JSMN_ARRAY){
		for(i = 1; i < tokcount; i++)
		{
			if(t[i].size > 0 && t[i].type == JSMN_STRING)
					nameTokIndex[count++] = i;
		}
		nameTokIndex[0] = count;
	}

}

void printNameList(char *jsonstr, jsmntok_t *t, int tokcount, int *nameTokIndex){
	int i;
	//int save;
	printf("***** Name List *****\n");
	for(i = 1; i < nameTokIndex[0]; i++)
	{
		printf("[NAME%3d] %.*s type: %d size: %d parent: %d\n",i,t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start, t[nameTokIndex[i]].type,t[nameTokIndex[i]].size,t[nameTokIndex[i]].parent);
		//save = t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start;
		//printf("save : %d\n",save);
	}
}

int *jsonObjectList(char *jsonstr, jsmntok_t *t, int tokcount){
	int i;
	int j = 0;
	int count = 1;

	int *objectTokIndex = (int*)malloc(sizeof(int)*2);

	//case2: 전체 토큰이 object{}일때
	if(t[0].type == JSMN_OBJECT){
		for(i = 0; i < tokcount; i++){
			if(t[i].type == JSMN_OBJECT && t[i-1].size == 0 && t[i].parent < 0){
				objectTokIndex = (int*)realloc(objectTokIndex, sizeof(int));
				objectTokIndex[count] = i;
				count++;
			}
		}
	}
	//case2: 전체 토큰이 array일때
	if(t[0].type == JSMN_ARRAY){
		for(i = 0; i < tokcount; i++){
			if(t[i].type == JSMN_OBJECT){
				objectTokIndex = (int*)realloc(objectTokIndex, sizeof(int));
				objectTokIndex[count] = i;
				count++;
			}
		}
	}

	objectTokIndex[0] = count - 1;

	return objectTokIndex;
}

void printObjectList(char *jsonstr, jsmntok_t *t, int tokcount, int *objectTokIndex){
	int i;
	int j = 0;
	int count = 1;


	printf("***** Object List *****\n");
	for(i = 1; i <= objectTokIndex[0]; i++)
	{
		printf("[NAME%3d] %.*s\n",i,t[objectTokIndex[i]+2].end-t[objectTokIndex[i]+2].start,jsonstr + t[objectTokIndex[i]+2].start);
	}
}

void selectNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
	int i;
	int select;
	while(1){

		printf("Select Name's no (exit:0) >> ");
		scanf("%d",&select);
		if(select == 0) break;

		printf("[NAME%2d] %.*s\n",select, t[nameTokIndex[select]].end-t[nameTokIndex[select]].start,jsonstr + t[nameTokIndex[select]].start);

		for(i = 1; i < 128; i++)
		{
			if(nameTokIndex[select + i - 1] == nameTokIndex[select+1]) break;
			printf("%.*s\n",t[nameTokIndex[select]+i].end-t[nameTokIndex[select]+i].start,jsonstr + t[nameTokIndex[select]+i].start);
		}
		printf("\n");
	}
}
/*
void selectObjectList(char *jsonstr, jsmntok_t *t, int tokcount, int *objectTokIndex, int *nameTokIndex, char *objectstr, int *objectsize){
int select;
int i;
int count;
char string[128];
char line[128];
int size;
size = nameTokIndex[0]/objectTokIndex[0];
printf("size: %d\n",size);
while(1){
	char *JSON_OBJECT = (char*)malloc(sizeof(line));
	strcpy(JSON_OBJECT,"");
	printf("Select Object's no (exit:0) >> ");
	scanf("%d",&select);
	if(select == 0) break;
	sprintf(string,"%.*s : %.*s",t[objectTokIndex[select]+1].end-t[objectTokIndex[select]+1].start,jsonstr + t[objectTokIndex[select]+1].start,
	t[objectTokIndex[select]+2].end-t[objectTokIndex[select]+2].start,jsonstr + t[objectTokIndex[select]+2].start);
	strcat(JSON_OBJECT,string);

	for(i = 2; i <= size ; i++)
	{
		sprintf(string,"\n	[%.*s] ",t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start);
		count += strlen(string);
		JSON_OBJECT = (char*)realloc(JSON_OBJECT,count+1);
		strcat(JSON_OBJECT,string);
		sprintf(string," %.*s",t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start,jsonstr + t[nameTokIndex[i]+1].start);
		count += strlen(string);
		JSON_OBJECT = (char*)realloc(JSON_OBJECT,count+1);
		strcat(JSON_OBJECT,string);

	}

	printf("%s\n",JSON_OBJECT);
	objectstr = JSON_OBJECT;
}
}
*/
void selectObjectList(char *jsonstr, jsmntok_t *t, int tokcount, int *objectTokIndex, int *nameTokIndex, char *objectstr, int *objectsize){
int select;
int i;
int j;
int count;
char string[128];
char line[128];


while(1){
	printf("Select Object's no (exit:0) >> ");
	scanf("%d",&select);
	if(select == 0) break;

	i = objectTokIndex[select]+1;

	for(j = 1; j < 3; j++){
		printf("\t [%.*s]",t[nameTokIndex[j+i]].end-t[nameTokIndex[j+i]].start,jsonstr + t[nameTokIndex[j+i]].start);
		printf("    %.*s\n",t[nameTokIndex[j+i]+1].end-t[nameTokIndex[j+i]+1].start,jsonstr + t[nameTokIndex[j+i]+1].start);
	}






}

}


int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	int name[128];
	int objectsize[128];

	char* JSON_STRING = readJSONFile();
	char* JSON_OBJECT;
	//printf("%s",JSON_STRING);

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 /*|| t[0].type != JSMN_OBJECT */) {
		printf("Object expected\n");
		return 1;
	}
	int *object = jsonObjectList(JSON_STRING, t, r);

	printf("\n total num of token: %d\n\n", r);

	jsonNameList(JSON_STRING, t, r, name, objectsize);
	printNameList(JSON_STRING, t, r, name);
	selectNameList(JSON_STRING, t, name);
	printObjectList(JSON_STRING, t, r, object);
	//selectObjectList(JSON_STRING, t, r, object, name, JSON_OBJECT, objectsize);
 /*
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "name") == 0) {

			printf("- name: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "keywords") == 0) {

			printf("- keywords: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "description") == 0) {

			printf("- description: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		}   else if (jsoneq(JSON_STRING, &t[i], "examples") == 0) {

			printf("- examples: \n  * %.*s\n", t[i+2].end-t[i+2].start,
					JSON_STRING + t[i+2].start);
			i++;
		} */
		/*else if (jsoneq(JSON_STRING, &t[i], "repository") == 0) {
			int j;
			printf("- repository:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue;  We expect groups to be an array of strings
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);

	}*/



	/*
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			printf("- User: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			printf("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue;
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}*/

	//return EXIT_SUCCESS;
}
