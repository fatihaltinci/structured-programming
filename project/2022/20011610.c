#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

typedef struct Ogrenci{
	char ogrID[9]; // 8 -> \0 i�in 9 oldu
	char ad[30]; 
	char soyad[30]; 
	int puan; 
	struct Ogrenci *next; 
	struct Ogrenci *prev;
}OGRENCI;

typedef struct Yazar{
	int yazarID; 
	char yazarAd[30]; 
	char yazarSoyad[30]; 
	struct Yazar *next;
}YAZAR;

typedef struct KitapOrnek{
	char etiketNo[20]; 
	char durum[8]; 
	struct KitapOrnek *next;
}KITAPORNEK;

typedef struct Kitap{
	char kitapAdi[30]; 
	char ISBN[13]; 
	int adet; 
	struct Kitap *next; 
	struct KitapOrnek *head;
}KITAP;

typedef struct KitapYazar{
	char ISBN[13]; 
	int yazarID;
}KITAPYAZAR;

typedef struct Tarih{
	int gun; 
	int ay; 
	int yil;
}TARIH;

typedef struct KitapOdunc{
	char etiketNo[20]; 
	char ogrID[9]; 
	int islemTipi:1; 
	struct Tarih islemTarihi;
}KITAPODUNC;

OGRENCI *ogrenciOkuCSV(const char *dosyaAdi);
KITAP *kitapOkuCSV(const char *dosyaAdi);
YAZAR *yazarOkuCSV(const char *dosyaAdi);
void kitapOduncOkuCSV(const char *dosyaAdi, KITAPODUNC *kitapOdunc, int kitapOduncSayi);
KITAPYAZAR *kitapYazarOkuCSV(const char *dosyaAdi, KITAPYAZAR *kitapyazar, int satirSayisi);
int kitapYazarSayisiOkuCSV(const char *dosyaAdi);

int kitapYazarEslestir(KITAPYAZAR **head, KITAPYAZAR *kitapyazar, int N);

OGRENCI *ogrenciOlustur(char *ogrID, char *ad, char *soyad, int puan);
KITAP *kitapOlustur(char *ad, char *isbn, int adet);
YAZAR *yazarOlustur(int yazarID, char *ad, char *soyad);
// KITAPODUNC *kitapOduncOlustur(char *etiketNo, char *ogrID, int islemTipi, TARIH *islemTarihi);

int ogrenciEkle(OGRENCI **head);
int kitapEkle(KITAP **head);
int yazarEkle(YAZAR **head);
KITAPORNEK *kitapOrnekEkle(KITAPORNEK *head, char *etiketNo, char *durum);

int ogrenciSil(OGRENCI **head);
int kitapSil(KITAP **head);
int yazarSil(YAZAR **head);
KITAPORNEK *kitapOrnekSil(KITAPORNEK *head, char *etiketNo);

int ogrenciGuncelle(OGRENCI **head);
int kitapGuncelle(KITAP **head);
int yazarGuncelle(YAZAR **head);

int degerDondurG(int deger);
int degerDondurA(int deger);
int degerDondurS(int deger);

void tumOgrenciListele(OGRENCI *head);
void tumKitapListele(KITAP *head);
void tumYazarListele(YAZAR *head);
void kitapOduncDurumlariListele(KITAPODUNC *kitapodunc, int kitapOduncSayi);
void kitapYazarEslesmeDurumlariListele(KITAPYAZAR *kitapyazar, int N);

void ogrenciEkleCSV(OGRENCI **head, OGRENCI *yeniOgrenci);
void kitapEkleCSV(KITAP **head, KITAP *yeniKitap);
void yazarEkleCSV(YAZAR **head, YAZAR *yeniYazar);
// void kitapOduncEkleCSV(KITAPODUNC **head, KITAPODUNC *yeniKitapOdunc);


void ogrenciAdiDegistir(OGRENCI** head, char* ogrID, char* yeniAd);
void kitapAdiDegistir(KITAP** head, char* ISBN, char* kitapAdi);
void yazarAdiDegistir(YAZAR** head, int yazarID, char* yeniAd);
void ogrenciSoyadiDegistir(OGRENCI** head, char* ogrID, char* yeniSoyad);
void yazarSoyadiDegistir(YAZAR** head, int yazarID, char* yeniSoyad);
void ogrenciNumarasiDegistir(OGRENCI** head, char* ad, char *soyad, char* yeniNumara);
void isbnNumarasiDegistir(KITAP** head, char* kitapAdi, char *ISBN);
void kitapAdetDegistir(KITAP** head, char* ISBN, int adet);
void kitapYazarDegistir(KITAPYAZAR *kitapyazar, char* ISBN, int yazarID, int N);
void ogrenciBilgisiGoruntuleID(OGRENCI *head, char *ogrID);
void ogrenciBilgisiGoruntuleAdSoyad(OGRENCI *head, char *ad, char *soyad);
void kitapBilgisiGoruntule(KITAP *head, char *kitapAdi);
void yazarBilgisiGoruntule(YAZAR *head, char *yazarAdi);
void kitapOrnekAl(KITAP *head, char *etiketNo, char *ogrID);
void kitapOrnekVer(KITAP *head, char *etiketNo, char *ogrID);
void kitapOrnekleriniOlustur(KITAP *head);
void kitapOrnekleriniListele(KITAP *head);
void dosyayaYazKitap(const char *dosyaAdi, KITAP *head);
void dosyayaYazYazar(const char *dosyaAdi, YAZAR *head);
void dosyayaYazOgrenci(const char *dosyaAdi, OGRENCI *head);
void dosyayaYazKitapOdunc(const char *dosyaAdi, KITAPODUNC *kitapodunc, int kitapOduncSayi);
void dosyayaYazKitapYazar(const char *dosyaAdi, KITAPYAZAR *kitapyazar, int N);
void kitapOduncAlEkle(const char *dosyaAdi, KITAPODUNC *kitapodunc, int kitapOduncSayi, char *etiketNo, char *ogrID, int p, int d, int m, int y);
void cezaliOgrenciListele(OGRENCI *head);
void zamanindaTeslimEdilmeyenKitaplariListele(KITAP *head, int zamanFarki);


void ogrenciEkleCSV(OGRENCI **head, OGRENCI *yeniOgrenci) {
  if (*head == NULL) {
    *head = yeniOgrenci;
    return;
  }
  OGRENCI *last = *head;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = yeniOgrenci;
  yeniOgrenci->prev = last;
}

void kitapEkleCSV(KITAP **head, KITAP *yeniKitap) {
  if (*head == NULL) {
    *head = yeniKitap;
    return;
  }
  KITAP *last = *head;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = yeniKitap;
}

void yazarEkleCSV(YAZAR **head, YAZAR *yeniYazar) {
  if (*head == NULL) {
    *head = yeniYazar;
    return;
  }
  YAZAR *last = *head;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = yeniYazar;
}

/* void kitapOduncEkleCSV(KITAPODUNC **head, KITAPODUNC *yeniKitapOdunc) {
	if (*head == NULL) {
    *head = yeniKitapOdunc;
    return;
  }
  KITAPODUNC *last = *head;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = yeniKitapOdunc;
} */

OGRENCI *ogrenciOlustur(char *ogrID, char *ad, char *soyad, int puan) {
  OGRENCI *ogrenci = (OGRENCI*)malloc(sizeof(OGRENCI));
  strcpy(ogrenci->ogrID, ogrID);
  strcpy(ogrenci->ad, ad);
  strcpy(ogrenci->soyad, soyad);
  ogrenci->puan = puan;
  ogrenci->next = ogrenci->prev = NULL;
  return ogrenci;
}

