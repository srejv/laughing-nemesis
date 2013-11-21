Authors: Oscar Dragén & Johan Toft

Description:

A small shell called lsh, it has some basic features such as:

 * Background processes
 * Change working directory
 * Pipes
 * Redirect output stream
 * Stream input
 * The parser sucks so you can't change directory to a directory with a space in it
 * SHELL SCRIPTING USING SCHEME

How to start the interpreter:

write !
this will start the interpreter.
lsh is the "execute shell command primitive" which executes programs in the shell.

Example to start vim:
srejv@srejv-SVP1321L1EBI ~/Documents/skola/Operating Systems/Lab assignment 1/lab1 $ ./lsh
@/home/srejv/Documents/skola/Operating Systems/Lab assignment 1/lab1> !
> (lsh "vim")
ok
> 


You can also:
> (load "stdlib.scm") ; this file contains a few neat functions.
stdlib-loaded
> (lsh (pipe 'fortune 'cowsay))
 ________________________________________
/ The man who sets out to carry a cat by \
| its tail learns something that will    |
| always be useful and which never will  |
| grow dim or doubtful.                  |
|                                        |
\ -- Mark Twain                          /
 ----------------------------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
ok
> (lsh "fortune | cowsay") ; the above command is equivalent to this

If you don't have any extra parameters you can use ' to quote a symbol to lsh, since they are 
stored in the variable table in the same way (string) it works. If you want parameters, you need
to execute it as a string. It's also turing complete which means that you can implement scheme
in the interpreter by writing:

> (define env (environment))
ok
> (define (repl)
    (write-char #\])
    (write-char #\space)
    (write (eval (read) env))
    (write-char #\newline)
    (repl))
ok
> (repl)
] (cons 1 2)
(1 . 2)
] (define (double x) (* 2 x))
ok
] (double 21)
42

Cool, huh? :)