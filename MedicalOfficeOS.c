#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PATIENTS 10
#define NUM_DOCTORS 2
#define MAX_CONSULT_TIME 10

pthread_mutex_t doctor_mutex[NUM_DOCTORS];

void *patient_thread(void *arg) 
{
    int id = *((int *)arg);
    int doctor_id;
    int consult_time;
    int wait_time = 0;

    
    // Gasim un doctor disponibil
    for (doctor_id = 0; doctor_id < NUM_DOCTORS; doctor_id++) 
    {
        if (pthread_mutex_trylock(&doctor_mutex[doctor_id]) == 0)
            break;
        wait_time++;
    }

    if (doctor_id == NUM_DOCTORS)
        // Toti doctorii sunt ocupati, atunci asteptam o perioada random de timp
        sleep(rand() % 5);

    // Consultatie cu doctor
    consult_time = rand() % MAX_CONSULT_TIME + 1;
    sleep(consult_time);

    // Doctorul este eliberat
    pthread_mutex_unlock(&doctor_mutex[doctor_id]);

    printf("Patient %d waited %d seconds and had a %d second consultation with doctor %d\n",
            id, wait_time, consult_time, doctor_id);

    return NULL;
}

int main() 
{
    int i;
    int patient_id[NUM_PATIENTS];
    pthread_t patient_threads[NUM_PATIENTS];

    // Initializam doctor mutexes
    for (i = 0; i < NUM_DOCTORS; i++)
        pthread_mutex_init(&doctor_mutex[i], NULL);

    // Creem threaduri pacient
    for (i = 0; i < NUM_PATIENTS; i++) 
    {
        patient_id[i] = i;
        pthread_create(&patient_threads[i], NULL, patient_thread, &patient_id[i]);
    }

    // Asteptam ca threadurile pacienti sa se termine
    for (i = 0; i < NUM_PATIENTS; i++)
        pthread_join(patient_threads[i], NULL);

    // Distrugem doctor mutexes
    for (i = 0; i < NUM_DOCTORS; i++) {
        pthread_mutex_destroy(&doctor_mutex[i]);
    }

    return 0;
}