KITAP *kitapOlustur(char *ad, char *isbn, int adet) {
  KITAP *kitap = (KITAP*)malloc(sizeof(KITAP));
  strcpy(kitap->kitapAdi, ad);
  strcpy(kitap->ISBN, isbn);
  kitap->adet = adet;
  kitap->next = NULL;
  return kitap;
}

YAZAR *yazarOlustur(int yazarID, char *ad, char *soyad) {
  YAZAR *yazar = (YAZAR*)malloc(sizeof(YAZAR));
  yazar->yazarID = yazarID;
  strcpy(yazar->yazarAd, ad);
  strcpy(yazar->yazarSoyad, soyad);
  yazar->next = NULL;
  return yazar;
}

/* KITAPODUNC *kitapOduncOlustur(char *etiketNo, char *ogrID, int islemTipi, TARIH *islemTarihi) {
  KITAPODUNC *kitapodunc = (KITAPODUNC*)malloc(sizeof(KITAPODUNC));
  strcpy(kitapodunc->etiketNo, etiketNo);
  strcpy(kitapodunc->ogrID, ogrID);
  kitapodunc->islemTipi = islemTipi;
  kitapodunc->islemTarihi->gun = islemTarihi->gun;
  kitapodunc->islemTarihi->ay = islemTarihi->ay;
  kitapodunc->islemTarihi->yil = islemTarihi->yil;
  kitapodunc->next = NULL;
  return kitapodunc;
} */

OGRENCI *ogrenciOkuCSV(const char *dosyaAdi){
	FILE *fp = fopen(dosyaAdi, "r");
	if (fp == NULL){
		printf("Dosya a��lamad�.");
		return NULL;
	}
	
	OGRENCI *ogrenci = NULL;
	// OGRENCI *head;
	char satir[1024];
  	while (fgets(satir, 1024, fp)) {
    	char *ogrID = strtok(satir, ",");
	    char *ad = strtok(NULL, ",");
	    char *soyad = strtok(NULL, ",");
	    char *puan_str = strtok(NULL, ",");
	    int puan = atoi(puan_str);
	    
    	OGRENCI *yeniOgrenci = ogrenciOlustur(ogrID, ad, soyad, puan);
		ogrenciEkleCSV(&ogrenci, yeniOgrenci);
  }
  fclose(fp);
  return ogrenci;
}

KITAP *kitapOkuCSV(const char *dosyaAdi){
	FILE *fp = fopen(dosyaAdi, "r");
	if (fp == NULL){
		printf("Dosya a��lamad�.");
		return NULL;
	}
	
	KITAP *kitap = NULL;
	char satir[1024];
  	while (fgets(satir, 1024, fp)) {
    	char *ad = strtok(satir, ",");
	    char *isbn = strtok(NULL, ",");
	    char *adet_str = strtok(NULL, ",");
	    int adet = atoi(adet_str);
	    
    	KITAP *yeniKitap = kitapOlustur(ad, isbn, adet);
		kitapEkleCSV(&kitap, yeniKitap);
  }
  fclose(fp);
  return kitap;
}

YAZAR *yazarOkuCSV(const char *dosyaAdi){
	FILE *fp = fopen(dosyaAdi, "r");
	if (fp == NULL){
		printf("Dosya a��lamad�.");
		return NULL;
	}
	
	YAZAR *yazar = NULL;
	char satir[1024];
  	while (fgets(satir, 1024, fp)) {
    	char *yazarID_str = strtok(satir, ",");
    	int yazarID = atoi(yazarID_str);
	    char *ad = strtok(NULL, ",");
	    char *soyad = strtok(NULL, ",");
	    
    	YAZAR *yeniYazar = yazarOlustur(yazarID, ad, soyad);
		yazarEkleCSV(&yazar, yeniYazar);
  }
  fclose(fp);
  return yazar;
}

void kitapOduncOkuCSV(const char *dosyaAdi, KITAPODUNC *kitapOdunc, int kitapOduncSayi){

	FILE *fp = fopen(dosyaAdi, "r");
		if (fp == NULL){
			printf("Dosya a��lamad�.");
			// return 0;
		}
		char satir[1024];
		char *etiketNo, *ogrID, *islemTipi, *islemTarihi;
		
	  	int i;
	  	for(i=0; i<kitapOduncSayi; i++){
		
		if (fgets(satir, 1024, fp) != NULL){
			// return 0;
		}
  		
  		if (satir[strlen(satir) - 1] == '\n') {
		satir[strlen(satir) - 1] = '\0';
		}
  		
  		etiketNo = strtok(satir, ",");
		ogrID = strtok(NULL, ",");
		islemTipi = strtok(NULL, ",");
		islemTarihi = strtok(NULL, ",");
		
		
		strncpy((kitapOdunc)[i].etiketNo, etiketNo, sizeof(kitapOdunc->etiketNo));
		strncpy((kitapOdunc)[i].ogrID, ogrID, sizeof(kitapOdunc->ogrID));
		(kitapOdunc)[i].islemTipi = atoi(islemTipi);
		
		
		
		int gun, ay, yil;
		TARIH tarih;
		
		if (sscanf(islemTarihi, "%d.%d.%d", &gun, &ay, &yil) == 3) {
			tarih.gun = gun;
			tarih.ay = ay;
			tarih.yil = yil;
		} 
		else 
		{
		fprintf(stderr, "Tarih bilgisi hatal�: %s\n", islemTarihi);
		}
		(kitapOdunc)[i].islemTarihi = tarih;
		
		}
	fclose(fp);
	// return kitapOdunc;
	}	

KITAPYAZAR *kitapYazarOkuCSV(const char *dosyaAdi, KITAPYAZAR *kitapyazar, int satirSayisi){
    char karakter;
    
    FILE *fp = fopen(dosyaAdi, "r");
    if (fp == NULL) {
        printf("Dosya a��lamad�!\n");
        return NULL;
    }
    
    char satir[1024];
	char *ISBN;
	char *yazarID;
	
  	int i;
  	for(i=0; i<satirSayisi; i++){
	
	if (fgets(satir, 1024, fp) != NULL){
		// return 0;
	}
	
	if (satir[strlen(satir) - 1] == '\n') {
	satir[strlen(satir) - 1] = '\0';
	}
	
	ISBN = strtok(satir, ",");
	yazarID = strtok(NULL, ",");
	
	strncpy((kitapyazar)[i].ISBN, ISBN, sizeof(kitapyazar->ISBN));
	(kitapyazar)[i].yazarID = atoi(yazarID);
	}

    fclose(fp);
    return kitapyazar;
	
}

int kitapYazarSayisiOkuCSV(const char *dosyaAdi){
	KITAPYAZAR *kitapyazar;
	int satirSayisi = 0;
    char karakter;
    FILE *fp = fopen(dosyaAdi, "r");
    if (fp == NULL) {
        printf("Dosya a��lamad�!\n");
        return -1;
    }

    while ((karakter = fgetc(fp)) != EOF) {
        if (karakter == '\n') {
            satirSayisi++;
        }
    }
    int N = satirSayisi+1;
    return N;
}

