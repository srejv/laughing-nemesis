
Types:
	FIXNUM
	THE_EMPTY_LIST
	BOOLEAN
	SYMBOL
	FIXNUM 
	CHARACTER
	STRING
	PAIR
	PRIMITIVE_PROC
	COMPOUND_PROC
	INPUT_PORT
	OUTPUT_PORT
	EOF_OBJECT

FIXNUM: 1 2 -44 etc
BOOLEAM: #f #t
SYMBOL: anytext
THE EMPTY LIST: ()
CHARACTER: #\a #\tab #\newline #\space
STRING: "this\t is \na string"
PAIR: (0 . 1) (0 1) (0 1 2 3) (0 . (1 . ()))
PRIMTIVE_PROC: Primtives
COMPOUND_PROC:
INPUT_PORT:
OUTPUT_PORT:
EOF_OBJECT: EOF

Primitives:
	null?
	boolean?
	symbol?
	integer?
	char?
	string?
	pair?
	procedure?
	
	char->integer
	integer->char
	number->string
	string->number
	symbol->string
	string->symbol

	+
	-
	*
	quotient
	remainder
	=
	<
	>
	
	cons
	car
	cdr
	set-car!
	set-cdr!
	list
	
	eq?", is_eq_proc);

	apply", apply_proc);
	interacction-environment", interaction_environment_proc);
	null-environment", null_environment_proc);
	environment", environment_proc);
	eval", eval_proc);

