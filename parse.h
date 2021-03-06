
#ifndef _PARSE_H_
#define _PARSE_H_

typedef struct c {
  char **pgmlist;
  struct c *next;
} Pgm;

typedef struct node { 
  Pgm  *pgm;
  char *rstdin;
  char *rstdout;
  char *rstderr;
  int bakground;
} Command;

extern void initp( void );
extern int parse ( char *, Command *);
extern int nexttoken( char *, char **);
extern int acmd( char *, Pgm **);
extern int isidentifier( char * );

#endif