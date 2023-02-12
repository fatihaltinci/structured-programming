#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct DERS{
    char dersKodu[255];
    char dersAdi[255];
    int toplamKredi;
    int dersKontenjani;
    int ogrenciSayisi;
    int *ogrenciNolar;
    struct DERS *sonrakiDers;
}ders;

typedef struct OGRENCI{
    int ogrenciNo;
    char ogrenciAdi[255];
    char ogrenciSoyadi[255];
    int kayDersSayisi;
    int kayToplamKredi;
    struct OGRENCI *oncekiOgrenci;
    struct OGRENCI *sonrakiOgrenci;
}ogrenci;

typedef struct OGRENCIDERS{
    int ID;
    char dersKodu[255];
    int ogrenciNo;
    char tarih[11];
    char dersDurumu[255];
    struct OGRENCIDERS *oncekiOgrenciDers;
    struct OGRENCIDERS *sonrakiOgrenciDers;
}ogrenciders;

void derseKayitliOgrenciler(ders *ders1){
    while(ders1!=NULL){
        printf("Kodu: %s, Ad�: %s, Kredisi: %d, Kontenjan: %d\n",ders1->dersKodu,ders1->dersAdi,ders1->toplamKredi,ders1->dersKontenjani);
        for (int i=0; i<ders1->ogrenciSayisi+1; i++) {
            printf("��renci Nolar�: %d\n", *(ders1->ogrenciNolar+i));
        }
        ders1=ders1->sonrakiDers;
    }
}

void dersleriYazdir(ders *ders1){

    while(ders1!=NULL){
        printf("Kodu: %s, Ad�: %s, Kredisi: %d, Kontenjan: %d\n",ders1->dersKodu,ders1->dersAdi,ders1->toplamKredi,ders1->dersKontenjani);
        ders1=ders1->sonrakiDers;
    }

}

void ogrencileriYazdir(ogrenci *ogrenci1){
    while(ogrenci1 != NULL){
        printf("��renci No: %d, Ad�: %s, Soyad�: %s, Toplam Kredi: %d, Ders Say�s�: %d\n",ogrenci1->ogrenciNo,ogrenci1->ogrenciAdi,ogrenci1->ogrenciSoyadi,ogrenci1->kayToplamKredi,ogrenci1->kayDersSayisi);
        ogrenci1 = ogrenci1->sonrakiOgrenci;
    }
}

void ogrenciDersYazdir(ogrenciders *ogrenciders1){
    while (ogrenciders1 != NULL){
        printf("ID: %d, Ders Kodu: %s, ��renci No: %d, Tarih: %s, Ders Durumu: %s",ogrenciders1->ID,ogrenciders1->dersKodu,ogrenciders1->ogrenciNo,ogrenciders1->tarih,ogrenciders1->dersDurumu);
        ogrenciders1 = ogrenciders1->sonrakiOgrenciDers;
    }
}
void dersVerileriYerlestir(ders *gelenDers,char buff[255],char delimiter[3]){
    char *token;

    token=strtok(buff,delimiter);
    strcpy(gelenDers->dersKodu,token);

    token=strtok(NULL,delimiter);
    strcpy(gelenDers->dersAdi,token);

    token=strtok(NULL,delimiter);
    gelenDers->toplamKredi=atoi(token);

    token=strtok(NULL,delimiter);
    //token[strlen(token)-1]='\0';
    gelenDers->dersKontenjani=atoi(token);
}

ders* dersleriOku(char dosyaAdi[20],char delimiter[3]){
    FILE *fp=fopen(dosyaAdi,"r");
    char buf[255];

    ders *anaDers=(ders*)malloc(sizeof(ders));
    ders *oncekiDers;
    ders *anlikDers;
    if(fgets(buf,255,fp)!=NULL){
        //printf("%s",buf);
        dersVerileriYerlestir(anaDers,buf,delimiter);
        anaDers->ogrenciSayisi = 0;
        anaDers->ogrenciNolar = (int *) malloc(anaDers->dersKontenjani * sizeof(int));
    }
    oncekiDers=anaDers;

    while(fgets(buf,255,fp)!=NULL){
        //printf("%s",buf);
        anlikDers =(ders*)malloc(sizeof(ders));
        anlikDers->ogrenciSayisi = 0;
        anlikDers->ogrenciNolar = (int *) malloc(anlikDers->dersKontenjani * sizeof(int));
        oncekiDers->sonrakiDers=anlikDers;
        dersVerileriYerlestir(anlikDers,buf,delimiter);
        oncekiDers = anlikDers;
    }
    oncekiDers->sonrakiDers=NULL;

    fclose(fp);
    return anaDers;
}
void ogrenciVerileriYerlestir(ogrenci *gelenOgrenci,char buff[255],char delimiter[3]){
    char *token;

    token=strtok(buff,delimiter);
    gelenOgrenci->ogrenciNo=atoi(token);

    token=strtok(NULL,delimiter);
    strcpy(gelenOgrenci->ogrenciAdi,token);

    token=strtok(NULL,delimiter);
    strcpy(gelenOgrenci->ogrenciSoyadi,token);

    token=strtok(NULL,delimiter);
    gelenOgrenci->kayToplamKredi=atoi(token);

    token=strtok(NULL,delimiter);
    gelenOgrenci->kayDersSayisi=atoi(token);
}

