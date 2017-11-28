#include<pthread.h>
#include<unistd.h>
#include<cstdio>
#include<algorithm>
#include<ctime>

using namespace std;

int meat, food;
pthread_mutex_t food_mutex = PTHREAD_MUTEX_INITIALIZER, meat_mutex = PTHREAD_MUTEX_INITIALIZER;
int days_c[1000], days_h[1000];
pthread_t cooks[1000], hunters[1000];
int day_ms = 1;
int days = 0;
#define DEBUG 0

void* hunter(void* x)
{
    #if DEBUG
        long id = (long) x;
    #endif
    for (int i = 0; i < 365; i++)
    {
        int a = rand()%6;
        int b = rand()%6;
        if (a > b)
        {
            pthread_mutex_lock(&meat_mutex);
            meat++;
            #if DEBUG
                printf("Hunter %ld hunts. Meat: %d\n", id, meat);
            #endif
            pthread_mutex_unlock(&meat_mutex);
        }
        
        pthread_mutex_lock(&food_mutex);
        if (food)
        {
            food--;
            #if DEBUG
                printf("Hunter %ld eats. Food: %d\n", id, food);
            #endif
            pthread_mutex_unlock(&food_mutex);
        }
        else
        {
            pthread_mutex_unlock(&food_mutex);
            #if DEBUG
                printf("Hunter %ld left after %d days.\n", id, days_h[id]);
            #endif
            pthread_exit(NULL);
        }
        #if DEBUG
            days_h[id]++;
        #endif
        usleep(1000 * day_ms);
    }
    pthread_exit(NULL);
}

void* cook(void* x)
{
    #if DEBUG
        long id = (long) x;
    #endif
    for (int i = 0; i < 365; i++)
    {
        bool ok = false;
        pthread_mutex_lock(&meat_mutex);
        if (meat)
        {
            meat--;
            #if DEBUG
                printf("Cook %ld takes meat. Meat: %d\n", id, meat);
            #endif
            ok = true;
        }
        pthread_mutex_unlock(&meat_mutex);
        
        if (ok)
        {
            pthread_mutex_lock(&food_mutex);
            food += rand()%6 + 1;
            #if DEBUG
                printf("Cook %ld cooks. Food: %d\n", id, food);
            #endif
            pthread_mutex_unlock(&food_mutex);
        }
        
        pthread_mutex_lock(&food_mutex);
        if (food)
        {
            food--;
            #if DEBUG
                printf("Cook %ld eats. Food: %d\n", id, food);
            #endif
            pthread_mutex_unlock(&food_mutex);
        }
        else
        {
            pthread_mutex_unlock(&food_mutex);
            #if DEBUG
                printf("Cook %ld left after %d days.\n", id, days_c[id]);
            #endif
            pthread_exit(NULL);
        }
        #if DEBUG
            days_c[id]++;
        #endif
        usleep(1000 * day_ms);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    srand(time(NULL));   
    int c = 0, h = 0;
    h = atoi(argv[1]);
    c = atoi(argv[2]);
    meat = atoi(argv[3]);
    food = atoi(argv[4]);
    
    for (long i = 0; i < h; i++)
        pthread_create(&hunters[i], NULL, hunter, (void*) i);
        
    for (long i = 0; i < c; i++)
        pthread_create(&cooks[i], NULL, cook, (void*) i);
        
    for (int i = 0; i < h; i++)
        pthread_join(hunters[i], NULL);
        
    for (int i = 0; i < c; i++)
        pthread_join(cooks[i], NULL);
    
    return 0;
}

