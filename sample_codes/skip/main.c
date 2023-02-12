#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define SKIPLIST_MAXLEVEL        16
#define SKIPLIST_NUM_NODES(l)    (l->numNodes)
#define SKIPLIST_NODE_FIRST(l)   (l->header->forward[0])
#define SKIPLIST_NODE_NEXT(n)    (n->forward[0])
#define SKIPLIST_NODE_VALUE(n)   (n ? n->value : NULL)
#define SKIPLIST_DEFAULT_BLOCK_SIZE    4096
#define SKIPLIST_NODE_NEAREST_UNIT(n)  ((n) + ((~((n) & 3) + 1) & 3))
#define SKIPLIST_NODE_UNIT_SIZE(ns)    (SKIPLIST_NODE_NEAREST_UNIT((ns)))
#define SKIPLIST_UNIT_MULT    (4)
#define SKIPLIST_NODE_SIZE(l)         (sizeof(SkipListNode_t) + (l) * sizeof(SkipListNode_t *))
#define SKIPLIST_NODE_UNIT_LEVEL(l)    (SKIPLIST_NODE_UNIT_SIZE(SKIPLIST_NODE_SIZE(l)) / SKIPLIST_UNIT_MULT  - 1)
#define SKIPLIST_MAX_NODE_SIZE         (SKIPLIST_NODE_SIZE(SKIPLIST_MAXLEVEL))
#define SKIPLIST_MAX_UNIT_SIZE        (SKIPLIST_NODE_UNIT_SIZE(SKIPLIST_MAX_NODE_SIZE))
#define SKIPLIST_NUM_UNITS            (SKIPLIST_MAX_UNIT_SIZE / SKIPLIST_UNIT_MULT)

typedef int  (*SkipListCmp_t)  (void *key1, void *key2);
typedef void (*SkipListFree_t) (void *value);

typedef void * (*SkipListUserAlloc_t)(void *key, void *data, void **newKey, void **newData);

typedef struct SkipListNode_s {
    void           *key;
    void           *value;
	unsigned char	level;
	unsigned char	reserved[3];
    struct SkipListNode_s *forward[1]; /* Forward pointerlar�n de�i�ken b�y�kl��� */
} SkipListNode_t;

typedef struct SkipList_s {
    int             numNodes; 
    int             maxLevels; 
    int             level;         /* Maksimum katman say�s� (listedeki katmanlar�n say�s�ndan 1 fazla) */
    SkipListCmp_t   cmpFn;
    SkipListFree_t  freeFn;

    void            **bh;

    SkipListNode_t  *header;    /* Headar�n pointer� */
}

SkipList_t;
SkipList_t *SkipListAlloc( SkipListCmp_t, SkipListFree_t );
void SkipListFree( SkipList_t * );
int SkipListInsert( SkipList_t *, void *, void *, int  );
int SkipListDelete( SkipList_t *, void * ); 
SkipListNode_t *SkipListGetNode( SkipList_t *, void * );
SkipListNode_t *SkipListGetNodeOrNext( SkipList_t *, void * );
int SkipListGetCount( SkipList_t * );
SkipListNode_t *SkipListGetFirst( SkipList_t * );
SkipListNode_t *SkipListGetNext( SkipList_t *, void * );
SkipListNode_t *
SkipListFindOrInsert( SkipList_t *list, void *key, void *value , SkipListUserAlloc_t allocFn, int *found );
void SkipListClear( SkipList_t *list );
int SkipListDeleteNode( SkipList_t *list, void *key );
void SkipListNClear( SkipList_t *list , int n) ;
static int getNewLevel( int );
static SkipListNode_t *newSkipListNode( SkipList_t *, void *, void *, int );
static void * skipListNodeAlloc( SkipList_t *list, int level );
static void skipListNodeFree( SkipList_t *list, void *node );

/* FONKS�YONLAR */

/*
Yeni skip list'i belle�e atar.
*/