ogrenci* ogrencileriOku(char dosyaAdi[20],char delimiter[3]){
    FILE *fp=fopen(dosyaAdi,"r");
    char buf[255];

    ogrenci *anaOgrenci = (ogrenci*)malloc(sizeof(ogrenci));
    ogrenci *anlikOgrenci;
    ogrenci *oncekiOgrenci;

    if(fgets(buf,255,fp)!=NULL){
        ogrenciVerileriYerlestir(anaOgrenci,buf,delimiter);
    }
    anaOgrenci->oncekiOgrenci = NULL;
    oncekiOgrenci = anaOgrenci;

    while(fgets(buf,255,fp)!=NULL){
        anlikOgrenci = (ogrenci*)malloc(sizeof(ogrenci));
        anlikOgrenci->oncekiOgrenci = oncekiOgrenci;
        oncekiOgrenci->sonrakiOgrenci = anlikOgrenci;
        ogrenciVerileriYerlestir(anlikOgrenci,buf,delimiter);
        oncekiOgrenci = anlikOgrenci;
    }
    oncekiOgrenci->sonrakiOgrenci = NULL;
    fclose(fp);
    return anaOgrenci;

}
void ogrenciDersVerileriYerlestir(ogrenciders *gelenOgrenciDers,char buff[255],char delimiter[3]){
    char *token;

    token=strtok(buff,delimiter);
    gelenOgrenciDers->ID=atoi(token);

    token=strtok(NULL,delimiter);
    strcpy(gelenOgrenciDers->dersKodu,token);

    token=strtok(NULL,delimiter);
    gelenOgrenciDers->ogrenciNo=atoi(token);

    token=strtok(NULL,delimiter);
    //token[strlen(token)]='\0';
    strcpy(gelenOgrenciDers->tarih,token);

    token=strtok(NULL,delimiter);
    strcpy(gelenOgrenciDers->dersDurumu,token);
}
void ogrenciDersKayit(ogrenciders* anaOgrenciDers, ders* ders1){
    struct OGRENCIDERS *odBayrak;
    struct DERS *dersBayrak;
    dersBayrak = ders1;
    odBayrak = anaOgrenciDers;
    const char kayitli[7] = "kayitli";
    while(ders1->sonrakiDers != NULL){
        while (anaOgrenciDers->sonrakiOgrenciDers != NULL){
            if(strcmp(anaOgrenciDers->dersKodu,ders1->dersKodu) == 0){
                *(ders1->ogrenciNolar+(ders1->ogrenciSayisi)) = anaOgrenciDers->ogrenciNo;
                ders1->ogrenciSayisi = ders1->ogrenciSayisi + 1;
            }
            anaOgrenciDers = anaOgrenciDers->sonrakiOgrenciDers;
        }
        ders1->ogrenciSayisi = ders1->ogrenciSayisi - 1;
        anaOgrenciDers = odBayrak;
        ders1 = ders1->sonrakiDers;
    }
    //derseKayitliOgrenciler(dersBayrak);
}
ogrenciders* ogrenciDersOku(char dosyaAdi[20],char delimiter[3],ders* ders){
    FILE *fp=fopen(dosyaAdi,"r");
    char buf[255];

    ogrenciders *anaOgrenciDers= (ogrenciders*)malloc(sizeof(ogrenciders));
    ogrenciders *anlikOgrenciDers;
    ogrenciders *oncekiOgrenciDers;

    if(fgets(buf,255,fp)!=NULL){
        ogrenciDersVerileriYerlestir(anaOgrenciDers,buf,delimiter);
    }
    anaOgrenciDers->oncekiOgrenciDers = NULL;
    oncekiOgrenciDers = anaOgrenciDers;

    while(fgets(buf,255,fp)!=NULL){
        anlikOgrenciDers = (ogrenciders*)malloc(sizeof(ogrenciders));
        anlikOgrenciDers->oncekiOgrenciDers = oncekiOgrenciDers;
        oncekiOgrenciDers->sonrakiOgrenciDers = anlikOgrenciDers;
        ogrenciDersVerileriYerlestir(anlikOgrenciDers,buf,delimiter);
        oncekiOgrenciDers = anlikOgrenciDers;
    }
    oncekiOgrenciDers->sonrakiOgrenciDers = NULL;
    fclose(fp);
    ogrenciDersKayit(anaOgrenciDers,ders);
    return anaOgrenciDers;
}

