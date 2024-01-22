# Assembly-code-executor
This C code allows you to transform a program written in language assembler (in one file) into a program written in machine language (named <i>hexa.txt</i>). It will then execute it instruction after instruction in order to simulate a fictitious machine called a stack machine.

## WARNING
### Structure of supported files
The files supported by the program are limited to the following structure:
* one instruction per line
* each instruction is coded on five bytes
* each line is composed of an optional label, 0, 1 or more spaces or a
tabulation, of the operation code of the instruction (in lower case), followed, if there is data, by a space
and the data, finally at the end of a potential comment starting with the character **;**
* the value of a piece of data will be specified in decimal

Here are the different supported instructions with their machine code given in parentheses:
* (0) push x , (1) push# i , (2) ipush , (3) pop x , (4) ipop , (5) dup , (6) op i , (7) jmp adr , (8) jpz adr , (9) rnd x , (10) read x , (11) write x , (12) call adr , (13) ret , (14) halt.

The “op” machine instruction allows you to perform an arithmetic or logical operation on the data.
located at the top of the stack. Depending on the data, here are the possible operations :
* 0: adds the two values at the top,
1: subtraction,
2: multiplication,
3: integer division,
4: modulo,
5: reverses the value at the top of the stack,
6: equality test,
7: inequality test,
8: test >,
9: test ≥,
10: test <,
11: test ≤,
12: and bit-by-bit logic,
13: or bit-by-bit logic,
14: bitwise logical exclusive or,
15: bitwise non-logical

If you wish, you have short programs written in assembly language to test the simulator in the [“Test files”](https://github.com/MenSouf07/Assembly-code-executor/tree/main/Test_files) folder (remember to look at the ["README_TestFiles.txt"](https://github.com/MenSouf07/Assembly-code-executor/blob/main/Test_files/README_TestFiles.txt) file to find out what each test program does).


### Details
* If a text file already has the name “copy” in your directory, it will be deleted at runtime.
* The maximum label size is set at 300, to change it, you can change the defined N value
on line 29.
* In the event of a syntax error in the source file (incorrect line structure, unknown instruction, etc.),
the machine language file “hexa.txt” will not be created and the first erroneous line detected will be
indicated on the terminal.
* This program does not necessarily support logic errors that may occur in
the source file: it was designed to stop in the event of a system error (i.e. access out of memory, jmp out of
program…) It is therefore the user's responsibility to avoid writing an infinite program or
to write a halt instruction, for example.



## GETTING STARTED

### Prerequisites
* Have a C compiler.

### Installation & use
To install Assembly-code-executor, follow these steps:

#### 1. Clone the Repository
Open the terminal:
```sh
git clone https://github.com/MenSouf07/Assembly-code-executor.git 
cd Assembly-code-executor
```
#### 2. Compile the Program
To compile the program, execute the following command in the terminal:
```sh
gcc simulator.c -o simulator
```
To run a <i>pgm.txt</i> source file, write to the terminal ():
```sh
./simulator acces/path/pgm.txt        #Linux
simulator.exe C:\acces\path\pgm.txt   #Windows
```
Make sure to replace "access/path/pgm.txt" (respectively "C:\access\path\pgm.tx") with the actual path of your <i>pgm.txt</i> file.

The <i>hexa.txt</i> file will contain the translation of the <i>pgm.txt</i> file into machine code. the execution when it will be done in the terminal


## AUTHORS & CONTACTS
Don't hesitate to contact us for more information :
* Mendel Souffir alias [@MenSouf07](https://github.com/MenSouf07)
  * mail : mendel.souffir@dauphine.eu / mendel.souffir07@gmail.com
  * [Linkedin](https://www.linkedin.com/in/mendel-souffir-1971252b0/)
* Camille Jouanet alias [@kmillee](https://github.com/kmillee)
  * mail : camille.jouanet@dauphine.eu
  * [Linkedin](https://fr.linkedin.com/in/camillejouanet)
