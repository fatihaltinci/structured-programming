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
        printf("Kodu: %s, Adý: %s, Kredisi: %d, Kontenjan: %d\n",ders1->dersKodu,ders1->dersAdi,ders1->toplamKredi,ders1->dersKontenjani);
        for (int i=0; i<ders1->ogrenciSayisi+1; i++) {
            printf("Öðrenci Nolarý: %d\n", *(ders1->ogrenciNolar+i));
        }
        ders1=ders1->sonrakiDers;
    }
}

void dersleriYazdir(ders *ders1){

    while(ders1!=NULL){
        printf("Kodu: %s, Adý: %s, Kredisi: %d, Kontenjan: %d\n",ders1->dersKodu,ders1->dersAdi,ders1->toplamKredi,ders1->dersKontenjani);
        ders1=ders1->sonrakiDers;
    }

}

void ogrencileriYazdir(ogrenci *ogrenci1){
    while(ogrenci1 != NULL){
        printf("Öðrenci No: %d, Adý: %s, Soyadý: %s, Toplam Kredi: %d, Ders Sayýsý: %d\n",ogrenci1->ogrenciNo,ogrenci1->ogrenciAdi,ogrenci1->ogrenciSoyadi,ogrenci1->kayToplamKredi,ogrenci1->kayDersSayisi);
        ogrenci1 = ogrenci1->sonrakiOgrenci;
    }
}

void ogrenciDersYazdir(ogrenciders *ogrenciders1){
    while (ogrenciders1 != NULL){
        printf("ID: %d, Ders Kodu: %s, Öðrenci No: %d, Tarih: %s, Ders Durumu: %s",ogrenciders1->ID,ogrenciders1->dersKodu,ogrenciders1->ogrenciNo,ogrenciders1->tarih,ogrenciders1->dersDurumu);
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
    printf("\nDosyayý kontrol et");
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

    printf("--EKLE/ÇIKAR ÝÞLEMLERÝ->DERS EKLE--\n");

    printf("Ders kodunu girin: \n:");
    scanf("%s",&dersKodu);

    while (dersKontrol.sonrakiDers != NULL){
        if(strcmp(dersKontrol.dersKodu,dersKodu) == 0){
            printf("Ders kayýtlýdýr.");
            dersKontrolTF = 1;
            break;
        }else{
            dersKontrol = *dersKontrol.sonrakiDers;
        }
    }
    if (dersKontrolTF < 1){
        printf("Ders adýný girin: \n:");
        scanf("%s",&dersAdi);
        printf("Dersin kredisini girin: \n:");
        scanf("%d",&toplamKredi);
        printf("Dersin kontenjanýný girin: \n:");
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
        //printf("while çýkýþ");
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
    printf("\nDosyayý kontrol et");

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

    printf("--EKLE/ÇIKAR ÝÞLEMLERÝ->ÖÐRENCÝ EKLE--\n");

    printf("Öðrenci numarasýný girin: \n:");
    scanf("%d",&ogrenciNo);

    while (ogrenciKontrol.sonrakiOgrenci != NULL){
        if(ogrenciKontrol.ogrenciNo == ogrenciNo){
            printf("Öðrenci kayýtlýdýr.");
            ogrenciKontrolTF = 1;
            break;
        }else{
            ogrenciKontrol = *ogrenciKontrol.sonrakiOgrenci;
        }
    }
    if (ogrenciKontrolTF < 1){
        printf("Öðrenci adýný girin: \n:");
        scanf("%s",&ogrenciAdi);
        printf("Öðrenci soyadýný girin: \n:");
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
        //printf("while çýkýþ");
        ogrenci->sonrakiOgrenci = &eklenecekOgrenci;
        eklenecekOgrenci.sonrakiOgrenci = NULL;
        eklenecekOgrenci.oncekiOgrenci = ogrenci;

        //ogrencileriYazdir(&ogrenciBayrak);
        ogrenciDosyaYaz(eklenecekOgrenci);
    }
}

void ekleCikarMenu(ders* ders,ogrenci * ogrenci1){
    int ekleCikarMenuNo;
    printf("--EKLE/ÇIKAR ÝÞLEMLERÝ--\n");
    printf("Yapmak istediðiniz iþlemi seçin: \n");
    printf("1-DERS EKLE \n2-DERS ÇIKAR \n3-ÖÐRENCÝ EKLE \n4-ÖÐRENCÝ ÇIKAR \n5-DERSE ÖÐRENCÝ EKLE \n6-DERSTEN ÖÐRENCÝ ÇIKAR \n0-ÇIKIÞ \n:");
    scanf("%d",&ekleCikarMenuNo);
    switch (ekleCikarMenuNo) {
        case 1:
            dersEkle(ders);
            break;
        case 2:
            printf("Ders çýkarma");
            break;
        case 3:
            ogrenciEkle(ogrenci1);
            break;
        case 4:
            printf("Öðrenci çýkarma");
            break;
        case 5:
            printf("Öðrenci çýkarma");
            break;
        case 6:
            printf("Öðrenci çýkarma");
            break;
        case 0:
            printf("Çýkýlýyor...");
            break;
            //anaMenu();
    }
}

void derseKayitliOgrencileriListeleTam(ogrenci * ogrenci1, int ptr){
    while (ogrenci1->sonrakiOgrenci != NULL){
        if( ogrenci1->ogrenciNo == ptr){
            printf("Öðrenci No: %d, Öðrenci Adý: %s, Öðrenci Soyadý: %s,\n",ogrenci1->ogrenciNo,ogrenci1->ogrenciAdi,ogrenci1->ogrenciSoyadi);

        }
        ogrenci1 = ogrenci1->sonrakiOgrenci;
    }
}

void derseKayitliOgrencileriListele(ders* ders,ogrenci * ogrenci1){
    char dersKodu[255];

    printf("--LÝSTELEME ÝÞLEMLERÝ->DERSE KAYITLI ÖÐRENCÝLERÝ LÝSTELE--\n");

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
            printf("Ders Kodu: %s, Ders Adý: %s\n", ders->dersKodu, ders->dersAdi);
            break;
        }
        ders = ders->sonrakiDers;
    }
    *ders = dersBayrak;
}

void ogrenciAldigiDersleriListele(ogrenciders* od, ders* ders1){
    int ogrenciNo;
    

    printf("--LÝSTELEME ÝÞLEMLERÝ->ÖÐRENCÝNÝN ALDIÐI DERSLERÝ LÝSTELE--\n");

    printf("Öðrenci no girin: \n:");
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
    printf("--LÝSTELEME ÝÞLEMLERÝ--\n");
    printf("Yapmak istediðiniz iþlemi seçin: \n");
    printf("1-DERSE KAYITLI ÖÐRENCÝLERÝ LÝSTELE \n2-ÖÐRENCÝNÝN ALDIÐI DERSLERÝ LÝSTELE \n3-DERSLERÝ LÝSTELE \n4-ÖÐRENCÝLERÝ LÝSTELE \n0-ÇIKIÞ \n:");
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
    printf("Fatih Altýncý - 20011610\n");

    printf("Gitmek istediðiniz menü numarasýný seçin: \n");
    printf("1- EKLE/ÇIKAR ÝÞLEMLERÝ \n2- LÝSTELEME ÝÞLEMLERÝ \n9- ÇIKIÞ\n:");

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
