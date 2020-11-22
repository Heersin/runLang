# runLang
This is a repo to figure out some concepts in interpreter and compiler for later project.

## Build
```sh
make
make clean
```

## Usage
```sh
./build/bin/lisping
[console] >
```
### Basic Concept
this is a general interpreter, support only part of lisp now.
command, lisp and other languages are treated as Lang.

you can switch between these languages by using:
```
#[lang]
```
for example:
```
[console]> #lisp
[*]Change Run Lang to -> lisp
```
will lead you into the lisp environment

### Command Mode
when you open this interpreter, you will enter the "cmd" language by default;
the syntax of it is:

> cmd_name args1 args2 args3 ...

note that the charset of arg is [0-9a-zA-Z_]

support command:
* help -- print help message
* info -- echo arg1
* version -- print version info

### Lisp Mode
S-expr now

## Develop Guide

### Project Structure
```
runLang
├── build
│   ├── bin --- build binary
│   └── obj
├── doc
├── lib     --- third library
│   └── src
├── src     --- src here
│   └── include --- header file
└── test    --- test codes
    ├── example
    ├── obj
    └── src
|
├── LICENSE
├── Makefile
├── README.md

```

### Add a new Language

Follow these steps to create more langs. In short, you need:

* create two files (at least)
  1. my_lang.c
  2. my_lang.h

* modify 3 files
  1. lang.h
  2. lang.c
  3. engine.c

you can check the "cmd.h" and "cmd.c" to learn the command language if feel confused.
#### my_lang.h
your own lang should expose these interfaces to outside world
(substitute lang with the name of your lang)

1. LangParser init_lang(void); -- init lang ast parser
2. void clean_lang(LangParser); -- clean tree

LangParser is defined in src/include/lang.h, store your lang parser
in parser->lang, and others in rule[x];

3. lang_val lang_read(LangParser parser) -- parse ast and store info in your own struct;
4. lang_val lang_eval(lang_val val) -- exec lang
5. void lang_println(lang_val val) -- print result/error ...
6. void lang_del(lang_val val) -- delete lang_val

`lang_val` is a pointer points to develop-defined structure.
you can use it to recv result, print, eval ...

#### my_lang.c
1. init
use mpc parser to create the ruler of lang

2. lang_val
It's a pointer (typedef pointer)

#### lang.h
define a new lang MACRO.

#### lang.c
in map_lang: convert your name of lang into MACRO

#### engine.c
in init_context:
  add the exposed function into specified array.
