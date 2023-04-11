#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// çark yapısı struct, typedef kullanarak tipini CARK olarak tanımladım
typedef struct Cark {
    int sayi;
    struct Cark *next;
    struct Cark *prev;
} CARK;

int carklariDoldur(CARK* CARK, int M, int N, int ortakSayi);
void carklariYazdir(CARK *cark1, CARK *cark2, CARK *cark3, int ortakSayi);
void ortakiSayiveKonum(CARK *CARK, int ortakSayi);
CARK* carkOlustur(CARK **head, int M);
int getPosition(CARK* cark, int num);

CARK *carkOlustur(CARK **head, int M) { // m sayısı kadar linkli listede node oluşturur
    CARK *temp, *iter;
    for (int i = 0; i < M; i++) {
        temp = (CARK *)malloc(sizeof(CARK));
        temp->sayi = 0;
        temp->next = NULL;
        temp->prev = NULL;
        if (*head == NULL) {
            *head = temp;
            iter = *head;
        } else {
            iter->next = temp;
            temp->prev = iter;
            iter = iter->next;
        }
    }
    iter->next = *head;
    (*head)->prev = iter;

    return *head;
}

int carklariDoldur(CARK *head, int M, int N, int ortakSayi) {
    int* kullanilanSayilar = (int*) calloc(N+1, sizeof(int)); // kullanılan sayıların listesi
    int* carktaKullanilan = (int*) calloc(N+1, sizeof(int)); // kaç çarkta yer aldığı takip edilen sayıların listesi
    int ortakIndex = rand() % M; // ortak sayının yerleştirileceği rastgele index
    int i = 0;
    int ortakSayiKullanilan = 0; // ortak sayı daha önce kullanıldı mı kontrolü
    int* ortakSayiCarki = (int*) calloc(M, sizeof(int)); // ortak sayının hangi çarkta kullanıldığı takibi

    CARK* iter = head;
    while (i < M) {
        if (i == ortakIndex && !ortakSayiKullanilan) { // ortak sayıyı yerleştirme
            iter->sayi = ortakSayi;
            kullanilanSayilar[ortakSayi] = 1; // ortak sayı kullanıldı olarak işaretlenir
            carktaKullanilan[ortakSayi]++;
            ortakSayiKullanilan = 1; // ortak sayı kullanıldı olarak işaretlenir
            ortakSayiCarki[i] = 1; // ortak sayı bu çarkta kullanıldı olarak işaretlenir
        } else { // farklı sayıları yerleştirme
            int rastgeleSayi;
            int sayiKullanimSayisi;
            do {
                rastgeleSayi = rand() % N + 1;
                sayiKullanimSayisi = carktaKullanilan[rastgeleSayi];
            } while (kullanilanSayilar[rastgeleSayi] || sayiKullanimSayisi >= 2 || (i != ortakIndex && rastgeleSayi == ortakSayi)); // kullanılmamış ve en fazla 2 çarkta olan rastgele sayı bulunana kadar döngü devam eder
            iter->sayi = rastgeleSayi;
            kullanilanSayilar[rastgeleSayi] = 1; // kullanılan sayı işaretlenir
            carktaKullanilan[rastgeleSayi]++;
        }
        i++;
        iter = iter->next;
    }

    // ortak sayının sadece bir kere kullanıldığı kontrol edilir ve gerekli düzenlemeler yapılır
    if (ortakSayiKullanilan) {
        int numWheelsUsed = 0;
        for (int j = 0; j < M; j++) {
            if (ortakSayiCarki[j]) { 
                numWheelsUsed++;
            }
        }
        if (numWheelsUsed > 1) {
            // ortak sayı birden fazla çarkta kullanılmış, tekrar doldurma işlemi yapılır
            free(kullanilanSayilar);
            free(carktaKullanilan);
            free(ortakSayiCarki);
            carklariDoldur(head, M, N, ortakSayi);
            return ortakIndex;
        }
    }
    free(kullanilanSayilar); // memory leak olmaması için kullanılan sayıların listesi silinir
    free(carktaKullanilan);     
}