int kitapYazarEslestir(KITAPYAZAR **head, KITAPYAZAR *kitapyazar, int N){
	KITAPYAZAR *yeniKitapYazar;
	yeniKitapYazar = (KITAPYAZAR*)malloc(sizeof(KITAPYAZAR));
	if (yeniKitapYazar == NULL){
	return -1;
	}
	printf("Eklemek istedi�iniz kitapISBN ve yazarID bilgilerini giriniz:\n");
	scanf("%s %d", yeniKitapYazar->ISBN, &yeniKitapYazar->yazarID);
	kitapyazar = (KITAPYAZAR*)realloc(yeniKitapYazar, (N+1)*sizeof(KITAPYAZAR));
	kitapyazar[N] = *yeniKitapYazar;
	(N)++;
	printf("\nKitap-Yazar ili�kisi eklendi.\n\n");
	return 0;
}

int ogrenciEkle(OGRENCI **head){
	OGRENCI *ogrenci;
	ogrenci = (OGRENCI*)malloc(sizeof(OGRENCI));
	if(ogrenci == NULL){
		*head = ogrenci;
		return -1;
	}
	printf("Eklemek istedi�iniz ��rencinin s�ras�yla ��renci no, isim, soyisim bilgilerini giriniz:\n");
	scanf("%s %s %s", ogrenci->ogrID, ogrenci->ad, ogrenci->soyad);
	ogrenci->puan = 100;
	OGRENCI *last = *head;
	while (last->next != NULL) {
	last = last->next;
	}
	if(strlen(ogrenci->ogrID) == 8){
		if(strcmp(last->ogrID, ogrenci->ogrID) == 0){
			printf("\nBu numara zaten kay�tl�.\n\n");
		}
		else{
			last->next = ogrenci;
			ogrenci->prev = last;
			printf("\n��renci Eklendi.\n\n");
			return 0;
		}
	}
	else if(strlen(ogrenci->ogrID) != 8){
		printf("\n��renci numaras� 8 haneli olmal�d�r.\n\n");
	}
	
}

int kitapEkle(KITAP **head){
	KITAP *kitap;
	kitap = (KITAP*)malloc(sizeof(KITAP));
	if(kitap == NULL){
		*head = kitap;
		return -1;
	}
	printf("Eklemek istedi�iniz kitab�n s�ras�yla ad�n�, isbn numaras�n�, ka� adet oldu�unu giriniz:\n");
	char *kitapAdi = (char*)malloc(sizeof(char));
	fflush(stdin);
	fgets(kitapAdi, 30, stdin);
	int uzunlukKitap = strlen(kitapAdi);
	if (kitapAdi[uzunlukKitap - 1] == '\n') {
	  kitapAdi[uzunlukKitap - 1] = '\0';
	}
	strcpy(kitap->kitapAdi, kitapAdi);
	scanf("%s %d", kitap->ISBN, &(kitap->adet));
	KITAP *last = *head;
	while (last->next != NULL) {
		last = last->next;
	}
	if(strlen(kitap->ISBN) == 13){
		if(strcmp(last->ISBN, kitap->ISBN) == 0){
			printf("\nBu numara zaten kay�tl�.\n\n");
		}
		else{
			last->next = kitap;
			printf("\nKitap Eklendi.\n\n");
			return 0;
		}
	}
	else if(strlen(kitap->ISBN) !=13){
		printf("\nISBN numaras� 8 haneli olmal�d�r.\n\n");
	}
	
}

int yazarEkle(YAZAR **head){
	YAZAR *yazar;
	yazar = (YAZAR*)malloc(sizeof(YAZAR));
	
	int yazarID;
	
	printf("Eklemek istedi�iniz yazar�n s�ras�yla isim, soyisim bilgilerini giriniz:\n");
	scanf("%s %s", yazar->yazarAd, yazar->yazarSoyad);
	
	if(yazar == NULL){
		*head = yazar;
		yazar->yazarID = 1;
		return -1;
	}
	else{
		YAZAR *last = *head;
		while (last->next != NULL) {
		last = last->next;
		}
		last->next = yazar;
		yazar->yazarID = last->yazarID + 1;
		
		while (last->next != NULL && last->yazarID == yazarID) {
      	yazarID++;
      	last = last->next;
    	}
	}
	printf("\nYazar Eklendi.\n\n");
	return 0;
}

int ogrenciSil(OGRENCI **head) {
	char *ogrID = (char*)malloc(sizeof(char));
	printf("Silmek istedi�iniz ��rencinin ��renci numaras�n� giriniz:\n");
	scanf("%s", ogrID);
	if(strlen(ogrID) == 8){
		OGRENCI *curr = *head;
	while (curr != NULL && strcmp(curr->ogrID, ogrID) != 0) {
		curr = curr->next;
	}
	if (curr == NULL) {
	    printf("Bu numarada bir ��renci yok!\n");
	    return 0;
	}
	if (curr == *head) {
	    *head = curr->next;
	}
	else {
	    curr->prev->next = curr->next;
			if (curr->next != NULL) {
	      		curr->next->prev = curr->prev;
	    	}
	}
	
	free(curr);
	printf("\n��renci Silindi.\n\n");
	return 1;
	}
	else{
		printf("\n��renci numaras�n� 8 haneli olmal�d�r.\n\n");
	}
}

int kitapSil(KITAP **head) {
	char *isbn = (char*)malloc(sizeof(char));
	printf("Silmek istedi�iniz kitab�n isbn numaras�n� giriniz:\n");
	scanf("%s", isbn);

	KITAP *curr = *head;
	while (curr != NULL && strcmp(curr->ISBN, isbn) != 0) {
		curr = curr->next;
	}
	if (curr == NULL) {
		printf("Yazar bulunamad�!\n");
		return 0;
	}
	if (curr == *head) {
		*head = curr->next;
	}
	else {
    	KITAP *temp = *head;
    	while (temp->next != curr) {
      		temp = temp->next;
    	}
    	temp->next = curr->next;
  	}
	free(curr);
	printf("\nKitap Silindi.\n\n");
	return 1;
}

int yazarSil(YAZAR **head) {
	char *yazarAd = (char*)malloc(sizeof(char));
	char *yazarSoyad = (char*)malloc(sizeof(char)); 
	printf("Silmek istedi�iniz yazar�n ad�n� ve soyad�n� giriniz:\n");
	scanf("%s %s", yazarAd, yazarSoyad);

	YAZAR *curr = *head;
	while (curr != NULL && (strcmp(curr->yazarAd, yazarAd) != 0 && strcmp(curr->yazarSoyad, yazarSoyad) != 0)) {
		curr = curr->next;
	}
	if (curr == NULL) {
		printf("Yazar bulunamad�!\n");
		return 0;
	}
	if (curr == *head) {
		*head = curr->next;
	}
	else {
    	YAZAR *temp = *head;
    	while (temp->next != curr) {
      		temp = temp->next;
    	}
    	temp->next = curr->next;
  	}
	free(curr);
	printf("\nYazar Silindi.\n\n");
	return 1;
}


void tumOgrenciListele(OGRENCI *head){
	OGRENCI *ogrenci = head;
	while(ogrenci != NULL){
		printf("��renci No: %s, ��renci Ad�: %s, ��renci Soyad�: %s, ��renci Puan�: %d\n", ogrenci->ogrID, ogrenci->ad, ogrenci->soyad, ogrenci->puan);
		ogrenci = ogrenci->next;
		printf("\n");
	}
}

void tumKitapListele(KITAP *head){
	KITAP *kitap = head;
	while(kitap != NULL){
		printf("Kitap Ad�: %s, Kitap ISBN: %s, Kitap Adeti: %d\n", kitap->kitapAdi, kitap->ISBN, kitap->adet);
		kitap = kitap->next;
		printf("\n");
	}
}

