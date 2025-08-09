# Build Lisp interpreter
This is a project from original idea of https://buildyourownlisp.com 

I follow lessons on Youtube channel [esadecimale](https://www.youtube.com/@esadecimale).

**Language C**

**Libraries**
- linenoise - simple C library to read/write on terminal of antirez [linenoise](http://github.com/antirez/linenoise)
- mpc - a micro parsing library from [orangeduck](https://github.com/orangeduck/mpc)

**repl**
contains test of parsing expressions

**versions**
- tag 0.3 contains a simple parser for numeric expressions
- tag 0.4 contains evalution for parser of tag 0.3 It contains a modified grammar which has one number or binary expressions

