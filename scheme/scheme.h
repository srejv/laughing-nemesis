#ifndef _SCHEME_H_
#define _SCHEME_H_

#include <stdio.h>

typedef enum {THE_EMPTY_LIST = 0, BOOLEAN, SYMBOL, FIXNUM,
              CHARACTER, STRING, PAIR, PRIMITIVE_PROC,
              COMPOUND_PROC, INPUT_PORT, OUTPUT_PORT,
              EOF_OBJECT} object_type;

typedef struct object {
    object_type type;
    union {
        struct {
            char value;
        } boolean;
        struct {
            char *value;
        } symbol;
        struct {
            long value;
        } fixnum;
        struct {
            char value;
        } character;
        struct {
            char *value;
        } string;
        struct {
            struct object *car;
            struct object *cdr;
        } pair;
        struct {
            struct object *(*fn)(struct object *arguments);
        } primitive_proc;
        struct {
            struct object *parameters;
            struct object *body;
            struct object *env;
        } compound_proc;
        struct {
            FILE *stream;
        } input_port;
        struct {
            FILE *stream;
        } output_port;
    } data;
} object;

void init_scm(void);
object *eval_scm(object *exp, object *env);
object *read_scm(FILE *in);
void write_scm(FILE *out, object *obj);
object *get_the_global_env(void);
int get_scheme_error();
void reset_scheme_error();


int peek(FILE *in);
char is_input_port(object *obj);
char is_compound_proc(object *obj);
object *setup_environment(void);
object *make_environment(void);
object *make_input_port(FILE *in);
char is_eof_object(object *obj);
object *make_output_port(FILE *in);
char is_output_port(object *obj);

#endif

