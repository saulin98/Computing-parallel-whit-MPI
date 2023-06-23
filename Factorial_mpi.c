#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

//Autor: Saúl Fosado Velázquez

struct timeval tv;
struct timezone tz;
double time_start,time_end;

long long factorial(int num){
	
	long long resultado=1;
	int i;
	
	if(num==0 || num==1){
		return resultado;
	}
	else{
		
		for(i=1;i<num;i++){
		resultado = resultado * i;
	}
		
		return resultado;
	}
		
}

int main(int argc,char **argv){



   int arr[1000000];
   int i;
   int id,npr;
   

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&id);
MPI_Comm_size(MPI_COMM_WORLD,&npr);

int tam_parcial=1000000/(npr-1);

int inicio=0;

if(id==0){
	
	gettimeofday(&tv,&tz);
    time_start= (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;

   printf("Comenze a llenar\n");
   for(i=0;i<1000000;i++){
	arr[i]=rand() % 100001;
    }
    printf("Termine de llenar\n");
	
   //Envio datos del servidor a los clientes
   for(i=1;i<npr;i++){
   MPI_Send(&arr[inicio],tam_parcial,MPI_INT,i,0,MPI_COMM_WORLD);
   inicio=inicio + tam_parcial;
}

int notificaciones[tam_parcial];

	//Recibo respuestas de los clientes
for(i=1;i<npr;i++){
	MPI_Status estado;	
	MPI_Recv(&notificaciones[0],tam_parcial,MPI_INT,i,0,MPI_COMM_WORLD,&estado);
}

printf("Todos los numeros han sido procesados\n");

gettimeofday(&tv,&tz);
time_end= (double)tv.tv_sec + (double)tv.tv_usec/1000000.0;
printf("Tiempo de ejecución:%lf\n",time_end - time_start);
}

else{
	
	//Recibo los datos envidos por el servidor
int recibidos[tam_parcial];	
MPI_Status estado;	
MPI_Recv(&recibidos[0],tam_parcial,MPI_INT,0,0,MPI_COMM_WORLD,&estado);

int banderas[tam_parcial];	
	
for(i=0;i<tam_parcial;i++){
	long long res=factorial(recibidos[i]);
	banderas[i]=1;
}
	//Envio respuestas al servidor
MPI_Send(&banderas[0],tam_parcial,MPI_INT,0,0,MPI_COMM_WORLD);

}


MPI_Finalize();

return 0;
 }
