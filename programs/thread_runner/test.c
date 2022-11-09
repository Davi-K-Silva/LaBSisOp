#include <stdio.h> 
#include <pthread.h> 
#include <time.h> 
 
 
#define NUM_BARRIERS 5 
#define NUM_THREADS 5 
 
 
#define LIMITE 100 
 
 
pthread_barrier_t barrier; 
pthread_t threads[NUM_THREADS]; 
 
 
 
 
void* dorme_espera(void *arg){ 
    int retorno, j, tempo; 
    int thread_id = (int) arg; 
    int aux=0; 
 
 
    while(aux<=LIMITE){ 
    aux+=thread_id+1; 
    tempo = rand()%10000; 
    printf("Thread %d esperando %d milisegundos com o valor atual de %d.\n", thread_id, tempo, aux); 
        usleep(tempo); 
    } 
 
 
    printf("Thread %d esperando na barreira\n", thread_id); 
    pthread_barrier_wait(&barrier); 
usleep(100000); 
printf("Thread %d passou da barreira\n", thread_id); 
} 
 
 
int main() { 
    int i; 
srand(time(NULL)); 
    if (pthread_barrier_init(&barrier, NULL, NUM_THREADS))  
printf("Impossivel criar barreira"); 
  for (i = 0; i < NUM_THREADS; i++) { 
    if (pthread_create(&threads[i], NULL, dorme_espera, (void*) i)) 
printf("Erro na criacao das threads"); 
    } 
    usleep(100000); 
pthread_exit(NULL); 
}