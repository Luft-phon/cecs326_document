#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>


using namespace std;

struct Product {
    int producerID;
    int productNumber;
};

const int BUFFER_SIZE = 10;
const int NUM_ITEMS = 20;
int producer_count[3] = {0,0,0};
int consumer_count[3] = {0,0,0};

bool produceFinished = false;

Product buffer[BUFFER_SIZE];

int in = 0;
int out = 0;
int count = 0;  // the number of items in the buffer

// Semaphores
sem_t mutex_sem;
sem_t empty_sem;
sem_t full_sem;

void* producer(void* arg) {
    Product product;

    sem_wait(&mutex_sem);
    int id = *(int*)arg;
    cout << "Producer " << id << " started."<< endl;
    cout << "Thread ID: " << pthread_self() << endl;
    sem_post(&mutex_sem);
    
    for (int item = 1; item <= NUM_ITEMS; item++) {
        product.producerID = id;
        product.productNumber = item;
        
        // ============================
        // TODO
        //
        // Wait until the buffer has an empty slot.
        sem_wait(&empty_sem);
        // Enter the critical section.
        sem_wait(&mutex_sem);
        // ============================

        buffer[in] = product;
        in = (in + 1) % BUFFER_SIZE;
        count++;        // increment the number of items in the buffer when an item is produced

        producer_count[id-1]++;


        if (count > BUFFER_SIZE){
            cerr << "Buffer is overflowed." << endl;
        }

        cout << "Producer " << product.producerID << " produced Product: " << product.productNumber << "| Buffer count: " << count << endl;
        // cout << "Produced: " << item << endl;
        
        // ============================
        // TODO
        // 
        // Leave the critical section.
        sem_post(&mutex_sem);       // signal()
        // Notify the consumer that an item is available.
        sem_post(&full_sem);  
        // ============================

        sleep(rand() % 3);
    }

    return nullptr;
}

void* consumer(void* arg) {
    Product product;

    int id = *(int*)arg;
    cout << "Consumer " << id << " started." << endl;
    cout << "Thread ID: " << pthread_self() << endl;

    while(true) {

        // ============================
        // TODO
        //
        // Wait until an item exists.
        sem_wait(&full_sem);
        // Enter the critical section.
        sem_wait(&mutex_sem);
        // ============================

        // exit consumer loop when no producers are producing and the buffer is empty
        if (produceFinished && count == 0) {
            sem_post(&mutex_sem);
            break;
        }

        product = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;    // decrement the number of items in the buffer when an item is consumed

        if (count < 0){
            cerr << "Buffer is underflowed." << endl;
        }

        cout << "Consumer " << id << " consumed Product: " << product.productNumber << " from Producer " << product.producerID << "| Buffer count: " << count << endl;

        consumer_count[id-1]++;
        // ============================
        // TODO
        //
        // Leave the critical section.
        sem_post(&mutex_sem);       // signal()
        // Notify the producer that one slot is empty.
        sem_post(&empty_sem);
        // ============================

        sleep(rand() % 3);
    }

    return nullptr;
}

void printBuffer() {
    cout << "============================" << endl;
    cout << "Execution Summary" << endl;
    cout << "============================" << endl;
    cout << "Products Produced: " << producer_count[0] + producer_count[1] + producer_count[2] << endl;
    cout << "Products Consumed: " << consumer_count[0] + consumer_count[1] + consumer_count[2] << endl;
    cout << endl;
    cout << "Producer 1: " << producer_count[0] << endl;
    cout << "Producer 2: " << producer_count[1] << endl;
    cout << "Producer 3: " << producer_count[2] << endl;
    cout << endl;
    cout << "Consumer 1: " << consumer_count[0] << endl;
    cout << "Consumer 2: " << consumer_count[1] << endl;
    cout << "Consumer 3: " << consumer_count[2] << endl;
    cout << endl;   
    if (count == 0){
        cout << "Buffer Empty." << endl;
    }
}

int main() {

    pthread_t producerThread[3];
    pthread_t consumerThread[3];

    int producerID[3];
    int consumerID[3];

    // ============================
    // TODO
    //
    // Initialize:
    //   mutex_sem
    //   empty_sem
    //   full_sem
    // ============================

    sem_init(&mutex_sem, 0, 1);      // binary semaphore
    sem_init(&empty_sem, 0, BUFFER_SIZE);   // counting semaphore
    sem_init(&full_sem, 0, 0);     // counting semaphore


    for (int i = 0; i < 3; i++) {
        producerID[i] = i + 1;
        pthread_create(&producerThread[i], nullptr, producer, &producerID[i]);
    }

    for (int i = 0; i < 3; i++) {
        consumerID[i] = i + 1;
        pthread_create(&consumerThread[i], nullptr, consumer, &consumerID[i]);
    }
    
    // wait until the producer_costumer thread finish execution.
    for(int i = 0; i < 3; i++){
        pthread_join(producerThread[i], NULL);
    }

    // All producer threads have completed.
    sem_wait(&mutex_sem);
    produceFinished = true;
    sem_post(&mutex_sem);

    // Wake up all consumer threads so they can check the exit condition when producer finished.
    for (int i = 0; i < 3; i++) {
        sem_post(&full_sem);
    }

    for(int i = 0; i < 3; i++){
        pthread_join(consumerThread[i], NULL);
    }

    printBuffer();

    // ============================
    // TODO
    //
    // Destroy:
    //   mutex_sem
    //   empty_sem
    //   full_sem
    sem_destroy(&mutex_sem);
    sem_destroy(&empty_sem);
    sem_destroy(&full_sem);
    // ============================


    cout << "\nProgram completed successfully." << endl;

    return 0;
}
