
void print(int a){
	printf("Hello world!");	
}

void helloworld(void (*fonksiyon)(int a)){
	fonksiyon();
}


int main() 
{
	helloworld(print);
	return 0; 
}