void dersDosyaYaz(struct DERS ders1){
    FILE *fp=fopen("dersler.txt","a");

    char dersKodu[255];
    char dersAdi[255];
    int toplamKredi;
    int dersKontenjani;
    char tK[255];
    char dK[255];

    strcpy(dersKodu,ders1.dersKodu);
    strcpy(dersAdi,ders1.dersAdi);
    toplamKredi = ders1.toplamKredi;
    dersKontenjani = ders1.dersKontenjani;

    fputs(dersKodu,fp);
    fputs(",",fp);
    fputs(dersAdi,fp);
    fputs(",",fp);
    fputs(itoa(toplamKredi, tK, 10),fp);
    fputs(",",fp);
    fputs(itoa(dersKontenjani, dK, 10),fp);
    fputs("\n",fp);
    fclose(fp);
    printf("\nDosyay� kontrol et");
}
void dersEkle(ders* ders){
    struct DERS eklenecekders;
    struct DERS dersKontrol;
    struct DERS dersBayrak;
    dersKontrol = *ders;
    dersBayrak = *ders;

    char dersKodu[255];
    char dersAdi[255];
    int toplamKredi;
    int dersKontenjani;
    int dersKontrolTF = 0;

    printf("--EKLE/�IKAR ��LEMLER�->DERS EKLE--\n");

    printf("Ders kodunu girin: \n:");
    scanf("%s",&dersKodu);

    while (dersKontrol.sonrakiDers != NULL){
        if(strcmp(dersKontrol.dersKodu,dersKodu) == 0){
            printf("Ders kay�tl�d�r.");
            dersKontrolTF = 1;
            break;
        }else{
            dersKontrol = *dersKontrol.sonrakiDers;
        }
    }
    if (dersKontrolTF < 1){
        printf("Ders ad�n� girin: \n:");
        scanf("%s",&dersAdi);
        printf("Dersin kredisini girin: \n:");
        scanf("%d",&toplamKredi);
        printf("Dersin kontenjan�n� girin: \n:");
        scanf("%d",&dersKontenjani);

        printf("Kaydediliyor... \n");

        strcpy(eklenecekders.dersKodu,dersKodu);
        strcpy(eklenecekders.dersAdi,dersAdi);
        eklenecekders.toplamKredi = toplamKredi;
        eklenecekders.dersKontenjani = dersKontenjani;
        //printf("struct atama");
        while(ders->sonrakiDers != NULL)
        {
            ders = ders->sonrakiDers;
        }
        //printf("while ��k��");
        ders->sonrakiDers = &eklenecekders;
        eklenecekders.sonrakiDers = NULL;


        //dersleriYazdir(&dersBayrak);
        dersDosyaYaz(eklenecekders);
    }
}
void ogrenciDosyaYaz(struct OGRENCI ogrenci1){
    FILE *fp=fopen("ogrenciler.txt","a");

    int ogrenciNo;
    char ogrenciAdi[255];
    char ogrenciSoyadi[255];
    char kayDersSayisi = 0;
    char kayToplamKredi = 0;
    char kDS[255];
    char kTK[255];
    char oN[255];

    ogrenciNo = ogrenci1.ogrenciNo;
    strcpy(ogrenciAdi,ogrenci1.ogrenciAdi);
    strcpy(ogrenciSoyadi,ogrenci1.ogrenciSoyadi);

    fputs(itoa(ogrenciNo,oN,10),fp);
    fputs(",",fp);
    fputs(ogrenciAdi,fp);
    fputs(",",fp);
    fputs(ogrenciSoyadi,fp);
    fputs(",",fp);
    fputs(itoa(kayToplamKredi,kTK,10),fp);
    fputs(",",fp);
    fputs(itoa(kayDersSayisi,kDS,10),fp);
    fputs("\n",fp);
    fclose(fp);
    printf("\nDosyay� kontrol et");

}
void ogrenciEkle(ogrenci* ogrenci){
    struct OGRENCI eklenecekOgrenci;
    struct OGRENCI ogrenciKontrol;
    struct OGRENCI ogrenciBayrak;
    ogrenciKontrol = *ogrenci;
    ogrenciBayrak = *ogrenci;

    int ogrenciKontrolTF = 0;

    int ogrenciNo;
    char ogrenciAdi[255];
    char ogrenciSoyadi[255];

    printf("--EKLE/�IKAR ��LEMLER�->��RENC� EKLE--\n");

    printf("��renci numaras�n� girin: \n:");
    scanf("%d",&ogrenciNo);

    while (ogrenciKontrol.sonrakiOgrenci != NULL){
        if(ogrenciKontrol.ogrenciNo == ogrenciNo){
            printf("��renci kay�tl�d�r.");
            ogrenciKontrolTF = 1;
            break;
        }else{
            ogrenciKontrol = *ogrenciKontrol.sonrakiOgrenci;
        }
    }
    if (ogrenciKontrolTF < 1){
        printf("��renci ad�n� girin: \n:");
        scanf("%s",&ogrenciAdi);
        printf("��renci soyad�n� girin: \n:");
        scanf("%s",&ogrenciSoyadi);

        printf("Kaydediliyor... \n");

        eklenecekOgrenci.ogrenciNo = ogrenciNo;
        strcpy(eklenecekOgrenci.ogrenciAdi,ogrenciAdi);
        strcpy(eklenecekOgrenci.ogrenciSoyadi,ogrenciSoyadi);
        eklenecekOgrenci.kayToplamKredi = 0;
        eklenecekOgrenci.kayDersSayisi = 0;

        while(ogrenci->sonrakiOgrenci != NULL)
        {
            ogrenci = ogrenci->sonrakiOgrenci;
        }
        //printf("while ��k��");
        ogrenci->sonrakiOgrenci = &eklenecekOgrenci;
        eklenecekOgrenci.sonrakiOgrenci = NULL;
        eklenecekOgrenci.oncekiOgrenci = ogrenci;

        //ogrencileriYazdir(&ogrenciBayrak);
        ogrenciDosyaYaz(eklenecekOgrenci);
    }
}

