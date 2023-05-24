#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

typedef struct agac {
    int meyveninDegeri; // Bir nevi k değeri
    int tutunmaGucu; // Toplam vurulması gereken değer  
    struct agac *left;
    struct agac *right;
}AGAC;

typedef struct Kova {
    int dusenMeyve;
    int enUstteki;
}KOVA;

AGAC* nodelaraDegerleriYerlestir(AGAC* node, int meyveninDegeri, int tutunmaGucu, int M);
AGAC* agaciOlustur(int N, int M);
AGAC* enKucukDugum(AGAC* node);
AGAC* nodeOlustur(int meyveninDegeri, int tutunmaGucu, int M);
void agaciBastir(AGAC* node);
AGAC* nodeSil(AGAC** root, int k);
int kovaBosalanaKadarPop(KOVA* kova, int* enUstteki, int M);
int pop(KOVA* kova, int* enUstteki);
int push(KOVA* kova, int* enUstteki, int dusenMeyve, int M);
int rastgeleSayi(int N, int* sayilar);

int rastgeleSayi(int N, int* sayilar) {
    srand(time(NULL));
    int rastgele;
    do {
        rastgele = rand() % N + 1;
    } while (sayilar[rastgele-1] == 1);
    sayilar[rastgele-1] = 1;
    return rastgele;
}

// Stack yapısından eleman çıkarma
int push(KOVA* kova, int* enUstteki, int dusenMeyve, int M) {
    if (*enUstteki == M - 1) {
        printf("Kova dolu.\n");
        return -1;
    } else {
        (*enUstteki)++;
        kova[*enUstteki].dusenMeyve = dusenMeyve;
    }
}

// Stack yapısına eleman ekleme
int pop(KOVA* kova, int* enUstteki) {
    if (*enUstteki == -1) {
        printf("Kova bos.");
        return -1;
    } else {
        int meyveninDegeri = kova[*enUstteki].dusenMeyve;
        (*enUstteki)--;
        return meyveninDegeri;
    }
}