void carklariYazdir(CARK *cark1, CARK *cark2, CARK *cark3, int ortakSayi) { // çarkların linkli listede gezerek yazdırılması
    printf("CARK 1: ");
    CARK *tmp1 = cark1;
    do {
        printf("%d \t", tmp1->sayi);
        tmp1 = tmp1->next;
    } while (tmp1 != cark1);
    printf("\n");

    printf("CARK 2: ");
    CARK *tmp2 = cark2;
    do {
        printf("%d \t", tmp2->sayi);
        tmp2 = tmp2->next;
    } while (tmp2 != cark2);
    printf("\n");

    printf("CARK 3: ");
    CARK *tmp3 = cark3;
    do {
        printf("%d \t", tmp3->sayi);
        tmp3 = tmp3->next;
    } while (tmp3 != cark3);
    printf("\n");

    printf("Ortak sayi: %d\n\n", ortakSayi);
}


void ortakiSayiveKonum(CARK *cark, int ortakSayi) { // çarklardaki ortak sayının konumunu bulma
    int konum = 0;
    CARK *tmp = cark;
    while (tmp != NULL) {
        konum++;
        if (tmp->sayi == ortakSayi) {
            printf("Carkta ortak sayi %d, konum %d\n", ortakSayi, konum);
            return;
        }
        tmp = tmp->next;
    }
    printf("Carkta ortak sayi %d, konum -1 (bulunamadi)\n", ortakSayi);
}

int dondurmeyiHesapla(CARK* cark1, CARK* cark2, CARK* cark3, int ortakSayi) {
    int *steps = malloc(2*sizeof(int));
    int pos = getPosition(cark1, ortakSayi); // ortak sayının 1. çarkta konumunu bul
    steps[0] = (pos - getPosition(cark2, ortakSayi)) % 40; // 2. çarkın ortak sayıya göre dönme adımı
    steps[1] = (pos - getPosition(cark3, ortakSayi)) % 40; // 3. çarkın ortak sayıya göre dönme adımı
    
    if(steps[0] > 0){
        printf("\n2. cark %d birim saat yonunde cevrilmeli\n", steps[0]);
    } else if(steps[0] < 0) {
        printf("\n2. cark %d birim saat yonunun tersine cevrilmeli\n", abs(steps[0]));
    } else {
        printf("\n2. cark degistirilmeyecek\n");
    }

    if(steps[1] > 0){
        printf("\n3. cark %d birim saat yonunde cevrilmeli\n", steps[1]);
    } else if(steps[1] < 0) {
        printf("\n3. cark %d birim saat yonunun tersine cevrilmeli\n", abs(steps[1]));
    } else {
        printf("\n3. cark degistirilmeyecek\n");
    }

    return *steps; // normalde burda return ettiğim steps'i diğer fonksiyona paramettre olarak verecektim fakat sonrasında değiştirdim direkt orda hesaplama yaptım
}

int getPosition(CARK* cark, int num) { // dönme adımını hesaplamak için ortak sayının konumunu buluyorum
    int pos = 0;
    CARK* current = cark;
    while(current->sayi != num) {
        current = current->next;
        pos++;
    }
    return pos;
}

