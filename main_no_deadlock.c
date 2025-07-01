#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5                   /* número de filósofos (y tenedores) */

pthread_mutex_t forks[N];     /* un mutex por tenedor           */
sem_t room;                   /* mozo: aforo máximo N-1         */

/* ---------- Rutina de hilo ---------------------------------------------- */
void *philosopher(void *arg)
{
    int id    = *(int *)arg;
    int left  = id;                 /* tenedor izquierdo             */
    int right = (id + 1) % N;       /* tenedor derecho               */

    while (1)
    {
        /* 1. Pensar ------------------------------------------------------ */
        printf("Filósofo %d piensa...\n", id);
        sleep(rand() % 3);

        /* 2. Hambre ------------------------------------------------------ */
        printf("Filósofo %d tiene hambre.\n", id);

        /* 3. Solicitar permiso al mozo (semáforo) ------------------------ */
        sem_wait(&room);            /* ↓ reserva un puesto en la mesa */

        /* 4. Tomar ambos tenedores (ya sin riesgo de deadlock) ----------- */
        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);

        /* 5. Comer ------------------------------------------------------- */
        printf("Filósofo %d come.\n", id);
        sleep(2);

        /* 6. Soltar tenedores y avisar al mozo --------------------------- */
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
        sem_post(&room);            /* ↑ libera el puesto             */
    }
    return NULL;
}

/* ---------- Programa principal ------------------------------------------ */
int main(void)
{
    pthread_t tid[N];
    int ids[N];

    /* Inicializa recursos */
    for (int i = 0; i < N; ++i)
        pthread_mutex_init(&forks[i], NULL);

    sem_init(&room, 0, N - 1);   /* aforo = 4                       */

    /* Crea un hilo por filósofo */
    for (int i = 0; i < N; ++i)
    {
        ids[i] = i;
        pthread_create(&tid[i], NULL, philosopher, &ids[i]);
    }

    /* Espera (en la práctica nunca termina) */
    for (int i = 0; i < N; ++i)
        pthread_join(tid[i], NULL);

    return 0;
}
