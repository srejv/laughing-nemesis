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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "parse.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

/*
 * Function declarations
 */

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

char *error;

void PrintCommand(int, Command *);
void PrintPgm(Pgm *);
void stripwhite(char *);
void runProgram(Pgm *program, int fdRead, int fdWrite ) {
  int fd[2] = {fdRead,fdWrite};
  if(program->next) {
    int result = pipe(fd);
    if(result < 0) {
      printf("PIPE ERROR\n");
      return;
    }

    pid_t pid = fork();
    if(pid == 0) {  
      runProgram(program->next, fdRead, fd[PIPE_WRITE]);
    } else {
      dup2(fdWrite, fd[PIPE_WRITE]);
      waitpid(pid, NULL, 0);
    }
  } else {
    dup2(fdRead, fd[PIPE_READ]);
    dup2(fdWrite, fd[PIPE_WRITE]);
  }
  dup2(fd[PIPE_READ], STDIN_FILENO);
  dup2(fd[PIPE_WRITE], STDOUT_FILENO);
  int result = execvp(program->pgmlist[0], program->pgmlist);
  if(result < 0) {
    perror(NULL);
    exit(-1);
  }
}


/* When non-zero, this global means the user is done using this program. */
int done = 0;

char *workingDirectory;

int executeShellCommand(const Pgm *program) {
  if(strcmp("exit",program->pgmlist[0]) == 0) {
    done = 1;
    return 1;
  }
  if(strcmp("cd", program->pgmlist[0]) == 0) {
    chdir(program->pgmlist[1]);
    return 1;
  }
  return 0;
}

void signalhandling(int sig) {
  // Do... NOTHING!!
}

/*
 * Name: main
 *
 * Description: Gets the ball rolling...
 *
 */
int main(void)
{
  Command cmd;
  int n;
  int fd[2];
  signal(SIGINT, signalhandling);

  while (!done) {

    char *line;

    workingDirectory = getcwd(workingDirectory, 255);
    printf("%s%s%s", KGRN, workingDirectory, KMAG);
    line = readline("> ");
    
    if (!line) {
      /* Encountered EOF at top level */
      done = 1;
    }
    else {
      /*
       * Remove leading and trailing whitespace from the line
       * Then, if there is anything left, add it to the history list
       * and execute it.
       */
      stripwhite(line);
      if(strlen(line) == 0) {
      }
      else if(*line) {
        add_history(line);
        /* execute it */
        n = parse(line, &cmd);
        //PrintCommand(n, &cmd);

        if(executeShellCommand(cmd.pgm) > 0) {
          // Must free line because it does this at the end of each iteration
          free(line);
          continue;
        }

        pid_t pid = fork();

        if(pid == 0) {
          Pgm *currentProgram = cmd.pgm;
          int file_input = STDIN_FILENO, file_output = STDOUT_FILENO;
          FILE *fin = NULL, *fout = NULL;

          if(cmd.rstdin) {
            fin = fopen(cmd.rstdin, "r");
            file_input = fileno(fin);
          }

          if(cmd.rstdout) {
            fout = fopen(cmd.rstdout, "w");
            file_output = fileno(fout);
          }

          runProgram(currentProgram, file_input, file_output); 

          if(fin) fclose(fin);
          if(fout) fclose(fout);
        }
        else {
          if(!cmd.bakground) {
            waitpid(pid, NULL, 0);
          }
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
 * Name: PrintCommand
 *
 * Description: Prints a Command structure as returned by parse on stdout.
 *
 */
void
PrintCommand (int n, Command *cmd)
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
void
PrintPgm (Pgm *p)
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
