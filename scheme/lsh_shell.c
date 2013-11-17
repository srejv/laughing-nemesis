
#include "lsh_shell.h"
#include "parse.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

/* When non-zero, this global means the user is done using this program. */
char *workingDirectory;
int waiting = 0;

void runProgram(const Pgm *program) {
  int result = execvp(program->pgmlist[0], program->pgmlist);
  if(result < 0) {
    perror(NULL);
    exit(-1);
  }
}

int hasAnotherCommand(const Pgm *program) {
  return program->next != NULL;
}


/**  setupStdinAndOut redirects stdin to read and stdout to write.
*    Read and write are file descriptors.
*/
void setupStdInAndOut(int read, int write) {
  dup2(read, STDIN_FILENO);
  dup2(write, STDOUT_FILENO);
}

void fixPipe(const Pgm *program, int fdRead, int fdWrite, int fd[2]) {
  int result = pipe(fd);
  if(result < 0) {
    printf("PIPE ERROR\n");
    return;
  }

  pid_t pid = fork();
  if(pid == 0) {  
    setupCommandsAndExecute(program->next, fdRead, fd[PIPE_WRITE]);
  } else {
    dup2(fdWrite, fd[PIPE_WRITE]);
    waitpid(pid, NULL, 0);
  }
}

void setupCommandsAndExecute(Pgm *program, int fdRead, int fdWrite ) {
  int fd[2] = {fdRead,fdWrite};
  if(hasAnotherCommand(program)) {
    fixPipe(program, fdRead, fdWrite, fd);
  } else {
    dup2(fdRead, fd[PIPE_READ]);
    dup2(fdWrite, fd[PIPE_WRITE]);
  }
  setupStdInAndOut(fd[PIPE_READ], fd[PIPE_WRITE]);
  runProgram(program);
}

void tryToExecuteExit(const char *command) {
  if(strcmp("exit",command) == 0) {
    exit(0);
  }
}

int tryToExecuteChangeDirectory(const Pgm *program) {
  if(strcmp("cd", program->pgmlist[0]) == 0) {
    chdir(program->pgmlist[1]);
    return 1;
  }
  return 0;
}

char *argv[50];
int tryPlz(const Pgm *program) {
  if(strcmp("plz", program->pgmlist[0]) == 0) {
    if(program->pgmlist[3] != NULL) {
      pid_t pid = fork();
      if(pid ==  0) {
        printf("Function: %s, Arg: %s\n", program->pgmlist[1], program->pgmlist[3]);
        argv[0] = program->pgmlist[1];
        argv[1] = program->pgmlist[3];
        printf("Function2: %s, Args: %s, %s\n", program->pgmlist[1], program->pgmlist[1], program->pgmlist[3]);
        int r = execvp(program->pgmlist[1], argv);
        if (r < 0) {
          perror(NULL);
          exit(0);
        }
      } else {
        waitpid(pid, NULL, 0);
      }

      return 1;
    } else {
      return 0;
    }
  }
  return 0;
}

int trySetEnv(const Pgm *program) {
  return 0;
}

int tryUnsetEnv(const Pgm *program) {
  return 0;
}

int executeShellCommand(const Pgm *program) {
  tryToExecuteExit(program->pgmlist[0]);
  if(tryToExecuteChangeDirectory(program)) return 1;
  if(tryPlz(program)) return 1;
  if(trySetEnv(program)) return 1;
  if(tryUnsetEnv(program)) return 1;
  return 0;
}

void printPreInput(const char *prompt) {
  printf("%s@", getlogin());
  workingDirectory = getcwd(workingDirectory, 255);
  printf("%s%s%s%s", KCYN, workingDirectory, KYEL, prompt);
}

void signalhandling(int sig) { 
  if(!waiting) {
    char *buf = "\n";  
    write(STDIN_FILENO, buf, 1);
    printPreInput("> ");
  }
}

void handleZombies(int sig) {
  if(sig == SIGCHLD) {
    waitpid(-1, NULL, WNOHANG);
  }
}

void executeCommand(const Command *cmd) {
  pid_t pid = fork();

  if(pid == 0) {
    Pgm *currentProgram = cmd->pgm;
    
    if(cmd->bakground) {
      signal(SIGINT, SIG_IGN);
    }

    int file_input = STDIN_FILENO, file_output = STDOUT_FILENO;
    FILE *fin = NULL, *fout = NULL;

    if(cmd->rstdin) {
      fin = fopen(cmd->rstdin, "r");
      file_input = fileno(fin);
    }

    if(cmd->rstdout) {
      fout = fopen(cmd->rstdout, "w");
      file_output = fileno(fout);
    }

    setupCommandsAndExecute(currentProgram, file_input, file_output); 

    if(fin) fclose(fin);
    if(fout) fclose(fout);
  }
  else {
    if(!cmd->bakground) {
      waiting = 1;
      waitpid(pid, NULL, 0);
      waiting = 0;
    }
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