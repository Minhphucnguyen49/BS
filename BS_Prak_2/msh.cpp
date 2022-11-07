
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>		// open()
#include <signal.h>			// signal()
#include <fcntl.h>			// open()
#include <stdio.h>			// printf(), ...
#include <time.h>				// time(), ...
#include <string.h>			// strtok()
#define MAXLINE 100
#define PATHLENGTH 1000
#define MOD "exit with CTR C \n"

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include<iostream>
using namespace std;

time_t tstart{0};
time_t tend{0};
time_t ttotal{};

void convertTime(time_t time){
    int hours = time % 86400 / 3600;
    int minutes = time % 3600 / 60;
    int seconds = time % 60;

    printf("\nTime elepsed %dh %dm %ds\n", hours, minutes, seconds);
}

void handler(int signal) {
    //printf("handler\n");
    time(&tend);
    ttotal = (tend-tstart);
    convertTime(ttotal);
    exit(0);
}

bool commandinBackround(char *parameters[]){
    for (size_t i = 0; parameters[i] != NULL; i++)
    {
        if(*parameters[i] == '&'){
            parameters[i] = NULL;
            return true;
        }
    }
    return false;
}

void sigchld_hdl(int sig) {

// Signal = Ereignis auf dass der Prozess reagieren wird
// Vater wartet auf Kind, soll also auf die Beendigung eines bestimmten Prozesses warten
// -1 = auf beliebigen Prozess warten, äquvalent zu wait()
// normalerweise steht da die pid vom Kind, auf den man wartet
// WNOHANG: Der aufrufende Prozess wird nicht blockiert, wenn der Prozess "pid" noch nicht beendet wurde bzw. noch nicht im Zombie-Status ist
// sagt, dass man nicht wartet

    while (waitpid(-1, NULL , WNOHANG) > 0);


}
char* get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir( buff, FILENAME_MAX );
    char* current_working_dir(buff);
    return current_working_dir;
}

// read_command

// 1. Parameter: Zeiger auf CSTring, wo das Kommando drin (z.B. echo)
// 2. Parameter: Array von Pointer auf CStrings (wo die Parameter drin stehen)
int read_command(char *command, char *parameters[]) { // prompt for user input and read a command line
    char workingDirectory[PATHLENGTH];

    getcwd(workingDirectory, sizeof(workingDirectory));
    fprintf(stdout, "%s> ", workingDirectory); //promt

    int noParam = 0;
    string input;
    char *token;

    //signal(SIGINT, handler);

    char charInput[MAXLINE];
    getline(cin, input);
    cin.clear();

    strcpy(charInput, input.c_str());
    charInput[sizeof(charInput) - 1] = 0;

    token = strtok(charInput, " ");

    strcpy(command, token);
    parameters[0] = command;

    int index = 1;
    while (token != NULL){
        token = strtok(NULL, " ");
        if(token != NULL) {
            parameters[index] = new char[MAXLINE];
            strcpy(parameters[index], token);
            index++;
        }
    }
    parameters[index] = NULL;
    if (command != NULL) //if command is there
        noParam = -1; // != 0 means it will not come into the if condition (noParams == 0)

    return(noParam);
} // read_command

int main(int argc, char *argv[]) {
    int childPid;
    int status;
    char command[MAXLINE];
    char *parameters[MAXLINE];
    int noParams;
    bool isCommandInBackround = false;

    time(&tstart);
    signal(SIGINT, handler);
    fprintf(stdout, "%s\n", MOD);	// exit with CTR C

    signal(SIGCHLD , sigchld_hdl); // Trigger wird hier 1 mal declaiert mehr nicht. //Zombies wird durch den Befehl entfernt.
    while (1) {
        noParams = read_command(command, parameters); // read user input
        isCommandInBackround = commandinBackround(parameters); //Check ob & eingeben wurde
        if (noParams == 0) {
            fprintf(stderr, "no command ?!\n");
            exit(1);
        }

        if (strcmp(command, "cd") == 0) {		// string compare , falls "cd" dann true
            chdir(parameters[1]);
            continue;
        }

        if (noParams > 0) {
            parameters[noParams] = NULL;
            isCommandInBackround = true;
        }

        if ((childPid = fork()) == -1) { // create child process
            fprintf(stderr, "can't fork!\n");
            exit(2);

        } else if (childPid == 0) { // child process
            execvp(command, parameters); // executes command
            exit(3);

        } else if ((childPid > 0) && !isCommandInBackround) {// father
            waitpid(childPid, &status, WUNTRACED | WCONTINUED);

        } else {//if ((childPid > 0) && isCommandInBackround) {
            fprintf(stdout, "[%d]\n",childPid);
            //signal(SIGCHLD, sigchld_hdl);
        }
    }
    exit(0);
}