void tumYazarListele(YAZAR *head){
	YAZAR *yazar = head;
	while(yazar != NULL){
		printf("Yazar No: %d, Yazar Ad�: %s, Yazar Soyad�: %s\n", yazar->yazarID, yazar->yazarAd, yazar->yazarSoyad);
		yazar = yazar->next;
		printf("\n");
	}
}

void kitapOduncDurumlariListele(KITAPODUNC *kitapodunc, int kitapOduncSayi){
	int i;
	for (i = 0; i < kitapOduncSayi; i++){
		printf("Kitap Etiket No: %s, ��renci No: %s, ��lem Tipi: %d, ��lem Tarihi: %d.%d.%d\n", kitapodunc[i].etiketNo, kitapodunc[i].ogrID, kitapodunc[i].islemTipi, kitapodunc[i].islemTarihi.gun, kitapodunc[i].islemTarihi.ay, kitapodunc[i].islemTarihi.yil);
		printf("\n");
	}
} 

void kitapYazarEslesmeDurumlariListele(KITAPYAZAR *kitapyazar, int N){
	int i;
	for (i = 0; i < N; i++){
		printf("Kitap ISBN: %s, Yazar ID: %d\n", kitapyazar[i].ISBN, kitapyazar[i].yazarID);
		printf("\n");
	}
} 

void ogrenciAdiDegistir(OGRENCI** head, char* ogrID, char* yeniAd) {
  OGRENCI* p = *head;
  while (p != NULL) {
    if (strcmp(p->ogrID, ogrID) == 0) {
      strcpy(p->ad, yeniAd);
      return;
    }
    p = p->next;
  }
}

void kitapAdiDegistir(KITAP** head, char* ISBN, char* kitapAdi) {
  KITAP* p = *head;
  while (p != NULL) {
    if (strcmp(p->ISBN, ISBN) == 0) {
      strcpy(p->kitapAdi, kitapAdi);
      return;
    }
    p = p->next;
  }
}

void yazarAdiDegistir(YAZAR** head, int yazarID, char* yeniAd) {
  YAZAR* p = *head;
  while (p != NULL) {
    if (p->yazarID == yazarID) {
      strcpy(p->yazarAd, yeniAd);
      return;
    }
    p = p->next;
  }
}

void ogrenciSoyadiDegistir(OGRENCI** head, char* ogrID, char* yeniSoyad) {
  OGRENCI* p = *head;
  while (p != NULL) {
    if (strcmp(p->ogrID, ogrID) == 0) {
      strcpy(p->soyad, yeniSoyad);
      return;
    }
    p = p->next;
  }
}

void yazarSoyadiDegistir(YAZAR** head, int yazarID, char* yeniSoyad) {
  YAZAR* p = *head;
  while (p != NULL) {
    if (p->yazarID == yazarID) {
      strcpy(p->yazarSoyad, yeniSoyad);
      return;
    }
    p = p->next;
  }
}

void ogrenciNumarasiDegistir(OGRENCI** head, char* ad, char *soyad, char* yeniNumara) {
  OGRENCI* p = *head;
  while (p != NULL) {
    if (strcmp(p->ad, ad) == 0 && strcmp(p->soyad, soyad) == 0) {
      strcpy(p->ogrID, yeniNumara);
      return;
    }
    p = p->next;
  }
}

void isbnNumarasiDegistir(KITAP** head, char* kitapAdi, char *ISBN) {
  KITAP* p = *head;
  while (p != NULL) {
    if (strcmp(p->kitapAdi, kitapAdi) == 0) {
      strcpy(p->ISBN, ISBN);
      return;
    }
    p = p->next;
  }
}

void kitapAdetDegistir(KITAP** head, char* ISBN, int adet) {
  KITAP* p = *head;
  while (p != NULL) {
    if (strcmp(p->ISBN, ISBN) == 0) {
      p->adet = adet;
      return;
    }
    p = p->next;
  }
}

void kitapYazarDegistir(KITAPYAZAR *kitapyazar, char* ISBN, int yazarID, int N) {
	int i;
	for (i = 0; i < N; i++){
		if (strcmp(kitapyazar[i].ISBN, ISBN) == 0) {
	      kitapyazar[i].yazarID = yazarID;
	      return;
    	}	
	}

}

void ogrenciBilgisiGoruntuleID(OGRENCI *head, char *ogrID) {
	OGRENCI *ogrenci = head;
	while (ogrenci != NULL) {
	if (strcmp(ogrenci->ogrID, ogrID) == 0) {
	printf("��renci Ad�: %s\n", ogrenci->ad);
	printf("��renci Soyad�: %s\n", ogrenci->soyad);
	printf("��renci Numaras�: %s\n", ogrenci->ogrID);
	printf("��renci Puan�: %d\n", ogrenci->puan);
	printf("\n");
	return;
	}
	ogrenci = ogrenci->next;
	}
	printf("��renci bulunamad�.\n");
}

void ogrenciBilgisiGoruntuleAdSoyad(OGRENCI *head, char *ad, char *soyad) {
	OGRENCI *ogrenci = head;
	while (ogrenci != NULL) {
	if (strcmp(ogrenci->ad, ad) == 0 && strcmp(ogrenci->soyad, soyad) == 0) {
	printf("��renci Ad�: %s\n", ogrenci->ad);
	printf("��renci Soyad�: %s\n", ogrenci->soyad);
	printf("��renci Numaras�: %s\n", ogrenci->ogrID);
	printf("��renci Puan�: %d\n", ogrenci->puan);
	printf("\n");
	return;
	}
	ogrenci = ogrenci->next;
	}
	printf("��renci bulunamad�.\n");
}

void kitapBilgisiGoruntule(KITAP *head, char *kitapAdi){
	KITAP *kitap = head;
	while(kitap != NULL){
		if(strcmp(kitap->kitapAdi, kitapAdi) == 0){
		printf("Kitap Ad�: %s, Kitap ISBN: %s, Kitap Adeti: %d\n", kitap->kitapAdi, kitap->ISBN, kitap->adet);
		kitap = kitap->next;
		printf("\n");
		}
		else if(strcmp(kitap->kitapAdi, kitapAdi) != 0){
			kitap = kitap->next;
		}
		else{
			kitap = NULL;
		}
	}
}

void yazarBilgisiGoruntule(YAZAR *head, char *yazarAdi){
	YAZAR *yazar = head;
	while(yazar != NULL){
		if(strcmp(yazar->yazarAd, yazarAdi) == 0){
		printf("Yazar No: %d, Yazar Ad�: %s, Yazar Soyad�: %s\n", yazar->yazarID, yazar->yazarAd, yazar->yazarSoyad);
		yazar = yazar->next;
		printf("\n");
		}
		else if(strcmp(yazar->yazarAd, yazarAdi)){
			yazar = yazar->next;
		}
		else{
			yazar = NULL;
		}
	}
}

KITAPORNEK *kitapOrnekEkle(KITAPORNEK *head, char *etiketNo, char *durum) {
    KITAPORNEK *yeniOrnek = (KITAPORNEK*)malloc(sizeof(KITAPORNEK));
    strcpy(yeniOrnek->etiketNo, etiketNo);
    strcpy(yeniOrnek->durum, durum);
    yeniOrnek->next = NULL;

    if (head == NULL) {
        return yeniOrnek;
    }

    KITAPORNEK *curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = yeniOrnek;
    return head;
}

