#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5                 /* número de filósofos (y de tenedores) */

pthread_mutex_t forks[N];   /* un mutex por tenedor */

/* ---------- Rutina de hilo ------------------------------------------------ */
void *philosopher(void *arg)
{
    int id    = *(int *)arg;        /* identificador 0..4         */
    int left  = id;                 /* tenedor a la izquierda      */
    int right = (id + 1) % N;       /* tenedor a la derecha        */

    while (1)
    {
        /* 1. Pensar -------------------------------------------------------- */
        printf("Filósofo %d piensa...\n", id);
        sleep(rand() % 3);          /* demora aleatoria (0–2 s)    */

        /* 2. Hambre -------------------------------------------------------- */
        printf("Filósofo %d tiene hambre.\n", id);

        /* 3. Tomar tenedores (sección propensa a deadlock) ----------------- */
        pthread_mutex_lock(&forks[left]);
        printf("Filósofo %d toma tenedor izquierdo (%d).\n", id, left);

        /* pausa para sincronizar a todos sosteniendo su tenedor izquierdo   */
        sleep(1);

        pthread_mutex_lock(&forks[right]);
        printf("Filósofo %d toma tenedor derecho (%d).\n", id, right);

        /* 4. Comer --------------------------------------------------------- */
        printf("Filósofo %d come.\n", id);
        sleep(2);

        /* 5. Dejar los tenedores ------------------------------------------ */
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
    }
    return NULL;
}

/* ---------- Programa principal ------------------------------------------- */
int main(void)
{
    pthread_t tid[N];
    int ids[N];

    /* Inicializa cada tenedor (mutex) */
    for (int i = 0; i < N; ++i)
        pthread_mutex_init(&forks[i], NULL);

    /* Crea un hilo por filósofo */
    for (int i = 0; i < N; ++i)
    {
        ids[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &ids[i]);
    }

    /* Espera (realmente nunca termina porque habrá deadlock) */
    for (int i = 0; i < N; ++i)
        pthread_join(tid[i], NULL);

    return 0;
}