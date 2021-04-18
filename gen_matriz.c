#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gen_matriz.h"

//Definiciones del variable de cálculo random y los valores de salida
#define MAX 100
#define ERR -1
#define FIN 0

int generar(int TAM) {
    
    //Declaración de las variables e inicialización de las matrices
    int *mA, *mB, i;
    mA=malloc(TAM*TAM*sizeof(int));
    mB=malloc(TAM*TAM*sizeof(int));
    
    //Creación de un fichero en blanco
    FILE *f1;
    f1=fopen("matrix.txt","w+");
    if (f1==NULL){
        printf("ERROR! No se pudo abrir el documento");
        return ERR;
    }
    
    srand(time(0));
    for(i=0; i<TAM*TAM; i++){
        mA[i]=rand()%MAX;
        //mA[i] = 1;
        fprintf(f1,"%d ",mA[i]); //Escritura en el fichero
    }

    fprintf(f1,"\n");
    
    for(i=0; i<TAM*TAM; i++){
        mB[i]=rand()%MAX;
        //mB[i] = 1;
        fprintf(f1,"%d ",mB[i]); //Escritura en el fichero
    }
    
    //Liberar el espacio asignado y cerrado del fichero
    free(mA);
    free(mB);
    fclose(f1);
    return FIN;
}