KITAPORNEK *kitapOrnekSil(KITAPORNEK *head, char *etiketNo) {
    if (head == NULL) {
        return NULL;
    }

    if (strcmp(head->etiketNo, etiketNo) == 0) {
        KITAPORNEK *silinecek = head;
        head = head->next;
        free(silinecek);
        return head;
    }

    KITAPORNEK *curr = head;
    while (curr->next != NULL) {
        if (strcmp(curr->next->etiketNo, etiketNo) == 0) {
            KITAPORNEK *silinecek = curr->next;
            curr->next = silinecek->next;
            free(silinecek);
            return head;
        }
        curr = curr->next;
    }

    return head;
}

void kitapOrnekAl(KITAP *head, char *etiketNo, char *ogrID) {
    if (head == NULL) {
        printf("Kitap bulunamadi.\n");
        return;
    }

    KITAP *curr = head;
    while (curr != NULL) {
        KITAPORNEK *tmp = curr->head;
        while (tmp != NULL) {
            if (strcmp(tmp->etiketNo, etiketNo) == 0) {
                if (strcmp(tmp->durum, "ODUNC") == 0) {
                    printf("Bu kitap zaten odunc alinmis. - %s\n\n", ogrID);
                }
                else {
                    strcpy(tmp->durum, ("ODUNC", ogrID));
                    printf("Kitap odunc alindi. - %s\n\n", ogrID);
                }
                return;
            }
            tmp = tmp->next;
        }
        curr = curr->next;
    }

    printf("Kitap bulunamadi.\n");
}

void kitapOrnekVer(KITAP *head, char *etiketNo, char *ogrID) {
    if (head == NULL) {
        printf("Kitap bulunamadi.\n\n");
        return;
    }

    KITAP *curr = head;
    while (curr != NULL) {
        KITAPORNEK *tmp = curr->head;
        while (tmp != NULL) {
            if (strcmp(tmp->etiketNo, etiketNo) == 0) {
                if (strcmp(tmp->durum, ("ODUNC", ogrID)) == 0) {
                    strcpy(tmp->durum, "RAFTA");
                    printf("Kitap iade edildi. - %s\n\n", ogrID);
                }
                else {
                    printf("Bu kitap zaten rafta. - %s\n\n", ogrID);
                }
                return;
            }
            tmp = tmp->next;
        }
        curr = curr->next;
    }

    printf("Kitap bulunamadi.\n");
}

void kitapOrnekleriniOlustur(KITAP *head) {
    KITAP *curr = head;
    while (curr != NULL) {
        for (int i = 0; i < curr->adet; i++) {
            char etiketNo[20];
            sprintf(etiketNo, "%s_%d", curr->ISBN, i+1);
            curr->head = kitapOrnekEkle(curr->head, etiketNo, "RAFTA");
        }
        curr = curr->next;
    }
}

void kitapOrnekleriniListele(KITAP *head) {
    KITAP *curr = head;
    while (curr != NULL) {
        printf("Kitap ad�: %s\n", curr->kitapAdi);
        printf("ISBN: %s\n", curr->ISBN);
        printf("Adet: %d\n", curr->adet);
        printf("\n");

        KITAPORNEK *kitapCurr = curr->head;
        while (kitapCurr != NULL) {
            printf("\tEtiket No: %s\n", kitapCurr->etiketNo);
            printf("\tDurum: %s\n", kitapCurr->durum);
            if (strcmp(kitapCurr->durum, "ODUNC") == 0) {
                // printf("\t��renci ID: %s\n", kitapCurr->ogrID);
            }
            printf("\n");
            kitapCurr = kitapCurr->next;
        }

        curr = curr->next;
    }
}

void kitapOrnekTeslimEdilmeyenListele(KITAP *head) {
    KITAP *curr = head;
    while (curr != NULL) {
        KITAPORNEK *kitapCurr = curr->head;
        while (kitapCurr != NULL) {
        	if(strcmp(kitapCurr->durum, "RAFTA") != 0){
        	printf("\tEtiket No: %s\n", kitapCurr->etiketNo);
            printf("\tDurum: %s\n", kitapCurr->durum);
            printf("\n");
            kitapCurr = kitapCurr->next;
			}
			else if(strcmp(kitapCurr->durum, "RAFTA") == 0){
				printf("�d�n� alanlar getirildi. Hepsi raftaysa bo�tur.\n\n");
				return;
			}
        }
        curr = curr->next;
    }
}

void dosyayaYazKitap(const char *dosyaAdi, KITAP *head) {
	KITAP *gezici = head;
	FILE *fp = fopen(dosyaAdi, "w");
	if (fp == NULL) {
	printf("Dosya a��lamad�!\n");
	return;
	}
	while (gezici != NULL) {
	fprintf(fp, "%s,%s,%d\n", gezici->kitapAdi, gezici->ISBN, gezici->adet);
	gezici = gezici->next;
	}
	fclose(fp);
}

void dosyayaYazYazar(const char *dosyaAdi, YAZAR *head) {
	YAZAR *gezici = head;
	FILE *fp = fopen(dosyaAdi, "w");
	if (fp == NULL) {
	printf("Dosya a��lamad�!\n");
	return;
	}
	while (gezici != NULL) {
	fprintf(fp, "%d,%s,%s\n", gezici->yazarID, gezici->yazarAd, gezici->yazarSoyad);
	gezici = gezici->next;
	}
	fclose(fp);
}

void dosyayaYazOgrenci(const char *dosyaAdi, OGRENCI *head) {
	OGRENCI *gezici = head;
	FILE *fp = fopen(dosyaAdi, "w");
	if (fp == NULL) {
	printf("Dosya a��lamad�!\n");
	return;
	}
	while (gezici != NULL) {
	fprintf(fp, "%s,%s,%d\n", gezici->ogrID, gezici->ad, gezici->soyad, gezici->puan);
	gezici = gezici->next;
	}
	fclose(fp);
}

void dosyayaYazKitapOdunc(const char *dosyaAdi, KITAPODUNC *kitapodunc, int kitapOduncSayi) {
	KITAPODUNC *gezici = kitapodunc;
	FILE *fp = fopen(dosyaAdi, "w");
	if (fp == NULL) {
	printf("Dosya a��lamad�!\n");
	return;
	}
	int i;
	for (i = 0; i < kitapOduncSayi; i++){
		fprintf(fp, "%s,%s,%d,%d.%d.%d\n", kitapodunc[i].etiketNo, kitapodunc[i].ogrID, kitapodunc[i].islemTipi, kitapodunc[i].islemTarihi.gun, kitapodunc[i].islemTarihi.ay, kitapodunc[i].islemTarihi.yil);
	}
	fclose(fp);
}

void dosyayaYazKitapYazar(const char *dosyaAdi, KITAPYAZAR *kitapyazar, int N) {
	KITAPYAZAR *gezici = kitapyazar;
	FILE *fp = fopen(dosyaAdi, "w");
	if (fp == NULL) {
	printf("Dosya a��lamad�!\n");
	return;
	}
	int i;
	for (i = 0; i < N; i++){
		fprintf(fp, "%s,%d\n", kitapyazar[i].ISBN, kitapyazar[i].yazarID);
	}
	fclose(fp);
}

