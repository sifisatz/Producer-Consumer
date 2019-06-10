/*
 * Author:  Atzamis Iosif 
 * Purpose: Producer Consumer Problem
 * E-mail: csd3094@csd.uoc.gr
 * Language:  C
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 1000000
#define PRODUCER_LOOPS 1000000
#define RED "\x1B[31m"
#define YEL "\x1B[33m"
#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

typedef int buffer_t;
buffer_t buffer[SIZE];
int buffer_index = 0;
int *cons_val;
int prod_val;
int spiounos = 0;
int katanalwtes;
int consumer_loops = 0;
pthread_mutex_t buffer_mutex;
/*
arxika o buffer tha einai empty.
*/

/*full_sem arxika tha exei timh ish me buffer SIZE,
    opou shmainei oti tosoi osoi  to SIZE apo producer mporoun
    na grapsoun kathe fora.*/
sem_t full_sem;  /* when 0, buffer is full */

/*empty_sem 
    arxika tha exei timh 0 opou shmainei oti kanenas consumer den 
    mporei na katanalwsei sthn sunexeia tha megalwnei sumfwna me ta
    thread producer pou exoun paraksei diafora values afou prwta kanei post o 
    producer thread ena empty_sem.*/
sem_t empty_sem; /* when 0, buffer is empty. */


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
void insert(buffer_t value);
/*
 *	Function : Consume
 *	--------------------------
 *	H sunarthsh auth einai upeuthinh gia thn katanalwsh  
 *  twn paragwmenwn arithmwn. Leitourgei san stoiva,opote xreiazetai 
 *  elegnxos gia to an einai adeia ka8e fora pou travame enan arithmo.	 
 *
 *  @return buffer_t value 
 */
buffer_t consume();
/*
 *	Function : Producer
 *	--------------------------
 *	H sunarthsh auth einai upeuthinh gia thn paragwgh tuxaiwn arithmwn
 *  mesa sto tuxaio xroniko diastima tou 1 10 miliseconds.
 *	 
 */
void *producer(void *thread_n);
/*
 *	Function : Consumer
 *	--------------------------
 *	H sunarthsh auth einai upeuthini gia thn katanalwsh arithmwn oi opoioi
 *  briskontai ston buffer[]. O kathe katanalwths prepei na katalanalwsei oso 
 *  pio grigora mporei arithmous apo ton buffer[] mexri na mhn uparxei allos h' na ton
 *  diakopsei o Producer h' kapoios allos Consumer
 *	 
 */
void *consumer(void *thread_n);