SkipList_t *
SkipListAlloc( SkipListCmp_t cmpFn, SkipListFree_t freeFn )
{
    int        i = 0;
    SkipList_t *list = NULL;

    /*
     * Kullan�c� fonksiyonlar� olmadan ilerleyemeyiz. 
     */
    if ( cmpFn == NULL || freeFn == NULL )
        return( NULL);
    /*
     * Yeni skip list i�in ramde yer a�. 
     */
    i = sizeof(SkipList_t);
    if ( (list = (SkipList_t *)malloc( i )) == NULL )
        return( NULL );
    memset( list, 0, i ); 
    
    /*
     * Liste i�in ilk rastgele seviyeyi ayarlay�n ve �e�itli liste de�i�kenlerini ba�lat�r. 
     */
     
    srand( (unsigned long)time(NULL) );
    list->level  = 0;
    list->cmpFn  = cmpFn;
    list->freeFn = freeFn;
    list->numNodes = 0;
    list->maxLevels = SKIPLIST_MAXLEVEL - 1;
    list->header = newSkipListNode( list, (void *)0, (void *)0, list->maxLevels );
    
    return( list );
}

/*
Skip list'i bo�alt�r.
*/

void
SkipListFree( SkipList_t *list ) 
{
    register SkipListNode_t *p, *q;
    int i;

    /*
     * Liste boyu b�t�n d���mleri sil. 
     */
     
    p = list->header->forward[0];

    while ( p ) {
 
        q = p->forward[0];

        /*
         * Bu d���mdeki ��eyi bo�altmak i�in kullan�c� tan�ml� serbest i�levi �a��r�n. 
         */

        list->freeFn( p->value );

        /*
         * �imdi d���m� bo�al�yoruz. 
         */
         
        skipListNodeFree( list, p );

        p = q;

    }
   
    /*
     * Header'� mallocla olu�turdu�umuzdan freelemeliyiz.
     */
 
    free(list->header);

    /*
     * D���mleri bo�altt�ktan sonra listeyi de bo�altmay� unutmuyoruz.
     */
     
    free( list );
}

/*
Verilen eleman� listeyi ekler.
*/
int
SkipListInsert( SkipList_t *list, void *key, void *value, int replace ) 
{
    int i = 0;
    int newLevel = 0;
    SkipListNode_t *x= NULL;
    SkipListNode_t *update[SKIPLIST_MAXLEVEL];
    
    /*
     * E�er arad���m�z eleman katmandaki say�dan b�y�kse t�m katmanlar� tarar (Kendi katman�ndaki header dahil)
     */
    for ( x=list->header, i=list->level; i >= 0; i-- ) {
        while ( x->forward[i] != 0 &&
                list->cmpFn(x->forward[i]->key, key) < 0 ) {
            x = x->forward[i];
        }
        /*
         * Katman pointer�n� tut. 
         */
        update[i] = x;
    }
    x = x->forward[0];
    /*
     * Eleman zaten var. 
     */
    if ( x && list->cmpFn(x->key, key) == 0 ) {
        if ( replace ) {
            list->freeFn( x->value );
            x->key = key;
            x->value = value;
            return( 0 );
        }
        else
            return( -1 );
    }
    /*
     * Bu anahtar kelime i�in listeye yeni eleman ekle.
     * Yeni katman� al ve listeyi d�zenle.
     */
    newLevel = getNewLevel( list->maxLevels );
    /*
     * Header katman�n� ayarla. 
     */
    if ( newLevel > list->level ) {
        for ( i=list->level+1; i<=newLevel; i++ ) {
            update[i] = list->header;
        }
        list->level = newLevel;
    }
    /*
     * Yeni eleman� olu�tur ve listeye ekle. 
     */
    x = newSkipListNode( list, key, value, newLevel );
    /*
     * T�m katmanlar� tara.
     */
    for ( i=0; i<=newLevel; i++ ) {
        /*
         * Yeni eleman�n sonraki pointer�n� ayarlar. 
         */
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;
    }
    list->numNodes++;
    
    return(0);
}

/*
E�le�en eleman� siler.
*/
int
SkipListDelete( SkipList_t *list, void *key ) 
{
    int i = 0;
    SkipListNode_t *x = NULL;
    SkipListNode_t *y = NULL;
    SkipListNode_t *update[SKIPLIST_MAXLEVEL];
    /*
     * E�er arad���m�z eleman katmandaki say�dan b�y�kse t�m katmanlar� tarar (Kendi katman�ndaki header dahil)
     */
    for ( x=list->header, i=list->level; i >= 0; i-- ) {
        while ( x->forward[i] != 0 &&
                list->cmpFn(x->forward[i]->key, key) < 0 ) {
            x = x->forward[i];
        }
        /*
         * Katman pointer�n� tut. 
         */
        update[i] = x;
    }
    x = x->forward[0];
    /*
     * Anahtar varsa, ilgili ��eyi sil.
     */
    if ( x && list->cmpFn(x->key, key) == 0 ) {
        for ( i=0; i<=list->level; i++ ) {
            if ( (y=update[i])->forward[i] == x ) {
                y->forward[i] = x->forward[i];
            }    
        }
        list->freeFn( x->value );
        skipListNodeFree( list, x );
        /*
         * Yeni header katman�n� ayarla..
         */
        while ( list->level > 0 &&
            list->header->forward[list->level] == NULL ) {
            list->level--; 
        }
        list->numNodes--;
        return( 0 );
    }
    else {
        return( -1 );
    }    
}

