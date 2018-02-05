// includes
#include <stdio.h>
#include <stdbool.h> // bool, true, false
#include <time.h>    // time_t, time
#include <unistd.h>  // usleep
#include <pthread.h> // pthread_exit, pthread_t, pthread_create
#include <stdlib.h>  // EXIT_SUCCESS, atoi, exit, NULL, RAND_MAX, rand, srand


// global variables
#define MAX_THREADS 300  // max number of threads
int shared_variable = 0; // variable shared between threads


/*
name: is_positive_number
warn: must be a null terminated string
goal: check if string only has numeric characters
uses: stdbool.h
*/
bool is_positive_number(char *string) {  
    int index = 0;
    char character = string[index];
    char null_char = '\0';

    do {
        if (character >= '0' && character <= '9') {
            ++index;
            character = string[index];
            continue;
        } 

        else {
            return false;
        }
    } while (character != null_char);

    return true;
}

/*
name: parse_positive_number
warn: inheriets all of atoi's problems such as overflow/wrapping
goal: convert a string to a positive integer
fail: returns -1 on failure
uses: is_positive_number(), stdlib.h
*/
int parse_positive_number(char *string) {
    if (is_positive_number(string)) {
        return atoi(string);
    }
    else {
        return -1;
    }
}

/*
name: simple_thread
goal: the thread function found in the specs
*/
void *simple_thread(void *thread_id) {
    int value;
    time_t timer;
    long tid = (long)thread_id;

    srand((unsigned) time(&timer));
    for (int number = 0; number < 20; ++number) {
        if (rand() > RAND_MAX / 2) {
            usleep(500);
        }

        value = shared_variable;
        printf("thread(%ld): value = %d\n", tid, value);
        shared_variable = value + 1;
    }

    value = shared_variable;
    printf("thread(%ld): final value = %d\n", tid, value);
    pthread_exit(NULL);
}

/*
name: spawn_function
goal: spawn number of threads declared in command line parameter
*/
void spawn_function(int num_threads) {
    pthread_t threads[num_threads];

    for (long index = 0; index < num_threads; ++index) {
        printf("main: creating thread %ld\n", index);
        int return_code = pthread_create(&threads[index], NULL, &simple_thread, (void *)index);

        if (return_code) {
            printf("main: error: pthread_create() exited with %d\n", return_code);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}

/*
name: error_message
goal: display message to user on error
*/
void error_message() {
    printf("error: Invalid arguments\n");
    printf("\nusage: program <integer>\n");
    printf("where integer is a value between 1 and %d, inclusive\n", MAX_THREADS - 1);
}

/*
name: handle_argument
goal: parse command line argument and use that to spawn threads
uses: spawn_function, error_message
*/
void handle_argument(char *argument) {
    int num_threads = parse_positive_number(argument);

    if (0 < num_threads && num_threads < MAX_THREADS) {
        spawn_function(num_threads);
    }
    else {
        error_message();
    }
}

/*
name: main
goal: define program initial behavior
*/
int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *argument = argv[1];
        handle_argument(argument);        
    }
    else {
        error_message();
    }

    return EXIT_SUCCESS;
}
