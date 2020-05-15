/*
 * BlockingStack.c
 *
 * Fixed-size generic array-based BlockingStack implementation.
 *
 */

#include "BlockingStack.h"


sem_t *pop_avaliable, *push_avaliable;
pthread_mutex_t pointer_mutex, size_mutex, pop_mutex, assign_mutex;

/**
 * at the end of the function, unlink all semaphores
 */
static void unlink_semaphores(){
	if(pop_avaliable) sem_close(pop_avaliable);
	if(push_avaliable) sem_close(push_avaliable);
	sem_unlink("pop_avaliable");
	sem_unlink("push_avaliable");
}

/**
 * destory all mutex before termination of the threads
 */
static void destroy_allmutex(){
	pthread_mutex_destroy(&pointer_mutex);
	pthread_mutex_destroy(&size_mutex);
	pthread_mutex_destroy(&assign_mutex);
	pthread_mutex_destroy(&pop_mutex);
}

/**
 * set up all mutex signal to initialized value
 */
static void mutex_startup(){
	pthread_mutex_init(&pointer_mutex,NULL);
	pthread_mutex_init(&size_mutex,NULL);
	pthread_mutex_init(&pop_mutex,NULL);
	pthread_mutex_init(&assign_mutex,NULL);
}
/**
 * set up two semaphores as a signal for pop and push function
 */
static void semaphores_startup(int max_size){
	if((pop_avaliable = sem_open("pop_avaliable",O_CREAT,0666,0)) == SEM_FAILED) printf("pop_available");
	if((push_avaliable = sem_open("push_avaliable",O_CREAT,0666,max_size)) == SEM_FAILED) printf("pop_available");
}

/**
 * this function will be call and create as thread for multi-thread
 * it will wait for signal indicating pushing is avaliable
 * mutex is used for pointer and size protection
 * it take array of arguments one is pointer to stack one is pointer to element
 */
static void *pfunc_push(void** args){

	sem_wait(push_avaliable);
	

	pthread_mutex_lock(&assign_mutex);
	BlockingStack* stack = (BlockingStack*)*(args);
	void* element = *(args+1);
	pthread_mutex_unlock(&assign_mutex);

	pthread_mutex_lock(&size_mutex);
	stack->current_size++;
	pthread_mutex_unlock(&size_mutex);

	pthread_mutex_lock(&pointer_mutex);
	stack->top = new_ListNode(element,stack->top);
	pthread_mutex_unlock(&pointer_mutex);

	pthread_mutex_lock(&assign_mutex);
	free(args);
	pthread_mutex_unlock(&assign_mutex);
	
	sem_post(pop_avaliable);
	

	return NULL;
}

/**
 * the function will be called to be a thread
 * it wait for th signal and pop element into pop queues
 * quite similar to stack, some thread safety measure is considered
 */
static void *pfunc_pop(void* arg){
	sem_wait(pop_avaliable);

	pthread_mutex_lock(&assign_mutex);
	BlockingStack* stack = (BlockingStack*)arg;
	ListNode* currentnode = stack->pop_queue;
	pthread_mutex_unlock(&assign_mutex);

	pthread_mutex_lock(&pointer_mutex);
	ListNode *popedNode = stack->top;
	void *popedElement = popedNode->element;
	stack->top = stack->top->next;
	ListNode_destroy(popedNode);
	pthread_mutex_unlock(&pointer_mutex);

	pthread_mutex_lock(&size_mutex);
	stack->current_size--;
	pthread_mutex_unlock(&size_mutex);
	sem_post(push_avaliable);

	while(true){
		pthread_mutex_lock(&pop_mutex);
		
		if(currentnode->next == NULL){
			currentnode->next = new_ListNode(popedElement,NULL);
			pthread_mutex_unlock(&pop_mutex);

			return NULL;
		}
		currentnode = currentnode->next;
		pthread_mutex_unlock(&pop_mutex);
	}
	


}


/**
 * creating a new blocking stack, initilize the mutex for pointer and size
 * set up new semaphores and some parameter
 */

BlockingStack *new_BlockingStack(int max_size) {
	BlockingStack *this = (BlockingStack*)malloc(sizeof(BlockingStack));
	
	mutex_startup();

	semaphores_startup(max_size);

	this->current_size = 0;
	this->top = NULL;
	this->max_size = max_size;
	this->pop_queue = new_ListNode(NULL,NULL);

	return this;
}


bool BlockingStack_push(BlockingStack* this, void* element) {
	if(element == NULL){
		return false;
	}
	void** args = malloc(2*sizeof(void*));
	*(args) = this;
	*(args + 1 ) = element;  
	pthread_create(&(this->push_thread),NULL,pfunc_push,args);
	return true;
}



void* BlockingStack_pop(BlockingStack* this) {

	pthread_create(&(this->pop_thread),NULL,pfunc_pop,this);

	return this->pop_queue;

}


int BlockingStack_size(BlockingStack* this) {
	return this->current_size;
}


bool BlockingStack_isEmpty(BlockingStack* this) {
	if(this->current_size == 0){
		return true;
	}
	return false;
}


/**
 *  the cleaing function is quite similar to original stack function
 *  some thread saftey function is implemented 
 *  it destory and reset the new semaphores and mutex
 */
void BlockingStack_clear(BlockingStack* this) {
	while(this->current_size>0){
		ListNode *toBeRemoved = this->top;
		
		pthread_mutex_lock(&pointer_mutex);
		this->top = this->top->next;
		pthread_mutex_unlock(&pointer_mutex);
		
		ListNode_destroyElement(toBeRemoved);
		ListNode_destroy(toBeRemoved);
		
		pthread_mutex_lock(&size_mutex);
		this->current_size--;
		pthread_mutex_unlock(&size_mutex);

		unlink_semaphores();
		semaphores_startup(this->max_size);

		destroy_allmutex();
		mutex_startup();
	}
}


void BlockingStack_destroy(BlockingStack* this) {
	BlockingStack_clear(this);
	unlink_semaphores();
	destroy_allmutex();
	free(this);
}