AGAC* nodeOlustur(int meyveninDegeri, int tutunmaGucu, int M) {
    AGAC* node = (AGAC*)malloc(sizeof(AGAC)); // Mallocla nodeların yerini ayırıyoruz
    node->meyveninDegeri = meyveninDegeri;
    node->tutunmaGucu = tutunmaGucu;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AGAC* agaciOlustur(int N, int M) {
    AGAC* root = NULL;
    int sayilar[N];
    int i;
    for (i = 0; i < N; i++) {
        sayilar[i] = 0;
    }
    for (i = 0; i < M; i++) {
        int meyveninDegeri = rastgeleSayi(N, sayilar); // N'e kadar rastgele değerlerle dolduruyoruz
        int tutunmaGucu = meyveninDegeri;
        root = nodelaraDegerleriYerlestir(root, meyveninDegeri, tutunmaGucu, M);
    }
    return root;
}

AGAC* nodelaraDegerleriYerlestir(AGAC* node, int meyveninDegeri, int tutunmaGucu, int M) {
    if (node == NULL) {
        return nodeOlustur(meyveninDegeri, tutunmaGucu, M); // Boşsa ilk node oluştur
    }
    if (meyveninDegeri < node->meyveninDegeri) { // Sayı küçükse sol tarafa ekle
        node->left = nodelaraDegerleriYerlestir(node->left, meyveninDegeri, tutunmaGucu, M);
    } else if (meyveninDegeri > node->meyveninDegeri) { // Sayı büyükse sağ tarafa ekle
        node->right = nodelaraDegerleriYerlestir(node->right, meyveninDegeri, tutunmaGucu, M);
    } else {
        return node;
    }
    return node;
}

void agaciBastir(AGAC* node) {
    if (node != NULL) {
        agaciBastir(node->left);
        printf("(Meyvenin degeri (k): %d, Meyvenin tutunma gucu: %d) \n", node->meyveninDegeri, node->tutunmaGucu);
        agaciBastir(node->right);
    }
}

AGAC* enKucukDugum(AGAC* node) // Sildiğimiz yerde bst yapısını korumak için en küçük değeri buluyoruz
{
    AGAC* current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

AGAC* nodeSil(AGAC** root, int k) {
    // Ağaçta k yoksa, işlem yapma
    if (*root == NULL) {
        return NULL;
    }
    // Ağacın solunda ara
    if (k < (*root)->meyveninDegeri) {
        nodeSil(&((*root)->left), k);
    }
    // Ağacın sağında ara
    else if (k > (*root)->meyveninDegeri) {
        nodeSil(&((*root)->right), k);
    }
    // Bulundu, sil
    else {
        // Yaprak düğüm
        if ((*root)->left == NULL && (*root)->right == NULL) {
            free(*root);
            *root = NULL;
        }
        // Tek çocuklu düğüm
        else if ((*root)->left == NULL) {
            AGAC* temp = *root;
            *root = (*root)->right;
            free(temp);
        }
        else if ((*root)->right == NULL) {
            AGAC* temp = *root;
            *root = (*root)->left;
            free(temp);
        }
        // İki çocuklu düğüm
        else {
            AGAC* temp = enKucukDugum((*root)->right);
            (*root)->meyveninDegeri = temp->meyveninDegeri;
            (*root)->tutunmaGucu = temp->tutunmaGucu; 
            nodeSil(&((*root)->right), temp->meyveninDegeri);
        }
    }
    return *root;
}

int kovaBosalanaKadarPop(KOVA* kova, int* enUstteki, int M) // Kovaya attığımız meyveler alttan üste doğru olduğu için pop işlemini ters çeviriyoruz
{
    int sayac = 0;
    int meyveler[M];
    int i = 0, j = 0;
    while (*enUstteki != -1) {
        meyveler[i] = pop(kova, enUstteki);
        i++;
    }
    for (j = M-1; j >= 0; j--) {
        printf("%d. meyve: %d\n", ++sayac, meyveler[j]);
    }
    return sayac;
}


int main(){
    int N, M;
    // N maksimum meyve ağırlığı
    // M meyve adedi

    printf("Maksimum meyve agirligini girin (N): ");
    scanf("%d", &N);

    printf("Meyve adedini girin (M): ");
    scanf("%d", &M);

    AGAC* root = agaciOlustur(N, M);
    KOVA* kova1 = (KOVA*)malloc(M * sizeof(KOVA));
    int i = 0;
    int enUstteki = -1; // Kovanın en üstteki elemanı
    int oyunSayaci = 0; 
    while (!oyunSayaci) {
        printf("Tur %d:\n", i++);
        agaciBastir(root);
        printf("\n");
        int p, k; // p: atış gücü, k: meyve ağırlığı
        printf("Kac gucunde vuracaksiniz? ");
        scanf("%d", &p);
        printf("Hangi k degerine sahip meyveyi vuracaksiniz? ");
        scanf("%d", &k);

        // Meyveyi ara
        AGAC *current = root;
        while (current != NULL) {
            if (current->meyveninDegeri == k) { // Meyve bulundu
                oyunSayaci = 1;
                current->tutunmaGucu -= p; // Meyve tutunma gücünü atış gücü kadar düşür
                if (current->tutunmaGucu <= 0) { // Meyve düştü
                    printf("Meyve kovaya dustu: %d\n", current->meyveninDegeri);
                    push(kova1, &enUstteki, current->meyveninDegeri, M); // Kovaya ekle
                    nodeSil(&root, k); // Meyveyi ağaçtan sil
                }
                break;
            } else if (current->meyveninDegeri > k) {
                current = current->left; // Sol alt ağaca git
            } else {
                current = current->right; // Sağ alt ağaca git
            }
        }
        if (!oyunSayaci) {
            printf("Belirtilen k degerine sahip bir meyve yok.\n");
            i--;
        }
        if(root != NULL){
            oyunSayaci = 0;
        }
    }
    kovaBosalanaKadarPop(kova1, &enUstteki, M);
}
