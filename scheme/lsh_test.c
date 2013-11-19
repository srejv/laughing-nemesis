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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "parse.h"
#include "lsh_shell.h"
#include "scheme.h"


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
  signal(SIGINT, signalhandling);
  signal(SIGCHLD, handleZombies);
  FILE *f;
  object *exp;

  init_scm();

  while (1) {

    char *line;

    printPreInput("");
    line = readline("> ");
    
    if (!line) {
      /* Encountered EOF at top level */
      if(line) {
        free(line);
      }
      break;
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

        if(line[0] == '!') {
          // LISP :D let's do this!
          while(1) {
            printf("> ");
            exp = read_scm(stdin);
            if (exp == NULL) {
              break;
            }
            write_scm(stdout, eval_scm(exp, get_the_global_env()));
            printf("\n");
            if(get_scheme_error() < 0) {
              break; 
            }
          }
          reset_scheme_error();
          free(line);
          continue;
        }

        /* execute it */
        n = parse(line, &cmd);
        if(n < 0) {
          printf("Parse error\n");
          free(line);
          continue;
        }

        if(executeShellCommand(cmd.pgm) == 0) {
          // Didn't execute any shell command, execute program
          executeCommand(&cmd);
        } 
      }
    }
    
    if(line) {
      free(line);
    }
  }
  return 0;
}