static void
SkipListDummyFree( void *dummy )
{

}


/*
E�le�en d���m� siler.
*/
 
int
SkipListDeleteNode( SkipList_t *list, void *key )
{
    SkipListFree_t saveFree = list->freeFn;
    int ret;

    list->freeFn = SkipListDummyFree;

    ret = SkipListDelete(list,key);

    list->freeFn = saveFree;

    return ret;

}

/*
E�le�en d���m� bulur.
*/
 
SkipListNode_t *
SkipListGetNode( SkipList_t *list, void *key )
{
    int i = 0;
    SkipListNode_t *x= NULL;
     
    for ( x=list->header, i=list->level; i >= 0; i-- ) {
        while ( x->forward[i] != 0 &&
                list->cmpFn(x->forward[i]->key, key) < 0 ) {
            x = x->forward[i];
        }
    }
    x = x->forward[0];
    if ( x && list->cmpFn(x->key, key) == 0 ) {
        return( x );
    }
    else {
        return( NULL );
    }    
}


/*
Listedeki d���m� getirir d���m yoksa di�er d���me ge�er.
*/
 
SkipListNode_t *
SkipListGetNodeOrNext( SkipList_t *list, void *key )
{
    int i = 0;
    SkipListNode_t *x= NULL;
    
     
    for ( x=list->header, i=list->level; i >= 0; i-- ) {
        while ( x->forward[i] != 0 &&
                list->cmpFn(x->forward[i]->key, key) < 0 ) {
            x = x->forward[i];
        }
    }
    x = x->forward[0];
    return (x);  
}



/*
Listedeki b�t�n elemanlar�n say�s�n� getirir.
*/

int
SkipListGetCount( SkipList_t *list )
{
    return( SKIPLIST_NUM_NODES(list) );
}

/*
Listedeki (katmandaki) ilk d���m� getirir.
*/

SkipListNode_t *
SkipListGetFirst( SkipList_t *list )
{
    return( SKIPLIST_NODE_FIRST(list) );
}

/*
Listedeki sonraki d���m� getirdi�imiz fonksiyon.
*/

SkipListNode_t *
SkipListGetNext( SkipList_t *list, void *key )
{
    int i = 0;
    SkipListNode_t *x= NULL;
    
    for ( x=list->header, i=list->level; i >= 0; i-- ) {
        while ( x->forward[i] != 0 &&
                list->cmpFn(x->forward[i]->key, key) < 0 ) {
            x = x->forward[i];
        }
    }
    x = x->forward[0];
    if ( x && list->cmpFn(x->key, key) == 0 ) {
        x = x->forward[0];
    }

    return( x );
}


/*
 * Veriyi basit�e buldu�umuz ve ekleme yapt���m�z fonksiyon.
 */
 
SkipListNode_t *
SkipListFindOrInsert( SkipList_t *list, void *key, void *value , SkipListUserAlloc_t allocFn, int *found )
{
    int i = 0;
    int newLevel = 0;
    SkipListNode_t *x= NULL;
    SkipListNode_t *update[SKIPLIST_MAXLEVEL];
    void *newKey;
    void *newValue;
    
    *found = 0;
    
    /*
     * E�er arad���m�z eleman katmandaki say�dan b�y�kse t�m katmanlar� tarar (Kendi katman�ndaki header dahil)
     */
     
    for ( x=list->header, i=list->level; i >= 0; i-- ) {
        while ( x->forward[i] != 0 &&
                list->cmpFn(x->forward[i]->key, key) < 0 ) {
            x = x->forward[i];
        }
        /*
         * Katman pointer�n� tut. 
         */
         
        update[i] = x;
    }
    x = x->forward[0];
    /*
     * Eleman zaten var. 
     */

    if ( x && list->cmpFn(x->key, key) == 0 ) {

        *found = 1;

        return x;

    }


    /*
     * BU katman i�in yeni eleman ekler.
     * Yeni katman ekle ve mevcut katman� d�zelt.
     */

    newLevel = getNewLevel( list->maxLevels );
    /*
     * Header katman�n� ayarla. 
     */
     
    if ( newLevel > list->level ) {
        for ( i=list->level+1; i<=newLevel; i++ ) {
            update[i] = list->header;
        }
        list->level = newLevel;
    }

    if ( allocFn ) {

        /*
         * Kullan�c�, yeniAnahtar ve yeniDeger'i ay�rmak i�in bir geri arama i�levine sahiptir. 
         */

        if ( !allocFn(key,value,&newKey,&newValue) )
            return NULL;

    } else {

        /*
         * Kullan�c�, sa�lanan anahtar� ve de�eri girmeye �al���r. 
         */

        newKey = key;

        newValue = value;

    }

    /*
     * Yeni eleman� olu�tur ve listeye ekle. 
     */
    x = newSkipListNode( list, newKey, newValue, newLevel );

    if (!x)
        return NULL;

    /*
     * T�m katmanlar� ara.
     */
    for ( i=0; i<=newLevel; i++ ) {
        /*
         * Yeni eleman�n sonraki pointer'�n� ayarla. 
         */
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;
    }
    list->numNodes++;
    
    return x;

}


