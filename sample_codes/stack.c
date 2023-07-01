#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

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

// function to convert decimal to binary
void ondanIki(int girdi)
{
    // array to store binary number
    int ondanIki[1024] = {};

    // counter for binary array
    int i = 0;
    if(girdi >= 0){
    	
    	while (girdi > 0) {

	        // storing remainder in binary array
	        ondanIki[i] = girdi % 2;
	        girdi = girdi / 2;
	        i++;
	        
	    }
	
	    // printing binary array in reverse order
	    int bitler = 8;
	    
	    if ( i > 8 ){
	    	bitler = 8*((i + 7)/8);
		}
	
	    printf("2'li Deðeri: \n");
	    
		for (int j = bitler-1; j >= 0; j--){
	    	printf("%d", ondanIki[j]);
		} 	
    	
	    for (int j = bitler-1; j >= 0; j--){
			if(ondanIki[j] == 0){
				push(ondanIki[j]);
			}
			else if(ondanIki[j] == 1){
				pop();
			}
		}	
    	
	}else{
		printf("Negatif sayý girdiniz. Stack'e signed magnitude pushlanacaktýr.");
		int girdiPozitif = abs(girdi);
		while (girdiPozitif > 0) {

	        // storing remainder in binary array
	        ondanIki[i] = girdiPozitif % 2;
	        girdiPozitif = girdiPozitif / 2;
	        i++;
	        
	    }
	
	    // printing binary array in reverse order
	    int bitler = 8;
	    
	    if ( i > 8 ){
	    	bitler = 8*((i + 7)/8);
		}
	
	    printf("2'li Deðeri: \n");
	    ondanIki[bitler-1] = 1;
		for (int j = bitler-1; j >= 0; j--){
	    	printf("%d", ondanIki[j]);
		}
		
		for (int j = bitler-1; j >= 0; j--){
			if(ondanIki[j] == 0){
				push(ondanIki[j]);
			}
			else if(ondanIki[j] == 1){
				pop();
			}
		}
	}
    
	
	
	
}

int main() {
	setlocale(LC_ALL, "Turkish");
	
   int girdi;
   
   printf("Çevrilecek sayýyý girin: \n");
   
   scanf("%d", &girdi);
   
	ondanIki(girdi);
		
	printf("\n");
	
	int sifirSayisi = peek();
	
   	if(birCounter == 0 && top >= 0){
   		printf("Sýfýr fazladýr. %d adet 1 gereklidir.", sifirSayisi);
	} 
	else if(birCounter == 0 && top == -1){
		printf("Sýfýr ve bir eþit sayýdadýr");
	}
	else {
   		printf("Bir fazladýr. %d adet 0 gereklidir.", birCounter);
	}
	

   return 0;
}
