
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <pthread.h> 

void *reader(void*);
void *writer(void*);

int written = 0;

void *writer(void *vargp){ 
    // ftok to generate unique key
    printf("\nRunning Writer Function....");
    key_t key = ftok("shmfile",65);  
    int shmid;
    char *str = (char*) shmat(shmid,(void*)0,0); 
    while(1){
        if(written==0){
        // shmget returns an identifier in shmid 
        shmid = shmget(key,1024,0666|IPC_CREAT); 
 
        // shmat to attach to shared memory 
        str = (char*) shmat(shmid,(void*)0,0); 
        printf("\n==============Writer Function===============================");
        printf("\nWrite Data : "); 
        fgets(str, sizeof(str), stdin);
        printf("Data written in memory: %s\n",str);
        printf("\n=============================================================");
        written = 1;
        }
    }
    
    //detach from shared memory
    shmdt(str); 

    return NULL;
}

void *reader(void *vargp){
     // ftok to generate unique key 

    printf("\nRunning Reader function....");
    key_t key = ftok("shmfile",65); 

    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    char *str = (char*) shmat(shmid,(void*)0,0); 
    
    while(1){
        

        if(written){
                // shmat to attach to shared memory 
                str = (char*) shmat(shmid,(void*)0,0);
                printf("\n==========================Reader Function=====================");
                printf("\nData read from memory: %s\n",str);
                printf("=================================================================");
                written = 0;
        }
        
    }
    
   
    //detach from shared memory  
    shmdt(str); 

    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 


    return NULL;
}
int main() 
{   
    int ch; 
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, writer, NULL);
    pthread_create(&thread_id, NULL, reader, NULL);
    pthread_join(thread_id, NULL); 
    printf("\nDone");

   
} 
