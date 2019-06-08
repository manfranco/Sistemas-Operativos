#pragma once
#include <pthread.h>

pthread_t recolector;

void *my_entry_function(void *param);

int main(void)
{
    pthread_create(&recolector, NULL, my_entry_function, &parameter);
}