void kitapOduncAlEkle(const char *dosyaAdi, KITAPODUNC *kitapodunc, int kitapOduncSayi, char *etiketNo, char *ogrID, int p, int g, int a, int s){
	KITAPODUNC *gezici = kitapodunc;
	FILE *fp = fopen(dosyaAdi, "w");
	if (fp == NULL) {
	printf("Dosya a��lamad�!\n");
	return;
	}
	int i;
	for (i = 0; i < kitapOduncSayi; i++){
		fprintf(fp, "%s,%s,%d,%d.%d.%d\n", kitapodunc[i].etiketNo, kitapodunc[i].ogrID, kitapodunc[i].islemTipi, kitapodunc[i].islemTarihi.gun, kitapodunc[i].islemTarihi.ay, kitapodunc[i].islemTarihi.yil);
		printf("\n");
	}
	
	fprintf(fp, "%s,%s,%d,%d.%d.%d\n", etiketNo, ogrID, p, g, a, s);
} 

void ogrenciGecTeslimCezaVer(OGRENCI** head, char* ogrID, int d, int m, int y, int g, int a, int s) {
    int zamanFarki;
    struct tm oduncZamani = {0};
    oduncZamani.tm_year = s - 1900;
    oduncZamani.tm_mon = a - 1;
    oduncZamani.tm_mday = g;

    struct tm iadeZamani = {0};
    iadeZamani.tm_year = y - 1900;
    iadeZamani.tm_mon = m - 1;
    iadeZamani.tm_mday = d;

    zamanFarki = ((iadeZamani.tm_year - oduncZamani.tm_year) * 365) + ((iadeZamani.tm_mon - oduncZamani.tm_mon) * 30) + (iadeZamani.tm_mday - oduncZamani.tm_mday);
    printf("Zaman Fark�: %d\n\n", zamanFarki);
    degerDondurG(zamanFarki);
    OGRENCI* p = *head;
    while (p != NULL) {
        if ((strcmp(p->ogrID, ogrID) == 0) && zamanFarki > 15) {
            p->puan = (p->puan - 15);
            return;
        }
        p = p->next;
    }
}

void cezaliOgrenciListele(OGRENCI *head){
	OGRENCI *ogrenci = head;
	while(ogrenci != NULL){
		ogrenci = ogrenci->next;
		if(ogrenci->puan != 100){
			printf("��renci No: %s, ��renci Ad�: %s, ��renci Soyad�: %s, ��renci Puan�: %d\n", ogrenci->ogrID, ogrenci->ad, ogrenci->soyad, ogrenci->puan);
			ogrenci = ogrenci->next;
			printf("\n");
		}
		else{
			printf("Cezal� ��renci yok.\n\n");
			return;
		}
	}
}

void zamanindaTeslimEdilmeyenKitaplariListele(KITAP *head, int zamanFarki) {
    KITAP *curr = head;
    while (curr != NULL) {
        KITAPORNEK *kitapCurr = curr->head;
        while (kitapCurr != NULL) {
        	if(strcmp(kitapCurr->durum, "RAFTA") != 0){
        	printf("\tEtiket No: %s\n", kitapCurr->etiketNo);
            printf("\tDurum: %s\n", kitapCurr->durum);
            if (zamanFarki > 15){
            	printf("Zaman Fark�ndan Cezal�: %d", zamanFarki);
			}
            printf("\n");
            kitapCurr = kitapCurr->next;
			}
			else if(strcmp(kitapCurr->durum, "RAFTA") == 0){
				printf("\n�d�n� alanlar getirildi. Hepsi raftaysa bo�tur.\n\n");
				return;
			}
        }
        curr = curr->next;
    }
}


int degerDondurG(int deger){
	int g = deger;
	return g;
}
	int degerDondurA(int deger){
	int a = deger;
	return a;
}
	int degerDondurS(int deger){
	int s = deger;
	return s;
}	


