/* 
 * Main source code file for lsh shell program
 * Author: Henning Phan and Gustav Ehrenborg
 * Group : B 22
 * Submit the entire lab1 folder as a tar archive (.tgz).
 * Command to create submission archive: 
      $> tar cvf lab1.tgz lab1/
 *
 */


#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parse.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * Function declarations
 */

void PrintCommand(int, Command *);
void PrintPgm(Pgm *);
void stripwhite(char *);
void Startexec(Command *);
void finishexec(Pgm *);

/* When non-zero, this global means the user is done using this program. */
int done = 0;
const int READ = 0;
const int WRITE = 1;

/*
 * Name: main
 *
 * Description: Gets the ball rolling...
 *
 */
int main(void) {
  signal(SIGINT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);

  Command cmd;
  int n;
  while (!done) {

    char *line;
    line = readline("> ");

    if (!line) {
      /* Encountered EOF at top level *///
      done = 1;
    }
    else {
        stripwhite(line);
        if(*line) {
            add_history(line);
            /* execute it */
            n = parse(line, &cmd);
            //PrintCommand(n, &cmd);
            if (! Builtexec(&cmd)) {

            /* If not a builtin cmd, fork */
                pid_t child_pid;
                int child_status;
                child_pid = fork();

                if(child_pid == 0){
                    Startexec(&cmd); // Child code, recursive helper function
                    exit(0);
                }else{
                  pid_t tpid;       // Parent code, to wait or not to wait for child
                  if(!cmd.bakground){ 
                      tpid = waitpid(child_pid, &child_status, 0);
                  }
                
                }
            }
        }
        /* Check for builtin command and execute it */
    }
    if(line) {
      free(line);
    }
  }
  return 0;
}
/*
 * Name. Builtexec
 *
 * Description: Checks if it is a built in function
 * Return: 1 if it's a builtin function, else 0
 */
int Builtexec(Command *cmd){
    Pgm *pgm = cmd->pgm;
    char **pl = pgm ->pgmlist;
    if( strcmp("exit", pl[0]) == 0) {
        exit(0);
    } else if ( strcmp("cd", pl[0]) == 0) {
        chdir(pl[1]);
    return 1;
    }
    return 0;
}

/*
 * Name: Startexec 
 *
 * Description: Start function for recursive calls
 * Sets up reading from file and writing to file and
 * Calls the function Finishexec to handle the piping
 *
 */
void Startexec(Command *cmd){
    char *rstdout = cmd->rstdout;   
    if (rstdout != NULL){           // If we should output to file
        printf("RSTDOUT: %s", rstdout);
        int out = creat(rstdout, S_IRWXU | S_IRGRP | S_IROTH); // Get files' STDIN
        dup2(out, STDOUT_FILENO);   // Connect our STDOUT to files STDIN
    }

    char *rstdin = cmd->rstdin;
    if (rstdin != NULL){            // If we should read from file
        printf("RSTDIN: %s", rstdin);
        int in = open( rstdin, O_RDONLY);   // Get files STDOUT
        dup2(in, STDIN_FILENO);             // Connect files' STDOUT to our STDIN
    }
    finishexec( cmd->pgm);          // Executes other programs recursively and setup the pipes

}

/*
 * Name: finishexec
 * 
 * Description: Recursive call that handles pipes
 *
 */
void finishexec(Pgm *pgm){
    signal(SIGINT, SIG_IGN);            // Ignore ^C
    Pgm *pgmNext = pgm->next;
    if( pgmNext == NULL ){              // If the next pgm is null
        char **pl = pgm ->pgmlist;      // then this is our last execution
        execvp(pl[0],pl);
        perror("ERROR COMMAND NOT FOUND");
        exit(1);
        
    }
    int fd[2];                          // Ready the pipe
    pipe(fd);

    int child_pid= fork();              // Fork
    if ( child_pid == -1 ){
        perror("fork");
    }

    if ( child_pid == 0){               // Child code
        dup2(fd[WRITE], STDOUT_FILENO); // Connect childs output to pipe
        close(fd[READ]);
        finishexec(pgmNext);            // see what pgm this child should execute
    }else{                              // Parent code
        dup2(fd[READ], STDIN_FILENO);   // Childs output is now parents input
        close(fd[WRITE]);
        char **pl = pgm ->pgmlist;
        execvp(pl[0],pl);               // Execute program
        perror("ERROR COMMAND NOT FOUND");

    }
}            
             
/*
 * Name: PrintCommand
 *
 * Description: Prints a Command structure as returned by parse on stdout.
 *
 */
void PrintCommand (int n, Command *cmd)
{
  printf("Parse returned %d:\n", n);
  printf("   stdin : %s\n", cmd->rstdin  ? cmd->rstdin  : "<none>" );
  printf("   stdout: %s\n", cmd->rstdout ? cmd->rstdout : "<none>" );
  printf("   bg    : %s\n", cmd->bakground ? "yes" : "no");
  PrintPgm(cmd->pgm);
}

/*
 * Name: PrintPgm
 *
 * Description: Prints a list of Pgm:s
 *
 */
void PrintPgm (Pgm *p)
{
  if (p == NULL) {
    return;
  }
  else {
    char **pl = p->pgmlist;

    /* The list is in reversed order so print
     * it reversed to get right
     */
    PrintPgm(p->next);
    printf("    [");
    while (*pl) {
      printf("%s ", *pl++);
    }
    printf("]\n");
  }
}

/*
 * Name: stripwhite
 *
 * Description: Strip whitespace from the start and end of STRING.
 */
void
stripwhite (char *string)
{
  register int i = 0;

  while (whitespace( string[i] )) {
    i++;
  }
  
  if (i) {
    strcpy (string, string + i);
  }

  i = strlen( string ) - 1;
  while (i> 0 && whitespace (string[i])) {
    i--;
  }

  string [++i] = '\0';
}
