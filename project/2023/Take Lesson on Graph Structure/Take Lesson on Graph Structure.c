#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int dersNo;
    struct Node* next;
} NODE;

typedef struct Graph {
    int dersSayisi;
    int* indegreeSayisi;
    NODE** komsulukListesi;
} GRAPH;

NODE* nodeOlustur(int dersNo) {
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    newNode->dersNo = dersNo;
    newNode->next = NULL;
    return newNode;
}

GRAPH* grafOlustur(int dersSayisi) {
    GRAPH* graph = (GRAPH*)malloc(sizeof(GRAPH));
    graph->dersSayisi = dersSayisi;
    int i;
    graph->komsulukListesi = (NODE**)malloc(dersSayisi * sizeof(NODE*));
    graph->indegreeSayisi = (int*)calloc(dersSayisi, sizeof(int));
    for (i = 0; i < dersSayisi; i++) {
        graph->komsulukListesi[i] = NULL;
    }
    return graph;
}

void onKosulekle(GRAPH* graph, int ders, int onKosul) {
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    newNode->dersNo = onKosul;
    newNode->next = NULL;

    if (graph->komsulukListesi[ders] == NULL) {
        graph->komsulukListesi[ders] = newNode;
    } else {
        NODE* currentNode = graph->komsulukListesi[ders];
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
    graph->indegreeSayisi[ders]++;
}


void grafiYazdir(GRAPH* graph) {
	int i;
    printf("\nKomsuluk Listesi\n");
    for (i = 0; i < graph->dersSayisi; i++) {
        NODE* currentNode = graph->komsulukListesi[i];
        printf("Ders %d: ", i+  1);
        while (currentNode != NULL) {
            printf("%d ", currentNode->dersNo + 1);
            currentNode = currentNode->next;
        }
        printf("\n");
    }
}

int ara(NODE* head, int flag)
{
    NODE* current = head;

    while (current != NULL) {
        if (current->dersNo == flag)
            return 1;
        current = current->next;
    }

    return -1;
}

int dersleriAl(GRAPH* graph) {
    int donem = 1;
    int alinanDersSayisi = 0;
    int flag = 0;

    while (alinanDersSayisi < graph->dersSayisi) {
    	int *dizi = (int*) malloc (graph->dersSayisi * sizeof(int));
    	int j = 0, i = 0, k = 0;
        printf("\n%d. Donem Aldigi Dersler:\n", donem);
        for (i = 0; i < graph->dersSayisi; i++) {
            if (graph->indegreeSayisi[i] == 0) {
            	dizi[j] = i;
                printf("Ders %d\n", i + 1);
                graph->indegreeSayisi[i] = -1;
				alinanDersSayisi++;
                j++;
            }
    	}
    	for (i = 0; i<j; i++){
    		for(k=0; k<graph->dersSayisi; k++){
    			flag = ara(graph->komsulukListesi[k], dizi[i]);
    			if (flag == 1){
    				graph->indegreeSayisi[k]--;
				}
				int flag = 0;
			}
		}
		donem++;
		printf("\nKalan Dersler\n");
		for(i=0; i<graph->dersSayisi; i++){
			if(graph->indegreeSayisi[i] > -1){
				printf("%d. Ders: %d\n", i+1, graph->indegreeSayisi[i]);
			}
		}
	}
	printf("Ders Kalmadi.\n");
    return donem;
}

int main() {
    int N, donem, i, j; 
    printf("Ders Sayisi (N): ");
    scanf("%d", &N);

    int** komsulukMatrisi = (int**)malloc(N * sizeof(int*));
    for (i = 0; i < N; i++) {
        komsulukMatrisi[i] = (int*)malloc(N * sizeof(int));
    }

    printf("\nKomsuluk Matrisini Girin:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("Matris[%d][%d]: ", i, j);
            scanf("%d", &komsulukMatrisi[i][j]);
        }
	}

    GRAPH* graph = grafOlustur(N);
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (komsulukMatrisi[i][j] == 1) {
                onKosulekle(graph, j, i);
            }
        }
    }

    grafiYazdir(graph);
    
    printf("\nIndegree Dizisi:\n");
	for (i = 0; i < N; i++) {
	    printf("Ders %d: %d\n", i + 1, graph->indegreeSayisi[i]);
	}
	
    donem = dersleriAl(graph);
    
	int donemSayisi = donem;
	printf("\nOgrenci toplamda %d donemde okulu bitirecektir.\n", donemSayisi - 1);
	
    for (i = 0; i < N; i++) {
        free(komsulukMatrisi[i]);
    }
    free(komsulukMatrisi);
    
    for (i = 0; i < N; i++) {
        NODE* currentNode = graph->komsulukListesi[i];
        while (currentNode != NULL) {
            NODE* temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
    }
    free(graph->komsulukListesi);
    free(graph);

    
    return 0;
}
