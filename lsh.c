/* 
 * Main source code file for lsh shell program
 *
 * You are free to add functions to this file.
 * If you want to add functions in a separate file 
 * you will need to modify Makefile to compile
 * your additional functions.
 *
 * Add appropriate comments in your code to make it
 * easier for us while grading your assignment.
 *
 * Submit the entire lab1 folder as a tar archive (.tgz).
 * Command to create submission archive: 
      $> tar cvf lab1.tgz lab1/
 *
 * All the best 
 */


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
void sigtest(int);
void Startexec(Command *);
/* When non-zero, this global means the user is done using this program. */
int done = 0;

/*
 * Name: main
 *
 * Description: Gets the ball rolling...
 *
 */
int main(void) {
  //signal(SIGINT, sigtest);
  signal(SIGTSTP, sigtest);
  static const int READ = 0;
  static const int WRITE = 1;

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
        pid_t child_pid;
        int child_status;
        child_pid = fork();
        if(child_pid == 0){
      /*
       * Remove leading and trailing whitespace from the line
       * Then, if there is anything left, add it to the history list
       * and execute it.
       */
        stripwhite(line);
      
            if(*line) {
              add_history(line);
              /* execute it */
              n = parse(line, &cmd);
              PrintCommand(n, &cmd);
              Startexec(&cmd);
            }
            /*
            int fd[2];
            pipe(fd);
            Pgm *pgm = cmd.pgm;
            int child_pid1 = fork();

            if(child_pid1==0){
                printf("teeeeeeeeeeeeeeeeeest pl[0]: ");
                char **pl = pgm->pgmlist;
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                execvp(pl[0],pl);
                printf("lsh: %s: command not found\n",pl[0]);
                exit(1);
            }else{
                Pgm *pgm1 = pgm->next;
                char **pl1 = pgm1->pgmlist;
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                execvp(pl1[0],pl1);
                printf("lsh: %s: command not found\n",pl1[0]);
                exit(1);
            }

            Pgm *pgm1 = pgm->next;
            char **pl = pgm->pgmlist;
            //char **pl =cmd.pgm->pgmlist;
            printf("teeeeeeeeeeeeeeeeeest pl[0]: ");
            execvp(pl[0],pl);
            printf("lsh: %s: command not found\n",pl[0]);
            */
            exit(1);
        }else{
          pid_t tpid;
          if(cmd.bakground){
            do{
              tpid = wait(&child_status);
            }while (tpid != child_pid);
          }
        
        }
    }
    
    if(line) {
      free(line);
    }
  }
  return 0;
}
/*
 * Name: Startexec 
 *
 * Description: Start function for recursive calls, handles special cases
 *
 */
void Startexec(Command *cmd){
    char *rstdout = cmd->rstdout;
    if (rstdout == NULL){
        printf("RSTDOUT: False");
    }else{
        printf("RSTDOUT: True");
        int fdin = open(rstdout,O_CREAT | O_WRONLY, S_IRWXU | S_IRGRP | S_IROTH);
        dup2(fdin, STDOUT_FILENO);
    }
    Pgm *pgm = cmd->pgm;
    Pgm *pgmNext = pgm->next;
    if( pgmNext==NULL){
        printf("pgmNext: NULL");
    }
    char **pl = pgm ->pgmlist;
    execvp(pl[0],pl);
    exit(1);


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
* Name: sigtest
*
* Signal handler
*/
void sigtest(int sig)
{
  if(sig == SIGINT)
  {
    ;
  }
  else if(sig == SIGTSTP)
  {
    ;
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
