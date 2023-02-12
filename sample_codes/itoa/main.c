#include <stdio.h>
#include <stdlib.h>

int MAXSIZE = 256;       
int stack[512];     
int top = -1;  
int birCounter = 0;         

int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

int peek() {
   return top+1;
}

int pop() {
   int data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
   		birCounter = birCounter +1;
    	// printf("Stack bos.\n");
   }
}

int push(int data) {

   if(!isfull()) {
   		if(birCounter != 0){
   			birCounter = birCounter-1;
		   } else {
		   	top = top + 1;   
      		stack[top] = data;
		   }
   } else {
      printf("Stack dolu.\n");
   }
}

int main(int argc, char *argv[]) {
	
	int girdi;
   
   	printf("Cevrilecek sayiyi girin: \n");
   
   	scanf("%d", &girdi);

    char buffer[512];
    
    itoa(girdi,buffer,2);   // here 2 means binary
    printf("Binary value = %s\n", buffer);
 
    itoa(girdi,buffer,10);   // here 10 means decimal
    printf("Decimal value = %s\n", buffer);
 
    itoa(girdi,buffer,16);   // here 16 means Hexadecimal
    printf("Hexadecimal value = %s\n", buffer);
    return 0;
}