int main(){
	setlocale(LC_ALL, "Turkish");
	
	int degerDondurG(int deger);
	int degerDondurA(int deger);
	int degerDondurS(int deger);
	
	void kitapOduncAlEkle(const char *dosyaAdi, KITAPODUNC *kitapodunc, int kitapOduncSayi, char *etiketNo, char *ogrID, int p, int d, int m, int y);
	void ogrenciGecTeslimCezaVer(OGRENCI** head, char* ogrID, int d, int m, int y, int g, int a, int s);
	
	int menu = 0;
	int ogrenciMenu = 0;
	int kitapMenu = 0;
	int yazarMenu = 0;
	int kitapOduncSayi = 5;
	int ogrenciGuncelleMenu = 0;
	int kitapGuncelleMenu = 0;
	int yazarGuncelleMenu = 0;
	int ogrenciBilgisiGoruntuleMenu = 0;
	int kitapBilgisiGoruntuleMenu = 0;
	int yazarBilgisiGoruntuleMenu = 0;
	int kitapOduncIadeMenu = 0;
	int N = 0;
	KITAPODUNC *kitapodunc;
	KITAPYAZAR *kitapyazar;
	
	OGRENCI *head = NULL;
	OGRENCI *ogrenci = ogrenciOkuCSV("Ogrenciler.csv");
	
	KITAP *ilk = NULL;
	KITAP *kitap = kitapOkuCSV("Kitaplar.csv");
	
	YAZAR *bir = NULL;
	YAZAR *yazar = yazarOkuCSV("Yazarlar.csv");
	
	KITAPYAZAR *first = NULL;
	
	
	// kitapyazar = kitapYazarOkuCSV("KitapYazar.csv");
	N = kitapYazarSayisiOkuCSV("KitapYazar.csv");
	kitapyazar = (KITAPYAZAR*)malloc(N*sizeof(KITAPYAZAR));
	kitapYazarOkuCSV("KitapYazar.csv", kitapyazar, N);
	
	kitapodunc = (KITAPODUNC*)malloc((kitapOduncSayi)*sizeof(KITAPODUNC));
	kitapOduncOkuCSV("KitapOdunc.csv", kitapodunc, kitapOduncSayi);
	
	kitapOrnekleriniOlustur(kitap);
	
	int g = degerDondurG(g);
	int a = degerDondurA(a);
	int s = degerDondurS(s);
	int zamanFarki = degerDondurG(zamanFarki);
	
	
	while(menu != 15){
		printf("-----------------��RENC� ��LEMLER�--------------------\n");
		printf("1- ��renci Ekle, Sil, G�ncelle \n2- ��renci Bilgisi G�r�nt�leme \n3- Kitap Teslim Etmemi� ��rencileri Listele \n4- Cezal� ��rencileri Listele \n5- T�m ��rencileri Listele \n6- Kitap �d�n� Al/Teslim Et \n");
		printf("\n");
		printf("-----------------K�TAP ��LEMLER�--------------------\n");
		printf("7- Kitap Ekle, Sil, G�ncelle \n8- Kitap Bilgisi G�r�nt�leme \n9- Raftaki Kitaplar� Listele \n10- Zaman�nda Teslim Edilmeyen Kitaplar� Listele \n11- Kitap-Yazar E�le�tir \n12- Kitab�n Yazar�n� G�ncelle \n");
		printf("\n");
		printf("-----------------YAZAR ��LEMLER�--------------------\n");
		printf("13- Yazar Ekle, Sil, G�ncelle\n14- Yazar Bilgisi G�r�nt�leme \n");
		printf("\n");
		printf("-----------------�IKI�--------------------\n");
		printf("15- �IK VE KAYDET\n\n");
		
		scanf("%d", &menu);
		switch(menu){
			case 1:
				printf("\nNe yapmak istiyorsunuz?\n1- ��renci Ekle \n2- ��renci Sil \n3- ��renci G�ncelle\n\n");
				scanf("%d", &ogrenciMenu);
				if(ogrenciMenu == 1){
					ogrenciEkle(&ogrenci);
				}
				else if(ogrenciMenu == 2){
					ogrenciSil(&ogrenci);
				}
				else if(ogrenciMenu == 3){
					printf("\nNeyi g�ncellemek istiyorsunuz?\n1- ��renci Numaras�\n2- ��renci Ad�\n3- ��renci Soyad�\n\n");
					scanf("%d", &ogrenciGuncelleMenu);
					if(ogrenciGuncelleMenu == 1){
						char *ogrID = (char*)malloc(sizeof(char));
						char *ad = (char*)malloc(sizeof(char));
						char *soyad = (char*)malloc(sizeof(char));
						printf("\nNumras�n� de�i�tirmek istedi�iniz ��rencinin ad�n� ve soyad�n� girin:\n\n");
						scanf("%s %s", ad, soyad);
						printf("\nYeni numaras�n� girin:\n\n");
						scanf("%s", ogrID);
						if(strlen(ogrID) == 8){
							ogrenciNumarasiDegistir(&ogrenci, ad, soyad, ogrID);
							printf("\n��rencinin numaras� de�i�tirildi.\n\n");
						}
						else{
							printf("\n��rencinin numaras� 8 haneli olmal�d�r.\n\n");
						}
					}
					else if(ogrenciGuncelleMenu == 2){
						char *ogrID = (char*)malloc(sizeof(char));
						char *ad = (char*)malloc(sizeof(char));
						printf("\nAd�n� de�i�tirmek istedi�iniz ��rencinin numaras�n� girin:\n\n");
						scanf("%s", ogrID);
						if(strlen(ogrID) == 8){
							printf("\nYeni ad�n� girin:\n\n");
							scanf("%s", ad);
							ogrenciAdiDegistir(&ogrenci, ogrID, ad);
							printf("\n��rencinin ad� de�i�tirildi.\n\n");
						}
						else{
							printf("\n��rencinin numaras� 8 haneli olmal�d�r.\n\n");
						}
					}
					else if(ogrenciGuncelleMenu == 3){
						char *ogrID = (char*)malloc(sizeof(char));
						char *soyad = (char*)malloc(sizeof(char));
						printf("\nSoyad�n� de�i�tirmek istedi�iniz ��rencinin numaras�n� girin:\n\n");
						scanf("%s", ogrID);
						if(strlen(ogrID) == 8){
							printf("\nYeni ad�n� girin:\n\n");
							scanf("%s", soyad);
							ogrenciSoyadiDegistir(&ogrenci, ogrID, soyad);
							printf("\n��rencinin soyad� de�i�tirildi.\n\n");
						}
						else{
							printf("\n��rencinin numaras� 8 haneli olmal�d�r.\n\n");
						}
					}
				}
				else{
					menu = 1;
				}
				break;
			case 2:
				printf("\n1-ID bilgisi girerek bilgileri getir.\n2-Ad soyad bilgisi girerek bilgileri getir.\n\n");
				scanf("%d", &ogrenciBilgisiGoruntuleMenu);
				if(ogrenciBilgisiGoruntuleMenu == 1){
						char *ogrID = (char*)malloc(sizeof(char));
						printf("\nBilgileri istenen ��rencinin numaras�n� girin:\n\n");
						scanf("%s", ogrID);
						if(strlen(ogrID) == 8){
							ogrenciBilgisiGoruntuleID(ogrenci, ogrID);
							printf("\n��rencinin bilgileri getirildi.\n\n");
						}
						else{
							printf("\n��rencinin numaras� 8 haneli olmal�d�r.\n\n");
						}
					}
					else if(ogrenciBilgisiGoruntuleMenu == 2){
						char *soyad = (char*)malloc(sizeof(char));
						char *ad = (char*)malloc(sizeof(char));
						printf("\nBilgileri istenen ��rencinin ad�n� ve soyad�n� girin:\n\n");
						scanf("%s %s", ad, soyad);
						ogrenciBilgisiGoruntuleAdSoyad(ogrenci, ad, soyad);
					}
				break;
			case 3:
				kitapOrnekTeslimEdilmeyenListele(kitap);
				break;
			case 4:
				cezaliOgrenciListele(ogrenci);
				break;
			case 5:
				printf("\n-----------------T�M ��RENC�LER�N DURUMU--------------------\n");
				tumOgrenciListele(ogrenci);
				break;
			case 6:
				printf("\n-----------------T�M K�TAPLARIN �D�N� DURUMU--------------------\n");
				kitapOduncDurumlariListele(kitapodunc, kitapOduncSayi);
				printf("\n1-�d�n� Al\n2-�ade Et\n\n");
				scanf("%d", &kitapOduncIadeMenu);
				if(kitapOduncIadeMenu == 1){
					printf("�d�n� alacak ��rencinin numaras�n� girin:");
					char *ogrID = (char*)malloc(sizeof(char));
					scanf("%s", ogrID);
					printf("�d�n� al�nacak kitab�n etiket numaras�n� girin:");
					char *etiketNo = (char*)malloc(sizeof(char));
					scanf("%s", etiketNo);
					int g, a, s;
					printf("��lem yap�lan g�n� giriniz:\n");
					scanf("%d", &g);
					printf("��lem yap�lan ay� giriniz:\n");
					scanf("%d", &a);
					printf("��lem yap�lan y�l� giriniz:\n");
					scanf("%d", &s);
					int p = 0;
					kitapOrnekAl(kitap, etiketNo, ogrID);
					degerDondurG(g);
					degerDondurA(a);
					degerDondurS(s);
					// kitapOduncAlEkle("KitapOdunc_Yeni.csv", kitapodunc, kitapOduncSayi, etiketNo, ogrID, p, g, a, s);
				}
				else if(kitapOduncIadeMenu == 2){
					printf("�ade edecek ��rencinin numaras�n� girin:");
					char *ogrID = (char*)malloc(sizeof(char));
					scanf("%s", ogrID);
					printf("�ade edilecek kitab�n etiket numaras�n� girin:");
					char *etiketNo = (char*)malloc(sizeof(char));
					scanf("%s", etiketNo);
					int d, m, y;
					printf("�ade edilen tarihi g�n, ay, y�l format�nda giriniz:\n");
					scanf("%d %d %d", &d, &m, &y);
					kitapOrnekVer(kitap, etiketNo, ogrID);
					ogrenciGecTeslimCezaVer(&ogrenci, ogrID, d, m, y, g, a, s);
				}
				
				break;
			case 7:
				printf("\nNe yapmak istiyorsunuz?\n1- Kitap Ekle \n2- Kitap Sil \n3- Kitap G�ncelle\n4- T�m Kitaplar� Listele\n\n");
				scanf("%d", &kitapMenu);
				if(kitapMenu == 1){
					kitapEkle(&kitap);
				}
				else if(kitapMenu == 2){
					kitapSil(&kitap);
				}
				else if(kitapMenu == 3){
					printf("\nNeyi g�ncellemek istiyorsunuz?\n1- ISBN Numaras�\n2- Kitap Ad�\n3- Kitap Adeti\n\n");
					scanf("%d", &kitapGuncelleMenu);
					if(kitapGuncelleMenu == 1){
						char *ISBN = (char*)malloc(sizeof(char));
						char *kitapAdiB = (char*)malloc(sizeof(char));
						printf("\nISBN numaras�n� de�i�tirmek istedi�iniz kitab�n ad�n� girin:\n\n");
						
						fflush(stdin);
						fgets(kitapAdiB, 30, stdin);
						int uzunlukKitapAdi = strlen(kitapAdiB);
						if (kitapAdiB[uzunlukKitapAdi - 1] == '\n') {
						  kitapAdiB[uzunlukKitapAdi - 1] = '\0';
						}
						// strcpy(kitap->kitapAdi, kitapAdiB);
						printf("\nYeni ISBN numaras�n� girin:\n\n");
						scanf("%s", ISBN);
						if(strlen(ISBN) == 13){
							isbnNumarasiDegistir(&kitap, kitapAdiB, ISBN);
							printf("\nISBN numaras� de�i�tirildi.\n\n");
						}
						else{
							printf("\nISBN numaras� 13 haneli olmal�d�r.\n\n");
						}
					}
					else if(kitapGuncelleMenu == 2){
						char *ISBN = (char*)malloc(sizeof(char));
						char *kitapAdi = (char*)malloc(sizeof(char));
						printf("\nAd�n� de�i�tirmek istedi�iniz kitab�n ISBN numaras�n� girin:\n\n");
						scanf("%s", ISBN);
						if(strlen(ISBN) == 13){
							printf("\nYeni ad�n� girin:\n\n");
							scanf("%s", kitapAdi);
							kitapAdiDegistir(&kitap, ISBN, kitapAdi);
							printf("\nKitab�n ad� de�i�tirildi.\n\n");
						}
						else{
							printf("\nISBN numaras� 13 haneli olmal�d�r.\n\n");
						}
					}
					else if(kitapGuncelleMenu == 3){
						char *ISBN = (char*)malloc(sizeof(char));
						int adet;
						printf("\nAdetini de�i�tirmek istedi�iniz kitab�n ISBN numras�n� girin:\n\n");
						scanf("%s", ISBN);
						if(strlen(ISBN) == 13){
							printf("\nYeni adeti girin:\n\n");
							scanf("%d", &adet);
							kitapAdetDegistir(&kitap, ISBN, adet);
							printf("\nKitap adeti de�i�tirildi.\n\n");
						}
						else{
							printf("\nISBN numaras� 13 haneli olmal�d�r.\n\n");
						}
					}
				}
				else if(kitapMenu == 4){
					printf("\n-----------------T�M K�TAPLARIN DURUMU--------------------\n");
					tumKitapListele(kitap);
				}
				break;
			case 8:
				printf("\nBilgileri istenen kitab�n ad�n� giriniz.\n\n");
				char *kitapAdi = (char*)malloc(sizeof(char));
				fflush(stdin);
				fgets(kitapAdi, 30, stdin);
				int uzunluk = strlen(kitapAdi);
				if (kitapAdi[uzunluk - 1] == '\n') {
				  kitapAdi[uzunluk - 1] = '\0';
				}
				kitapBilgisiGoruntule(kitap, kitapAdi);
				break;
			case 9:
				kitapOrnekleriniListele(kitap);
				break;
			case 10:
				zamanindaTeslimEdilmeyenKitaplariListele(kitap, zamanFarki);
				break;
			case 11:
				printf("\n-----------------T�M K�TAP-YAZAR E�LE�ME DURUMU--------------------\n");
				kitapYazarEslesmeDurumlariListele(kitapyazar, N);
				printf("\n");
				kitapYazarEslestir(&kitapyazar, kitapyazar, N+1);
				break;
			case 12:
				printf("\n-----------------K�TAP-YAZAR DE���T�RME MEN�S� --------------------\n");
				printf("\n");
				printf("\n-----------------T�M K�TAP-YAZAR E�LE�ME DURUMU--------------------\n");
				kitapYazarEslesmeDurumlariListele(kitapyazar, N);
				char *ISBN = (char*)malloc(sizeof(char));
				int yazarID;
				printf("\nYazar�n� de�i�tirmek istedi�iniz kitab�n ISBN numras�n� girin:\n\n");
				scanf("%s", ISBN);
				if(strlen(ISBN) == 13){
					printf("\nYeni yazar numaras�n� girin:\n\n");
					scanf("%d", &yazarID);
					kitapYazarDegistir(kitapyazar, ISBN, yazarID, N);
					printf("\nKitap yazar� de�i�tirildi.\n\n");
				}
				else{
					printf("\nISBN numaras� 13 haneli olmal�d�r.\n\n");
				}
				break;
			case 13:
				printf("\nNe yapmak istiyorsunuz?\n1- Yazar Ekle \n2- Yazar Sil \n3- Yazar G�ncelle\n4- T�m Yazarlar� Listele\n\n");
				scanf("%d", &yazarMenu);
				if(yazarMenu == 1){
					yazarEkle(&yazar);
				}
				else if(yazarMenu == 2){
					yazarSil(&yazar);
				}
				else if(yazarMenu == 3){
					printf("\nNeyi g�ncellemek istiyorsunuz?\n1- Yazar Ad�\n2- Yazar Soyad�\n\n");
					scanf("%d", &yazarGuncelleMenu);
					if(yazarGuncelleMenu == 1){
						int yazarID;
						char *ad = (char*)malloc(sizeof(char));
						printf("\nAd�n� de�i�tirmek istedi�iniz yazar�n numaras�n� girin:\n\n");
						scanf("%d", &yazarID);
						printf("\nYeni ad�n� girin:\n\n");
						scanf("%s", ad);
						yazarAdiDegistir(&yazar, yazarID, ad);
						printf("\nYazar�n ad� de�i�tirildi.\n\n");
					}
					else if(yazarGuncelleMenu == 2){
						int yazarID;;
						char *soyad = (char*)malloc(sizeof(char));
						printf("\nSoyad�n� de�i�tirmek istedi�iniz yazar�n numaras�n� girin:\n\n");
						scanf("%d", &yazarID);
						printf("\nYeni ad�n� girin:\n\n");
						scanf("%s", soyad);
						yazarSoyadiDegistir(&yazar, yazarID, soyad);
						printf("\nYazar�n soyad� de�i�tirildi.\n\n");
					}
					else{
						printf("Ge�erli bir numara girin.");
					}
				}
				else if(yazarMenu == 4){
					printf("\n-----------------T�M YAZARLARIN DURUMU--------------------\n");
					tumYazarListele(yazar);
				}
				break;
			case 14:
				printf("\nBilgileri istenen yazar�n ad�n� giriniz.\n\n");
				char *yazarAdi = (char*)malloc(sizeof(char));
				fflush(stdin);
				fgets(yazarAdi, 30, stdin);
				int uzunlukYazar = strlen(yazarAdi);
				if (yazarAdi[uzunlukYazar - 1] == '\n') {
				  yazarAdi[uzunlukYazar - 1] = '\0';
				}
				yazarBilgisiGoruntule(yazar, yazarAdi);
				break;
			case 15:
				menu = 15;
				dosyayaYazKitap("Kitaplar_Yeni.csv", kitap);
				dosyayaYazYazar("Yazarlar_Yeni.csv", yazar);
				dosyayaYazOgrenci("��renciler_Yeni.csv", ogrenci);
				dosyayaYazKitapOdunc("KitapOdunc_Yeni.csv", kitapodunc, kitapOduncSayi);
				dosyayaYazKitapYazar("KitapYazar_Yeni.csv", kitapyazar, N);
				free(kitapodunc);
				free(kitapyazar);
		}
	}
	
	
}
