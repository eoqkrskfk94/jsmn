#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"
#include "../productlist.h"


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
		strcat(filename, ".json");
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

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo){
	int i;
	int count = 1;
	int count2 = 1;
	int select = 0;

	if(t[0].type == JSMN_OBJECT){
		for(i = 1; i < tokcount; i++)
		{
			if(t[i].size > 0 && t[i].type == JSMN_STRING && t[i].parent != 0){
					nameTokenInfo[count++].tokindex = i;
				}
		}
		nameTokenInfo[0].tokindex = count;
	}
}

void printNameList(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo){
	int i;
	printf("***** Name List *****\n");

	for(i = 1; i <nameTokenInfo[0].tokindex; i++)
		printf("[NAME%3d] %.*s\n",i,t[nameTokenInfo[i].tokindex].end-t[nameTokenInfo[i].tokindex].start,jsonstr + t[nameTokenInfo[i].tokindex].start);
}

void jsonObjectList(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo){
	int i;
	int j = 0;
	int count = 1;


	if(t[0].type == JSMN_OBJECT){
		for(i = 0; i < tokcount; i++){
			if(t[i].type == JSMN_OBJECT && t[i].parent > 0){
				nameTokenInfo[count++].objectindex = i;
			}
		}
	}
	nameTokenInfo[0].objectindex = count -1;

}

void printObjectList(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo){
	int i;

	printf("***** Object List *****\n");
	for(i = 1; i <= nameTokenInfo[0].objectindex; i++)
	{
		printf("[NAME%3d] %.*s\n",i,t[nameTokenInfo[i].objectindex+2].end-t[nameTokenInfo[i].objectindex+2].start,jsonstr + t[nameTokenInfo[i].objectindex+2].start);
	}
}

void selectNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex, NameTokenInfo *nameTokenInfo){
	int i;
	int select;
	while(1){

		printf("Select Name's no (exit:0) >> ");
		scanf("%d",&select);
		if(select == 0) break;

		printf("[NAME%2d] %.*s\n",select, t[nameTokenInfo[select].tokindex].end-t[nameTokenInfo[select].tokindex].start,jsonstr + t[nameTokenInfo[select].tokindex].start);

		for(i = 1; i < 128; i++)
		{
			if(nameTokIndex[select + i - 1] == nameTokIndex[select+1]) break;
			printf("%.*s\n",t[nameTokIndex[select]+i].end-t[nameTokIndex[select]+i].start,jsonstr + t[nameTokIndex[select]+i].start);
		}
		printf("\n");
	}
}

void selectObjectList(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo){
	int select;
	int i;
	int j;
	int count;


	while(1){
		printf("Select Object's no (exit:0) >> ");
		scanf("%d",&select);
		if(select == 0) break;
		if(nameTokenInfo[select+1].objectindex == 0) nameTokenInfo[select+1].objectindex = tokcount;


		for(i = 1; i < nameTokenInfo[0].tokindex; i++){
			if(nameTokenInfo[i].tokindex > nameTokenInfo[select].objectindex && nameTokenInfo[i].tokindex < nameTokenInfo[select+1].objectindex){

				if(nameTokenInfo[i].tokindex == nameTokenInfo[select].objectindex+1){
					printf("%.*s : %.*s\n",t[nameTokenInfo[i].tokindex].end-t[nameTokenInfo[i].tokindex].start,jsonstr + t[nameTokenInfo[i].tokindex].start,t[nameTokenInfo[i].tokindex+1].end-t[nameTokenInfo[i].tokindex+1].start,jsonstr + t[nameTokenInfo[i].tokindex+1].start);
				}
				else{
					printf("\t [%.*s]",t[nameTokenInfo[i].tokindex].end-t[nameTokenInfo[i].tokindex].start,jsonstr + t[nameTokenInfo[i].tokindex].start);
					printf("\t %.*s\n",t[nameTokenInfo[i].tokindex+1].end-t[nameTokenInfo[i].tokindex+1].start,jsonstr + t[nameTokenInfo[i].tokindex+1].start);

				}
			}
		}

	}
}

void selectTokIndex(char *jsonstr, jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo){

	int select, i,j;
	char name[128];
	char temp[128];

	while(1){
		printf("Select Object's no to get the name token (exit:0) >> ");
		scanf("%d",&select);
		if(select == 0) break;
		printf("Enter name >> ");
		scanf("%s",name);

		if(nameTokenInfo[select+1].objectindex == 0) nameTokenInfo[select+1].objectindex = tokcount;


		for(i = nameTokenInfo[select-1].objectindex; i < nameTokenInfo[0].tokindex; i++){
			if(nameTokenInfo[i].tokindex > nameTokenInfo[select+1].objectindex) break;
			if(nameTokenInfo[i].tokindex < nameTokenInfo[select].objectindex) continue;
			sprintf(temp,"%.*s",t[nameTokenInfo[i].tokindex].end-t[nameTokenInfo[i].tokindex].start,jsonstr + t[nameTokenInfo[i].tokindex].start );
			if(!strcmp(name,temp)) printf("name token number: %d\n", nameTokenInfo[i].tokindex);
		}
	}
}

