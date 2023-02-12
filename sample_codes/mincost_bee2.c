#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include "beeheader.h"

// ITERATIONO 5
// BEENUMBER 5

int main(){
	int k = 0;
	while(k<ITERAITONNO){
			int min, i;
		int bee1, bee2, bee3, bee4, bee5;
		int beeArray[5];
		srand(time(NULL));
		bee1 = rand()%100;
		beeArray[0] = bee1;
		bee2 = rand()%100;
		beeArray[1] = bee2;
		bee3 = rand()%100;
		beeArray[2] = bee3;
		bee4 = rand()%100;
		beeArray[3] = bee4;
		bee5 = rand()%100;
		beeArray[4] = bee5;
	
		printf("%d %d %d %d %d \n", beeArray[0], beeArray[1], beeArray[2], beeArray[3], beeArray[4]);
		min = beeArray[0];
		for(i=1; i<5; i++){
			if(min>beeArray[i]){
				min = beeArray[i];
			}
		}
		printf("En dusuk maliyet: %d \n", BEEFUNC(min));
		k = k+1;
		
		}
	
	}
