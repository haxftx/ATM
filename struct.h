typedef struct CelulaH
{//celula listei H
    char *info;
    struct CelulaH *urm;
    
}CelulaH, *ListaH, **AdrH;

typedef struct card
{//tipul unui card
    char *cart_number;
    char *pin;
    char *expiry_date;
    char *CVV;
    char *balance;
    char *status;
    int block;
    ListaH history;
}TCard;

typedef struct CelulaLSC
{//celula liste LSC
    TCard *info;
    struct CelulaLSC *urm;
}CelulaLSC, *ListaLSC, **AdrLSC;

typedef struct CelulaLC
{//celula listei LC
    ListaLSC info;
    struct CelulaLC *urm;   
}CelulaLC, *ListaLC, **AdrLC;