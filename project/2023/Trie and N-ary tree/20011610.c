#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 100
#define MAX_DIGIT_LEN 10

struct TrieNode {
	int digit;
	char letter[MAX_WORD_LEN];
	char word[MAX_WORD_LEN];
    struct TrieNode* children[MAX_DIGIT_LEN];
    int isEndOfWord;
};

struct TrieNode* createNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    newNode->isEndOfWord = 0;
    int i; 
    for (i = 0; i < MAX_DIGIT_LEN; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

int* mapping(const char* word, int len) {
    int* arr = (int*)malloc(len * sizeof(int));
    int i;
    for (i = 0; i < len; i++) {
        if (word[i] >= 'a' && word[i] <= 'c')
            arr[i] = 2;
        else if (word[i] >= 'd' && word[i] <= 'f')
            arr[i] = 3;
        else if (word[i] >= 'g' && word[i] <= 'i' || word[i] == 'ý')
            arr[i] = 4;
        else if (word[i] >= 'j' && word[i] <= 'l')
            arr[i] = 5;
        else if (word[i] >= 'm' && word[i] <= 'o')
            arr[i] = 6;
        else if (word[i] >= 'p' && word[i] <= 's')
            arr[i] = 7;
        else if (word[i] >= 't' && word[i] <= 'v')
            arr[i] = 8;
        else if (word[i] >= 'w' && word[i] <= 'z')
            arr[i] = 9;
    } 
    return arr;
}

void insertWord(struct TrieNode* root, char* word) {
    int length = strlen(word);
    int i, j, k;
    int* arr2 = mapping(word, length);
    struct TrieNode* currentNode = root;
    int dolu = 0;
    for (j = 0; j<strlen(word); j++){
    	if(currentNode->children[arr2[j]] == NULL){
    		currentNode->children[arr2[j]] = createNode();
		}
		currentNode = currentNode->children[arr2[j]];
    	if(currentNode->digit == arr2[j]){
    		dolu = dolu + 1;
		}
	}
	if (dolu != strlen(word)){
		currentNode = root;
		for (j=0; j<strlen(word); j++){
			if (currentNode->children[arr2[j]] == NULL){
				currentNode->children[arr2[j]] = createNode();
			}
			currentNode = currentNode->children[arr2[j]];
			currentNode->letter[0] = word[j];
			currentNode->digit = arr2[j];
			if (j == strlen(word) - 1){
				currentNode->isEndOfWord = 1;
				for(j=0; j<strlen(word); j++){
		        	currentNode->word[j] = word[j];
				}
			}
		}
	}
	else if (dolu == strlen(word)){
		while(currentNode->children[0] != NULL){
			currentNode = currentNode->children[0];
		}
		currentNode->children[0] = createNode();
		currentNode = currentNode->children[0];
		currentNode->digit = 0;
		// currentNode->isEndOfWord = 1;
		for(j=0; j<strlen(word); j++){
        	currentNode->letter[j] = word[j];
        	currentNode->word[j] = word[j];
		}
	}
}

int readWordsFromFile(const char* filename, char*** words) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya acilamadi.\n");
        return 0;
    }
    int wordCount = 0, i;
    char word[MAX_WORD_LEN];

    *words = (char**)malloc(MAX_WORD_LEN * sizeof(char*));
    for (i = 0; i < MAX_WORD_LEN; i++) {
        (*words)[i] = (char*)malloc(MAX_WORD_LEN * sizeof(char));
    }

    while (fgets(word, sizeof(word), file) && wordCount < MAX_WORD_LEN) {
        word[strcspn(word, "\n")] = '\0'; 
        int len = strlen(word);
        if (len > 0) {
            strncpy((*words)[wordCount], word, MAX_WORD_LEN - 1);
            (*words)[wordCount][MAX_WORD_LEN - 1] = '\0';
            wordCount++;
        }
    }
    fclose(file);
    return wordCount;
}

void searchNumber(struct TrieNode* root, int number) {
    int len, i, j, counter;
	len = 0;
	int sayi = number;
    while (number > 0) {
        number /= 10;
        len++;
    }
    
    int digits[len];
    
    for(i=0; i<len; i++){
    	digits[i] = sayi % 10;
    	sayi = sayi / 10;
	}
    
    int tmp;
	for (i=0; i<len/2; i++){
		tmp = digits[i];
		digits[i] = digits[len-i-1];
		digits[len-i-1] = tmp;
	}
    char word[len];
    struct TrieNode* currentNode = root;
    for(i=0; i<len; i++){
    	if(currentNode->children[digits[i]] == NULL){
    		printf("Kelime sozlukte yoktur.\n");
    		return;
		}
		currentNode = currentNode->children[digits[i]];
		word[i] = currentNode->letter[0];
	}
	if(currentNode->isEndOfWord != 1){
		printf("Kelime sozlukte yoktur.\n");
    	return;
	}
	printf("Kelime bulundu: \n");
	printf("%s \n", currentNode->word);
	while(currentNode->children[0] != NULL){
		currentNode = currentNode->children[0];
		for(i=0; i<len; i++){
			printf("%c", currentNode->word[i]);
		}
		printf("\n");
	}
}



int main() {
    char filename[MAX_WORD_LEN];
    printf("Sozluk dosyasinin adini uzantisiyla beraber girin: ");
    scanf("%s", filename);
    struct TrieNode* root = createNode('\0',-1);
    char** words;
    int i;
    int wordCount = readWordsFromFile(filename, &words);
    printf("Sozlukteki Kelimeler: \n");
    for (i = 0; i < wordCount; i++) {
    	printf("%s \n", words[i]);
        insertWord(root, words[i]);
    }
    int number;
    printf("\nSorgulanacak sayiyi girin: ");
    while (scanf("%d", &number) == 1) {
        searchNumber(root, number);
        printf("Sorgulanacak sayiyi girin ('sayi' girmeyerek cikis yapabilirsiniz): ");
    }
    return 0;
}

