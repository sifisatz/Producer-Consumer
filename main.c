/*
 * Author:  Atzamis Iosif 
 * Purpose: Producer Consumer Problem
 * E-mail: csd3094@csd.uoc.gr
 * Language:  C
 */


#include "main.h"

/*
 *	Function : Consume
 *	--------------------------
 *	H sunarthsh auth einai upeuthinh gia thn katanalwsh  
 *  twn paragwmenwn arithmwn. Leitourgei san stoiva,opote xreiazetai 
 *  elegnxos gia to an einai adeia ka8e fora pou travame enan arithmo.	 
 *
 *  @return buffer_t value 
 */
buffer_t consume()
{
    if (buffer_index > 0)
    {
        return buffer[--buffer_index];
    }
    else
    {
        printf("Empty!\n");
    }
    return 0;
}
/*
 *	Function : Insert
 *	--------------------------
 *	H sunarthsh auth einai upeuthinh gia thn eisagwgh enos neou arithmou
 *  sthn stoiva-pinaka buffer[].Leitourgei san stoiva opote tha prepei na 
 *  elegxw kathe fora an exei ginei uperxilisi ths.
 *  
 *  @param buffer_t value : einai o arithmos o opoios tha valei sthn stoiva. 
 *	 
 */
void insert(buffer_t value)
{
    if (buffer_index < SIZE)
    {
        buffer[buffer_index++] = value;
    }
    else
    {
        printf("Full!\n");
    }
}

/*
 *	Function : Producer
 *	--------------------------
 *	H sunarthsh auth einai upeuthinh gia thn paragwgh tuxaiwn arithmwn
 *  mesa sto tuxaio xroniko diastima tou 1 10 miliseconds.
 *	 
 */
void *producer(void *thread_n)
{
    srand(time(NULL));

    int thread_numb = *(int *)thread_n;
    buffer_t value;
    int i = 0;
    while (i < PRODUCER_LOOPS)
    {
        usleep(rand() % 10);
        value = rand() % 100;
        sem_wait(&full_sem);               /* otan to sem einai 0 perimene alliws phgaine kai meiwse to */
        pthread_mutex_lock(&buffer_mutex); /*  If the mutex is already locked, the calling thread blocks 
                                                until the mutex becomes available. */
        prod_val = prod_val + value;
        spiounos++;
        insert(value);
        printf(RED "Producer %d added %d to buffer\n" RESET, thread_numb+1, value);
        i++;
        pthread_mutex_unlock(&buffer_mutex); /*  Release the mutex object referenced by mutex. */
        sem_post(&empty_sem);                // post (increment) emptybuffer semaphore
    }
    pthread_exit(0);
}


/*
 *	Function : Consumer
 *	--------------------------
 *	H sunarthsh auth einai upeuthini gia thn katanalwsh arithmwn oi opoioi
 *  briskontai ston buffer[]. O kathe katanalwths prepei na katalanalwsei oso 
 *  pio grigora mporei arithmous apo ton buffer[] mexri na mhn uparxei allos h' na ton
 *  diakopsei o Producer h' kapoios allos Consumer
 *	 
 */
void *consumer(void *thread_n)
{
    int thread_numb = *(int *)thread_n;
    buffer_t value;
    int i = 1, j;
    while (1)
    {
        sem_wait(&empty_sem);
        if (buffer_index == 0)
            break;

        pthread_mutex_lock(&buffer_mutex); /*  If the mutex is already locked, the calling thread blocks 
                                                until the mutex becomes available. */
        value = consume(value);
        cons_val[thread_numb] = value + cons_val[thread_numb];
        /*for (j = 0; j < katanalwtes; j++)
        {
            printf("Consumer %d : %d\n", j, cons_val[j]);
        }*/
        printf(GRN "Consumer %d dequeue %d from buffer\n" RESET, thread_numb+1, value);
        pthread_mutex_unlock(&buffer_mutex); /*  Release the mutex object referenced by mutex. */
        sem_post(&full_sem);                 // post (increment) fullbuffer semaphore
    }
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    int producer_input, consumer_input;
    int total_threads;
    if (atoi(argv[2]) <= 0)
    {
        printf(RED "Wrong argvs\n" RESET);
        return 0;
    }
    if (atoi(argv[4]) <= 0)
    {
        printf(RED "Wrong argvs\n" RESET);
        return 0;
    }
    producer_input = atoi(argv[2]);
    consumer_input = atoi(argv[4]);

    cons_val = malloc(sizeof(int) * consumer_input);
    katanalwtes = consumer_input;

    total_threads = producer_input + consumer_input;
    buffer_index = 0;

    pthread_mutex_init(&buffer_mutex, NULL);

    /*full_sem arxika tha exei timh ish me buffer SIZE,
    opou shmainei oti tosoi osoi  to SIZE apo producer mporoun
    na grapsoun kathe fora.*/

    sem_init(&full_sem,
             0,
             SIZE);
    /*empty_sem 
    arxika tha exei timh 0 opou shmainei oti kanenas consumer den 
    mporei na katanalwsei sthn sunexeia tha megalwnei sumfwna me ta
    thread producer pou exoun paraksei diafora values afou prwta kanei post o 
    producer thread ena empty_sem.*/
    sem_init(&empty_sem,
             0,
             0);
    pthread_t thread[total_threads];
    int thread_numb[total_threads];
    int i, j;
    for (i = 0; i < producer_input; i++)
    {
        thread_numb[i] = i;
        pthread_create(&thread[i],       // pthread_t *t
                       NULL,             // const pthread_attr_t *attr
                       producer,         // void *(*start_routine) (void *)
                       &thread_numb[i]); // void *arg
    }
    for (i = 0; i < consumer_input; i++)
    {
        thread_numb[i + producer_input] = i;
        pthread_create(&thread[i + producer_input],       // pthread_t *t
                       NULL,                              // const pthread_attr_t *attr
                       consumer,                          // void *(*start_routine) (void *)
                       &thread_numb[i + producer_input]); // void *arg
    }

    /*
    Producer threads creation
    */
    for (i = 0; i < producer_input; i++)
    {
        pthread_join(thread[i], NULL);
    }

    for (j = 0; j < (katanalwtes); j++)
    {
        sem_post(&empty_sem);
    }

    /*
    Consumer threads creation
    */
    for (i = 0; i < consumer_input; i++)
    {
        pthread_join(thread[i + producer_input], NULL);
    }
    for (j = 0; j < katanalwtes; j++)
    {
        printf(YEL "FINAL Consumer %d : %d\n" RESET, j + 1, cons_val[j]);
    }

    printf(YEL "FINAL Production nums : %d\n" RESET, prod_val);

    /*
    Destroy part
    */

    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&full_sem);
    sem_destroy(&empty_sem);

    return 0;
}