void printdisplay(char *jsonstr,jsmntok_t *t, int tokcount, NameTokenInfo *nameTokenInfo){

	int i;
	printf("********************************************************************************\n");
	printf("번호 \t제품명 \t\t제조자 \t\t가격 \t\t개수 \t\t총가격\n");
	printf("********************************************************************************\n");

	for(i = 1; i <= nameTokenInfo[0].objectindex; i++){
		char string[128];
		int total_price= 0;
		printf("%d",i);
		printf(" \t%.*s",t[nameTokenInfo[i].objectindex+4].end-t[nameTokenInfo[i].objectindex+4].start,jsonstr + t[nameTokenInfo[i].objectindex+4].start);
		printf(" \t\t%.*s",t[nameTokenInfo[i].objectindex+2].end-t[nameTokenInfo[i].objectindex+2].start,jsonstr + t[nameTokenInfo[i].objectindex+2].start);
		printf(" \t\t%.*s",t[nameTokenInfo[i].objectindex+6].end-t[nameTokenInfo[i].objectindex+6].start,jsonstr + t[nameTokenInfo[i].objectindex+6].start);
		printf(" \t\t%.*s",t[nameTokenInfo[i].objectindex+8].end-t[nameTokenInfo[i].objectindex+8].start,jsonstr + t[nameTokenInfo[i].objectindex+8].start);
		sprintf(string,"%.*s",t[nameTokenInfo[i].objectindex+6].end-t[nameTokenInfo[i].objectindex+6].start,jsonstr + t[nameTokenInfo[i].objectindex+6].start);
		total_price += atoi(string);
		sprintf(string,"%.*s",t[nameTokenInfo[i].objectindex+8].end-t[nameTokenInfo[i].objectindex+8].start,jsonstr + t[nameTokenInfo[i].objectindex+8].start);
		total_price *= atoi(string);
		printf(" \t\t%d\n",total_price);
	}
}

int makeProduct(char *jsonstr, jsmntok_t *t, int tokcount, product_t *phonelist[]){
	int trigger = 1;
	int j= 0;
	int count = 1;
	int number = 0;
	char temp[1024];


	for(j= 0; j < tokcount; j++){

		if(trigger == 1){
			phonelist[number] = (product_t*)malloc(sizeof(product_t));
			trigger = 0;
		}
		sprintf(temp,"%.*s",t[j].end-t[j].start,jsonstr + t[j].start);
		if(!strcmp(temp,"삼성")) phonelist[number]->company = SAMSUNG;
		else if(!strcmp(temp,"애플")) phonelist[number]->company = APPLE;
		else if(!strcmp(temp,"엘지")) phonelist[number]->company = LG;
		else if(!strcmp(temp,"샤오미")) phonelist[number]->company = SHAOMI;

		if(!strcmp(temp,"name")){
			sprintf(temp,"%.*s",t[j+1].end-t[j+1].start,jsonstr + t[j+1].start);
			strcpy(phonelist[number]->name,temp);
		}

		if(!strcmp(temp,"release_year")) {
			sprintf(temp,"%.*s",t[j+1].end-t[j+1].start,jsonstr + t[j+1].start );
			phonelist[number]->release_year = atoi(temp);
		}

		if(!strcmp(temp,"OS")){
			sprintf(temp,"%.*s",t[j+1].end-t[j+1].start,jsonstr + t[j+1].start);
			strcpy(phonelist[number]->OS,temp);
		}

		if(!strcmp(temp,"hardware")){
			sprintf(temp,"%.*s",t[j+1].end-t[j+1].start,jsonstr + t[j+1].start);
			strcpy(phonelist[number]->hardware,temp);
			trigger = 1;
			number++;
		}

	}

	return number;
}

void printProduct(product_t *phonelist[], int pcount){

	int i;
	printf("*************************************************************************************\n");
	printf("번호 \t제품명 \t\t제조자 \t\t출시일 \t\t운영체제 \t하드웨어\n");
	printf("*************************************************************************************\n");

	for(i = 0; i < pcount; i++){
		printf("%d",i+1);
		if(phonelist[i]->company == 0) printf(" \t삼성");
		else if(phonelist[i]->company == 1) printf(" \t애플");
		else if(phonelist[i]->company == 2) printf(" \t엘지");
		else if(phonelist[i]->company == 3) printf(" \t샤오미");
		printf("\t\t%-10s",phonelist[i]->name);
		printf("\t%d",phonelist[i]->release_year);
		printf("\t\t%s",phonelist[i]->OS);
		printf("\t\t%s\n",phonelist[i]->hardware);
	}
}



int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	int name[128];
	int objectsize[128];
	NameTokenInfo tokenInfo[128];
	product_t *phonelist[20];
	int total;

	char* JSON_STRING = readJSONFile();
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

	//jsonObjectList(JSON_STRING, t, r,tokenInfo);
	//jsonNameList(JSON_STRING, t, r, tokenInfo);
	//printNameList(JSON_STRING, t, r, tokenInfo);
	//selectNameList(JSON_STRING, t, name, tokenInfo);
	//printObjectList(JSON_STRING, t, r,tokenInfo);
	//selectObjectList(JSON_STRING, t, r, tokenInfo);
	//selectTokIndex(JSON_STRING, t, r, tokenInfo);
	//printdisplay(JSON_STRING,t,r, tokenInfo);
	total = makeProduct(JSON_STRING, t, r, phonelist);
	printProduct(phonelist, total);

}