void ekleCikarMenu(ders* ders,ogrenci * ogrenci1){
    int ekleCikarMenuNo;
    printf("--EKLE/�IKAR ��LEMLER�--\n");
    printf("Yapmak istedi�iniz i�lemi se�in: \n");
    printf("1-DERS EKLE \n2-DERS �IKAR \n3-��RENC� EKLE \n4-��RENC� �IKAR \n5-DERSE ��RENC� EKLE \n6-DERSTEN ��RENC� �IKAR \n0-�IKI� \n:");
    scanf("%d",&ekleCikarMenuNo);
    switch (ekleCikarMenuNo) {
        case 1:
            dersEkle(ders);
            break;
        case 2:
            printf("Ders ��karma");
            break;
        case 3:
            ogrenciEkle(ogrenci1);
            break;
        case 4:
            printf("��renci ��karma");
            break;
        case 5:
            printf("��renci ��karma");
            break;
        case 6:
            printf("��renci ��karma");
            break;
        case 0:
            printf("��k�l�yor...");
            break;
            //anaMenu();
    }
}

void derseKayitliOgrencileriListeleTam(ogrenci * ogrenci1, int ptr){
    while (ogrenci1->sonrakiOgrenci != NULL){
        if( ogrenci1->ogrenciNo == ptr){
            printf("��renci No: %d, ��renci Ad�: %s, ��renci Soyad�: %s,\n",ogrenci1->ogrenciNo,ogrenci1->ogrenciAdi,ogrenci1->ogrenciSoyadi);

        }
        ogrenci1 = ogrenci1->sonrakiOgrenci;
    }
}

