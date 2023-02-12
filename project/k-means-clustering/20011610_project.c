#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Zaman fonksiyonları için kullandığımız kütüphane
#include <math.h> // Matemiksel fonksiyonlar için kullandığımız kütüphane

int main(){
    clock_t t;
    t = clock();
    int i,j; // İterasyon değişkenlerimizi tanımladık.
    int a[6][6]; // 2 boyutlu dizimizi tanımladık.
    srand(time(0)); // zamanı seed olarak kullandık ve her seferinde farklı sayılar ürettik.
    for(i=0;i<6;i++)
    {
        for(j=0;j<6;j++)
// rastgele oluşturulmuş sayı
            a[i][j]=0+rand()%2; //rand() fonk ile üretilen sayıların aralığı [a,b)'de şu şekilde yazılır: a[i]=a+ Rand()%(ba);
    }

    for(i=0;i<6;i++)
        for(j=0;j<6;j++){
            if(a[i][j]==1)
                a[i][j]=i*6+j+1;
        }
    printf("rastgele oluşturulmuş nesne noktaları: \n");
    for(i=0;i<6;i++)
        for(j=0;j<6;j++){
            if(j!=5)
                printf(" %02d  ",a[i][j]);
            else
                printf(" %02d  \n",a[i][j]);
        }
    printf("\n"); printf("\n");

    int x[2]={1,1}; // İki başlangıç noktası seçer.
    int y[2]={4,4};
    int x1,x2,y1,y2;

    int d1[36]={0}; // Bunu noktalar arasındaki mesafeye göre iki kümelenmiş diziye böler.
    int d2[36]={0};
    int k=0;
    int l=0;
    for(i=0;i<6;i++)
        for(j=0;j<6;j++){
            if(a[i][j]!=0){
                if(sqrt((i-x[0])*(i-x[0])+(j-x[1])*(j-x[1]))<sqrt((i-y[0])*(i-y[0])+(j-y[1])*(j-y[1]))){
                    d1[k]=a[i][j];
                    k++;
                }
                else{
                    d2[l]=a[i][j];
                    l++;
                }
            }
        }
    printf("İki başlangıç noktası ile  dizileri kümelendirerek sınıflandırır. \n");
    printf("Birinci dizi: ");
    for(i=0;i<k;i++)
        printf("%d  ",d1[i]);
    printf("\n");
    printf("İkinci dizi: ");
    for(i=0;i<l;i++)
        printf("%d  ",d2[i]);

    printf("\n");
    int o=0;int p=0;
    int q; //Her noktanın satır numarası
    for(i=0;i<k;i++){
        q=(d1[i]-1)/6;
        o=q+o;
        p=d1[i]-q*6-1+p;
    }
    x[0]=o/k;x[1]=p/k;
    o=0;p=0;
    for(i=0;i<l;i++){
        q=(d2[i]-1)/6;
        o=q+o;
        p=d2[i]-q*6-1+p;
    }
    y[0]=o/l;y[1]=p/l;
    printf("\n");printf("\n");
    printf("İki sıralı dizi merkezi değerinden oluşturulan iki yeni koordinat noktası: \n");
    printf("%d, %d    ",x[0],x[1]);
    printf("%d, %d \n\n",y[0],y[1]);

    while(1){
        x1=x[0];x2=x[1];y1=y[0];y2=y[1];
        for(i=0;i<36;i++){
            d1[i]=0;d2[i]=0;}
        k=0;
        l=0;
        for(i=0;i<6;i++)
            for(j=0;j<6;j++){
                if(a[i][j]!=0){
                    if(sqrt((i-x[0])*(i-x[0])+(j-x[1])*(j-x[1]))<sqrt((i-y[0])*(i-y[0])+(j-y[1])*(j-y[1]))){
                        d1[k]=a[i][j];
                        k++;
                    }
                    else{
                        d2[l]=a[i][j];
                        l++;
                    }
                }
            }
        o=0;p=0;q=0;
        for(i=0;i<k;i++){
            q=(d1[i]-1)/6;
            o=q+o;
            p=d1[i]-q*6-1+p;
        }
        x[0]=o/k;x[1]=p/k;
        o=0;p=0;q=0;
        for(i=0;i<l;i++){
            q=(d2[i]-1)/6;
            o=q+o;
            p=d2[i]-q*6-1+p;
        }
        y[0]=o/l;y[1]=p/l;
        printf("İki sıralı dizi merkezi değerinden oluşturulan iki yeni koordinat noktası: \n");
        printf("%d, %d      ", x[0],x[1]);
        printf("%d, %d \n\n", y[0],y[1]);

        if(x1==x[0] && x2==x[1] && y1==y[0] && y2==y[1]){
            printf("Son iki koordinat noktası bulundu! \n");
            t = clock() - t;
            double time_taken = ((double)t)/CLOCKS_PER_SEC;
            printf("main() , %f saniyede çalıştı \n", time_taken);
            for(i=1;i<11;i++){
            printf("%d boyutlu dizi icin calisma zamani: \t",i); // 10 boyutlu diziye kadar calisma zamani
            for(j=1;j<=i;j++){
            
            printf("*");
        }
            printf("\n");
        
    }
            return 0;

        }
        
    }
    

}