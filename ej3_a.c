#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "gen_matriz.h" 

//Definiciones del variable de cálculo random y los valores de salida
#define MAX 100
#define ERR -1
#define FIN 0

int main(int argc, char** argv) {
    
    //Declaración de las variables
    int world_rank, world_size, i, j, k, r;
    FILE * handle1;  

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Comprobación de que hay mínimo dos procesadores
    if (world_size < 2) {
        fprintf(stderr, "World tiene que ser >= 2%s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    //Comprobación de que la llamada se realiza bien
    if(argc!=2){
        printf("Error! ./nombre TAM \n");
        return(ERR);
    }
    
    //Comprobación de que el número de procesadores no supera el tamaño
    int TAM = atoi(argv[1]);
    if(TAM<=world_size){
        printf("Error! la cantidad de procesadores no puede superar al tamaño");
        return(ERR);
    }
    
    //Declaración e inicialización de las matrices
    int *mA, *mB, *mC, *mT1, *mT2;
    mA=malloc(TAM*TAM*sizeof(int));
    mB=malloc(TAM*TAM*sizeof(int));
    mC=malloc(TAM*TAM*sizeof(int));
    
    if(world_rank==0){
	int aux;
    //Llamada a la función generadora de matrices
        r=generar(TAM);
        handle1=fopen("matrix.txt","r");
        //Comprobar que  se habre el fichero
	if(handle1==NULL){
        printf("ERROR! No se pudo abrir el documento");
        return (ERR);
    }
    //Lectura de las matrices del fichero
        for(i=0; i<TAM*TAM; i++){//Leer la matriz A desde el fichero
            fscanf(handle1, "%d", &aux);
            mA[i]=aux;
        }
	for(i=0; i<TAM*TAM; i++){//Leer la matriz B desde el fichero
            fscanf(handle1, "%d", &aux);
            mB[i]=aux;
        }
        //Cerrar el fichero
	fclose(handle1);
    }
    
   mT1=malloc(((TAM*TAM)/world_size)*sizeof(int));
    
    //Scatter de las líneas de la matriz A y Broadcast de la matriz B en su conjunto
    MPI_Scatter(&mA[0], (TAM*TAM)/world_size, MPI_INT, mT1, (TAM*TAM)/world_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mB[0], TAM*TAM, MPI_INT, 0, MPI_COMM_WORLD);

     //Declaración e inicialización de las matriz temporal
    int *mT;
    mT=malloc(((TAM*TAM)/world_size)*sizeof(int));

    //Realización de los cálculos
    for(i=0; i<TAM/world_size; i++){//TAM/world_size da como resultado el número de filas de A que se han mandado
        for (j=0; j<TAM; j++){//Puntero del vector-matriz del resultado
            int suma = 0;
            for (k=0; k<TAM; k++){//Puntero de localización de la suma
                suma += mT1[i*TAM+k] * mB[j*TAM+k];////De esta forma se cambia de fila y se corre entera
            }
            mT[i*TAM+j] = suma;//De esta forma se registran todos los resultados
        }
    }

    //Gather, recuperación de todos los datos mandados de vuelta en el proceso 0
    MPI_Gather(&mT[0], (TAM*TAM)/world_size, MPI_INT, mC, (TAM*TAM)/world_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    //Impresión por pantalla, liberación del espacio asignado y escritura del resultado en un fichero
    if(world_rank==0){
        FILE *f1;
        f1=fopen("resultado.txt","w");
        if (f1==NULL){
            printf("ERROR! No se pudo abrir el documento");
            return ERR;
        }
    	for(i=0; i<TAM*TAM; i++){
       		//printf("[%d]", mC[i]);
            	fprintf(f1,"%d ",mC[i]);
        	if((i+1)%TAM==0){
        	//    	printf("\n");
                   	fprintf(f1,"\n");
        	}
    	}
	fclose(f1);
    //printf("Tiempo de ejecución: %ld segundos.\n", elapsed);
    	free(mA);
    	free(mB);
    	free(mC);
    }
    MPI_Finalize();
    return(FIN);
}
