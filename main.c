#include <stdio.h> //To printf()
#include <stdlib.h> // For exit()
#include <unistd.h> // For sleep()
#include <pthread.h> //To implement pThreads and Mutex

# define ARRAY_SIZE 100 //Defines constat value for array size

//Global Variables/////////////////////////////////////////////////////
int cicles = -1000; //To ensure a finite amount of cicles (prevent infinte loop and print buffer "bravado")
char shared_Array [ARRAY_SIZE]; //Array to be shared between the threads
int pos = 0; //To navigate array and prevent out of bonds exception
//////////////////////////////////////////////////////////////////////

pthread_mutex_t mutex; //Mutex created

void *generate_Random_Value(void *vargp) 
{ 
    while (cicles++){
        sleep(0.01); //Slepp for 1/100 seconds (10 miliseconds)
        pthread_mutex_lock(&mutex);//Locks mutex
        if(pos + 1 > ARRAY_SIZE){ //Compares if there is out of bonds chance and the procedes to restart pos if so 
            pos = 0; //Restarts pos value to begin to write to the begining of the list anew
        }
        shared_Array[pos++] = rand() % 255; //%255 ensures that random will throw a number between 0 and 255
        pthread_mutex_unlock(&mutex);//Releases the lock   
    } 
} 

void *XOR_function(void *vargp) 
{ 
    while (cicles){
        if(pos < ARRAY_SIZE){
            pthread_mutex_lock(&mutex);//Locks mutex
            char current_Value = shared_Array[pos++]; //Increments pos to avoid repetitions 
            pthread_mutex_unlock(&mutex); //Releases the lock
            current_Value ^= 0x20; //XOR in C with integer variable
            printf("%c \n", current_Value); //Prints to terminal/console the XOR of the value in the array
        }
    }
} 
   
int main() 
{ 
    pthread_t thread_id; //Thread id Container
    pthread_create(&thread_id, NULL, generate_Random_Value, NULL); //Creates the thread for the random number generator
    pthread_create(&thread_id, NULL, XOR_function, NULL);  //Creates the thread for the XOR and print function
    pthread_join(thread_id, NULL); //Thread synchronization
    exit(0); //Exit 
}