/***
 *          _       _                _          _ _ 
 *      ___| | __ _(_)_ __ ___   ___| |__   ___| | |
 *     / __| |/ _` | | '__/ _ \ / __| '_ \ / _ \ | |
 *    | (__| | (_| | | | |  __/ \__ \ | | |  __/ | |
 *     \___|_|\__,_|_|_|  \___| |___/_| |_|\___|_|_|
 *
 *      (Named after a small baby coming this May)
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_ARG_LEN		1024
#define MAX_ARG_NUM		50

int pid;            // Process ID
int nosigAlrm = 0;  // Flag for printing main messages.

/* Signal Handler */

void sigHandler (int signum){
    if(signum == SIGALRM){
        if(kill(pid, SIGALRM) == -1){
            // killing didn't work
            nosigAlrm = 1;
        } else{
            // killed lengthy process
            write(0, "Dirty diaper eradicated!\n", 25);
            nosigAlrm = 0;
        }
    }
    
    return;
}

/* Here we go! */

int main(int argc, char *argv[]){
    int alrmLen;

    if(argc == 2)
        alrmLen = atoi(argv[1]);	// Commands run for this long
    else
        alrmLen = 0;

    /* Input */
    ssize_t numOfRead;
    char rawInput[MAX_ARG_LEN];		// Command
    char *paramVec[MAX_ARG_NUM];	// Paramaters
    char *ptr;                      // Used for parsing out params
    char *ptrHead;                  // Used for parsing out params
    int i;                 // Commane parameters! Yay!

    /* Shell loop */
    while(1){
        i = 0;

        write(0, "csh$ ", 5);
        numOfRead = read(1, rawInput, MAX_ARG_LEN);

        // skip bad input	
        if(numOfRead == -1) continue;
        if(numOfRead >= MAX_ARG_LEN) continue;

        // truncate return char '\n'
        rawInput[numOfRead-1] = '\0';

        // avoid empty input
        if(rawInput[0] == '\0') continue;

        // Does user wish to exit?
        if (strncasecmp(rawInput, "exit", strlen("exit")) == 0)
            return 0;

        /* Parse input */
        ptrHead = rawInput;
        ptr = rawInput;
        while(*ptr != '\0'){
            if(*ptr == ' '){
                *ptr = '\0';
                paramVec[i] = ptrHead;
                ptr++;
                ptrHead = ptr;
                i++;
            } else{
                ptr++;
            }
        }
        paramVec[i] = ptrHead;
        i++;
        paramVec[i] = NULL;

        /* Fork & execute */

        pid = fork();

        if(pid < 0){
            perror("fork failed");
        } else if(pid == 0) {
            if(execve(paramVec[0], paramVec, NULL) == -1){
                perror("execve failed");
                exit(1);
            }

        } else {
            if(alrmLen != 0) { 
                signal(SIGALRM, sigHandler);
                alarm(alrmLen);
            }

            wait(NULL);
            if(nosigAlrm == 1){
                write(0,"Diaper change failed! Baby is crying.\n", 39);
            } else {
                nosigAlrm = 1;
            }
            
        }

    }
}
