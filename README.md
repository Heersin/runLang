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

### Lisp 
S-expr now

## Develop Guid

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


