
#ifndef _LSDH_SHELL_
#define _LSDH_SHELL_

#include "parse.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/*
 * Function declarations
 */
void PrintCommand(int, Command *);
void PrintPgm(Pgm *);
void stripwhite(char *);
void setupCommandsAndExecute(Pgm *program, int fdRead, int fdWrite);

void runProgram(const Pgm *program);

int hasAnotherCommand(const Pgm *program);

/**  setupStdinAndOut redirects stdin to read and stdout to write.
*    Read and write are file descriptors.
*/
void setupStdInAndOut(int read, int write);
void fixPipe(const Pgm *program, int fdRead, int fdWrite, int fd[2]);
int trySetEnv(const Pgm *program);
int tryUnsetEnv(const Pgm *program);
int executeShellCommand(const Pgm *program);
void printPreInput(const char *prompt);
void signalhandling(int sig);
void handleZombies(int sig);
void executeCommand(const Command *cmd);

#endif