/* Listedeki t�m elemanlar� bo�altan method.*/

void
SkipListClear( SkipList_t *list ) 
{
    register SkipListNode_t *p, *q;
    int i;    

    /*
     * Header d���m� hari� t�m liste boyunca git ve sil i�lemi yap. 
     */
     
    p = list->header->forward[0];

    while ( p ) {

        q = p->forward[0];

         /*
         * D���mdeki elemlar� s�ras�yla ramden bo�altmak i�in yazd���m�z methodu kullan�yoruz.
         */

        list->freeFn( p->value );

        /*
         * D���m� ramden bo�alt�yoruz. 
         */

        skipListNodeFree( list, p );

        p = q;
    }
    

    /*
     * Header d���m hari� t�m nodelar bo�alt�ld�.
     */

    for ( i=0; i<=list->maxLevels; i++ ) {

        list->header->forward[i] = NULL;

    }

	list->level = 0;

    list->numNodes = 0;
    
}



/* 
Listedeki n kadar eleman� bo�altan method.
*/
 
void
SkipListNClear( SkipList_t *list , int n) 
{
    register SkipListNode_t *p, *q;
    int i;    
    SkipListNode_t *update[SKIPLIST_MAXLEVEL + 1];
	int maxUpdateLevel = -1;

	if ( !n ||
		!SKIPLIST_NUM_NODES(list) )
		return;


	if ( n > SKIPLIST_NUM_NODES(list) )
		n = SKIPLIST_NUM_NODES(list);

	if ( n == SKIPLIST_NUM_NODES(list) ) {
			
		SkipListClear(list);
		return;

	}
	
    /*
     * Header d���m� hari� t�m liste boyunca git ve sil i�lemi yap. 
     */

    p = list->header->forward[0];

	assert(p != NULL);

	i = n;

    while ( i ) {

        q = p->forward[0];

        /*
         * D���mdeki elemlar� s�ras�yla ramden bo�altmak i�in yazd���m�z methodu kullan�yoruz.
         */

		if ( p->level > maxUpdateLevel )
			maxUpdateLevel = p->level;

		memcpy(update,p->forward,(p->level  + 1 ) * sizeof(SkipListNode_t *));

        list->freeFn( p->value );

        /*
         * D���m� ramden bo�alt�yoruz. 
         */

        skipListNodeFree( list, p );

        p = q;

		i--;
    }

	memcpy(list->header->forward,update,(maxUpdateLevel + 1 ) * sizeof(SkipListNode_t *));

    while ( list->level > 0 &&
        list->header->forward[list->level] == NULL ) {
        list->level--; 
    }

    list->numNodes-=n;
    
}

/* STAT�K FONKS�YON TANIMLARI */

static int
getNewLevel( int maxLevel )

{
    int level;

    for ( level = 0; (rand() < (RAND_MAX / 2)) && level < SKIPLIST_MAXLEVEL; level++ )
        ;

    level = level > maxLevel ? maxLevel : level;

    return( level );

}

static SkipListNode_t *
newSkipListNode( SkipList_t *list, void *key, void *value, int newLevel )
{
    int i;
    SkipListNode_t *node;

    node = skipListNodeAlloc(list,newLevel);
    if ( node == NULL )
        return( NULL );
    for ( i=0; i<=newLevel; i++ ) {
        node->forward[i] = NULL;
    }
    node->key   = key;
    node->value = value;
	node->level = newLevel;
    return( node );
}


