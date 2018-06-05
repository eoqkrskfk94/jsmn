#ifndef __PRODUCT_H_
#define __PRODUCT_H_

#include <stddef.h>


typedef struct {
  int tokindex;
  int objectindex;

}NameTokenInfo;

typedef enum {
	SAMSUNG = 0,
	APPLE = 1,
	LG = 2,
	SHAOMI = 3
} company_t;



typedef struct{
  company_t company;  //제조사
  char name[20];      //제품명
  int release_year;   //가격
  char OS[20];        //개수
  char hardware[50];  //CPU
} product_t;


#endif
