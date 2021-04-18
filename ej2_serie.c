//#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Definiciones del variable de cálculo random y los valores de salida
#define MAX 100
#define ERR -1
#define FIN 0

int main(int argc, char** argv) {

    //Comprobacoión de que la llamada es correcta
    if(argc != 2){
           printf("Error! ./nombre TAM\n");
   	   return(ERR);
    }

    //Inicialización y declaración de las matrices
    int TAM = atoi(argv[1]);
    int *mA, *mB, *mC;
    mA=malloc(TAM*TAM*sizeof(int));
    mB=malloc(TAM*TAM*sizeof(int));
    mC=malloc(TAM*TAM*sizeof(int));

    srand(time(0));
    for(int i=0; i<TAM*TAM; i++){
            mA[i]=rand()%MAX;
            //mA[i] = 1;
    }

    for(int i=0; i<TAM*TAM; i++){
            mB[i]=rand()%MAX;
            //mB[i] = 1;
    }
    
    //Realización del cálculo
    for(int i=0; i<TAM; i++){
        for (int j=0; j<TAM; j++){
            int suma = 0;
            for (int k=0; k<TAM; k++){
                suma += mA[i+(j*TAM)] * mB[k+(j*TAM)];
            }
            mC[i+(j*TAM)] = suma;
        }
    }

    //Impresión por pantalla del resultado y liberación del espacio asignado
    for(int i=0; i<TAM*TAM; i++){
        //printf("[%d]", mC[i]);
        if((i+1)%TAM==0){
        //    printf("\n");
        }
    }

    free(mA);
    free(mB);
    free(mC);
    return FIN;
}