static void *
skipListNodeAlloc( SkipList_t *list, int level )
{
    int idx;

    return malloc(SKIPLIST_NODE_SIZE(level));

}

static void
skipListNodeFree( SkipList_t *list, void *node )
{
    int idx;

    free(node);            
}


/* ANA FONKS�YONLAR */


static int myCmp( void *key1, void *key2 )
{
    int key1Val = (int) ((int *)key1);
    int key2Val = (int) ((int *)key2);

    return( key1Val - key2Val );
}

static void myFree( void *value )
{
   printf("RamdenBosaltildi %d\n",(int)value);

}

static void displayMenu()
{
    printf( "\n\n\n\n\n" );
    printf( "1. Ekle \n" );
    printf( "2. Sil \n" );
    printf( "3. Ara \n" );
    printf( "4. SonrakiniGetir \n" );
    printf( "5. Yazdir \n" );
    printf( "6. Temizle \n" );
    printf( "7. Sonlandir \n" );
    printf( "\n\n\n" );
    printf( "Secim : " );
}

static void insertValue( SkipList_t *list )
{
    char buf[256];
    int  value = 0;

    printf( "\n\nEklenecek degeri girin : " );
    gets( buf );
    value = atoi(buf);
    printf( "Ekleniyor %d ... \n", value );
    if ( SkipListInsert( list, (void *)value, (void *)value, 0 ) != 0 ) {
        printf( "Eklerken hata olustu \n" );
    }
    else {
        printf( "Ekleme basarili \n" );
    }    
}

static void deleteValue( SkipList_t *list )
{
    char buf[256];
    int  key = 0;

    printf( "\n\nSilinecek degeri girin : " );
    gets( buf );
    key = atoi(buf);
    printf( "Eslesen eleman siliniyor %d ...\n", key );
    if ( SkipListDelete(list, (void *)key) != 0 ) {
        printf( "Silmede hata olustu \n" );
    }
    else {
        printf( "Silme basarili \n" );
    }    
}

static void searchValue( SkipList_t *list )
{
    char buf[256];
    int  key = 0;
    SkipListNode_t *node;
    
    printf( "\n\nAranacak degeri girin : " );
    gets( buf );
    key = atoi(buf);
    printf( "Girilen deger araniyor %d ... \n", key );
    node = SkipListGetNode( list, (void *)key );
    if ( node == NULL ) {
        printf( "Aramada hata olustu \n" );
    }
    else {
        printf( "Arama basarili \n" );
    }    
}

static void getNextValue( SkipList_t *list )
{
    char buf[256];
    int  key = 0;
    SkipListNode_t *node;
    
    printf( "\n\nBulunacak sonraki degeri girin : " );
    gets( buf );
    key = atoi(buf);
    printf( "Sonraki getiriliyor %d ... \n", key );
    node = SkipListGetNext( list, (void *)key );
    if ( node == NULL ) {
        printf( "Devaminda bir sey yok\n" );
    }
    else {
        printf( "Arama basarili \n" );
        printf( "Sonraki eleman %d\n", (int)node->value );
    }    
}

static void printValue( SkipList_t *list )
{
    int i = 0;
    SkipListNode_t *node;
    
    printf( "\n\n" );
    node = SkipListGetFirst( list );
    for ( ; node; node=node->forward[0] ) {
        printf( "Eleman-%d : %d\n", i++, (int)node->value );
    }    
}

static void clearList( SkipList_t *list )
{

    printf("\n\nListe temizleniyor \n");

    SkipListClear(list);

}

main()
{
    SkipList_t *list = NULL;
    char buf[256];
    int  choice = 0;
 
    list = SkipListAlloc( myCmp, myFree );
    if ( list == NULL ) {
        printf( "HATA: Hafiza atamasi basarisiz\n" );
        exit(1);
    }

    while (1) {
        displayMenu();
        choice = atoi( gets(buf) );
        switch( choice ) {
            case 1 :
                insertValue( list );
                break;
            case 2 :
                deleteValue( list );
                break;
            case 3 :
                searchValue( list );
                break;
            case 4 :
                getNextValue( list );
                break;
            case 5 :
                printValue( list );
                break;
            case 6 :
                clearList( list );
                break;
            case 7:
                SkipListFree(list);
                exit(0);
            default :
                break;
        }
    }

    exit(0);
}