void carklariDondur(CARK *cark1, CARK *cark2, CARK *cark3, int M, int ortakSayi) {
    // ortak sayının konumunu bulalım
    int konum1 = 0, konum2 = 0, konum3 = 0;
    CARK *currentCark = cark1;
    while (currentCark->sayi != ortakSayi) {
        konum1++;
        currentCark = currentCark->next;
    }
    currentCark = cark2;
    while (currentCark->sayi != ortakSayi) {
        konum2++;
        currentCark = currentCark->next;
    }
    currentCark = cark3;
    while (currentCark->sayi != ortakSayi) {
        konum3++;
        currentCark = currentCark->next;
    }

    // çark 2 döndürülmesi
    int cark2Konum = (konum2 + M - konum1) % M;
    while (cark2Konum > 0) {
        cark2 = cark2->next;
        cark2Konum--;
    }

    // çark 3 döndürülmesi
    int cark3Konum = (konum1 + M - konum3) % M;
    while (cark3Konum > 0) {
        cark3 = cark3->prev;
        cark3Konum--;
    }

    // çarkların yazdırılması
    printf("Birinci Cark: %d ", cark1->sayi);
    currentCark = cark1;
    for (int i = 0; i < M-1; i++) {
        currentCark = currentCark->next;
        printf("%d ", currentCark->sayi);
    }
    printf("\n");

    printf("Ikinci Cark: %d ", cark2->sayi);
    currentCark = cark2;
    for (int i = 0; i < M-1; i++) {
        currentCark = currentCark->next;
        printf("%d ", currentCark->sayi);
    }
    printf("\n");

    printf("Ucuncu Cark: %d ", cark3->sayi);
    currentCark = cark3;
    for (int i = 0; i < M-1; i++) {
        currentCark = currentCark->next;
        printf("%d ", currentCark->sayi);
    }
    printf("\n");
}


int main() {

    srand(time(NULL)); // zamanı kullanarak rastgele sayı üretmek srand
    int N, M, ortakSayi, ortakIndex;
	CARK *cark1 = NULL; // çarkların başlangıç değerleri NULL'lanır
	CARK *cark2 = NULL;
	CARK *cark3 = NULL;
    
    printf("Sayilar kaca kadar rastgele olusturulacak? (N): "); // sayılar 1-N arasında rastgele üretileceği için kullanıcıdan kaça kadar olan sayılar alınsın diye sorulur
    scanf("%d", &N);

    do {
        printf("Her carkta kac adet sayi olacak? (M): "); // her çarktaki sayı adeti kullanıcıdan alınır ve bu sırada sayıların rastgele oluşurken tekrarlanmaması için kontrol yapılır
        scanf("%d", &M);
        if (M >= N/2) {
            printf("Hata: M sayisi N/2'den kucuk olmalidir!\n");
        }
    } while (M >= N/2);

    while (N <= 0 || M <= 0 || N < M) {
        printf("Hata! N ve M degerleri pozitif olmali ve M <= N olmalidir.\n"); // eğer negatif sayılarsa girilirse veya M sayısı N'den büyükse hata mesajı verilir

        printf("Sayilar kaca kadar rastgele olusturulacak? (N): ");
        scanf("%d", &N);

        printf("Her carkta kac adet sayi olacak? (M): ");
        scanf("%d", &M);
    }
    int *steps = malloc(2*sizeof(int)); // çarkların dönme adımlarını tuttuğum dizi
    ortakSayi = rand() % N + 1;     // ortak sayının rastgele belirlenmesi
    printf("Ortak sayi belirlendi: %d\n", ortakSayi);
    cark1 = carkOlustur(&cark1, M); // çarkların oluşturulması
    cark2 = carkOlustur(&cark2, M);
    cark3 = carkOlustur(&cark3, M);
    ortakIndex = carklariDoldur(cark1, M, N, ortakSayi); // oluşan çark yapılarının doldurulması
    carklariDoldur(cark2, M, N, ortakSayi);
    carklariDoldur(cark3, M, N, ortakSayi);
    carklariYazdir(cark1, cark2, cark3, ortakSayi); // çarkların yazdırılması
    ortakiSayiveKonum(cark1, ortakSayi); // çarklardaki ortak sayının konumunun bulunması
    ortakiSayiveKonum(cark2, ortakSayi);
    ortakiSayiveKonum(cark3, ortakSayi);
    *steps = dondurmeyiHesapla(cark1, cark2, cark3, ortakSayi); // birinci çarka göre diğer çarkların dönme miktarlarının hesaplanması
    printf("\n");
    carklariDondur(cark1, cark2, cark3, M, ortakSayi); // çarkların döndürülüp bastırılması

    return 0;
}