void derseKayitliOgrencileriListele(ders* ders,ogrenci * ogrenci1){
    char dersKodu[255];

    printf("--L�STELEME ��LEMLER�->DERSE KAYITLI ��RENC�LER� L�STELE--\n");

    printf("Ders kodunu girin: \n:");
    scanf("%s",&dersKodu);

    while (ders->sonrakiDers != NULL){
         if(strcmp(ders->dersKodu, dersKodu) == 0){
            for (int i=0; i<ders->ogrenciSayisi+1; i++) {
                int ptr = *(ders->ogrenciNolar + i);
                derseKayitliOgrencileriListeleTam(ogrenci1, ptr);

            }
         }
        ders = ders->sonrakiDers;
    }
}

void ogrenciAldigiDersleriListeleTam(ders* ders, char dersKodu[255]) {
    struct DERS dersBayrak;
    dersBayrak = *ders;
    
    while(ders->sonrakiDers != NULL){
        //printf(ders->dersKodu);
        //printf("\n");

        if(strcmp(ders->dersKodu, dersKodu) == 0){
            printf("Ders Kodu: %s, Ders Ad�: %s\n", ders->dersKodu, ders->dersAdi);
            break;
        }
        ders = ders->sonrakiDers;
    }
    *ders = dersBayrak;
}

void ogrenciAldigiDersleriListele(ogrenciders* od, ders* ders1){
    int ogrenciNo;
    

    printf("--L�STELEME ��LEMLER�->��RENC�N�N ALDI�I DERSLER� L�STELE--\n");

    printf("��renci no girin: \n:");
    scanf("%d",&ogrenciNo);

    while(od->sonrakiOgrenciDers != NULL){
        if(od->ogrenciNo == ogrenciNo){
            char dersKodu[255];
            strcpy(dersKodu,od->dersKodu);
            ogrenciAldigiDersleriListeleTam(ders1, dersKodu);
        }
        
        od = od->sonrakiOgrenciDers;
    }
}

void listeleMenu(ders* ders,ogrenci * ogrenci1, ogrenciders* od){
    int listeleMenuNo;
    printf("--L�STELEME ��LEMLER�--\n");
    printf("Yapmak istedi�iniz i�lemi se�in: \n");
    printf("1-DERSE KAYITLI ��RENC�LER� L�STELE \n2-��RENC�N�N ALDI�I DERSLER� L�STELE \n3-DERSLER� L�STELE \n4-��RENC�LER� L�STELE \n0-�IKI� \n:");
    scanf("%d",&listeleMenuNo);

    switch (listeleMenuNo) {
        case 1:
            derseKayitliOgrencileriListele(ders, ogrenci1);
            break;
        case 2:
            ogrenciAldigiDersleriListele(od, ders);
            break;
        case 3:
            dersleriYazdir(ders);
            break;
        case 4:
            ogrencileriYazdir(ogrenci1);
            break;
    }
}

int anaMenu(ders* ders,ogrenci * ogrenci1,ogrenciders *ogrenciders1){
    printf("--DERS KAYIT UYGULAMASI--\n");
    printf("Fatih Alt�nc� - 20011610\n");

    printf("Gitmek istedi�iniz men� numaras�n� se�in: \n");
    printf("1- EKLE/�IKAR ��LEMLER� \n2- L�STELEME ��LEMLER� \n9- �IKI�\n:");

    int anaMenuNo;
    scanf("%d",&anaMenuNo);

    switch (anaMenuNo) {
        case 9:
            return 9;
        case 1:
            ekleCikarMenu(ders,ogrenci1);
            break;
        case 2:
            listeleMenu(ders,ogrenci1,ogrenciders1);
            break;

    }

    return 1;
}

void test(){
    ders* ders;
    ders = dersleriOku("dersler.txt",",\0");
    dersleriYazdir(ders);

    ogrenci * ogrenci1;
    ogrenci1 = ogrencileriOku("ogrenciler.txt",",\0");
    ogrencileriYazdir(ogrenci1);

    ogrenciders *ogrenciders1;
    ogrenciders1 = ogrenciDersOku("OgrenciDersKayit.txt",",\0",ders);
    ogrenciDersYazdir(ogrenciders1);
}

int main() {
    setlocale(LC_ALL,"Turkish");
    // test();
    ders* ders;
    ders = dersleriOku("dersler.txt",",\0");
    ogrenci * ogrenci1;
    ogrenci1 = ogrencileriOku("ogrenciler.txt",",\0");
    ogrenciders *ogrenciders1;
    ogrenciders1 = ogrenciDersOku("OgrenciDersKayit.txt",",\0",ders);
    int loop = 0;
    // while (loop < 5){
        loop = anaMenu(ders, ogrenci1, ogrenciders1);
    // }

    return 0;
}
