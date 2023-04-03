# InterpreterImplementation
Advanced calculator interpreter having various operations using C language.

INTERPRETER IN C

# CONTRIBUTORS
- [EnesBaserr](https://github.com/EnesBaserr) / ENES BAŞER
- [TheRealLowyer](https://github.com/TheRealLowyer) / YASİN ATLI


# SECTIONS :
1) INTRODUCTION
2) GENERAL PURPOSE
3) DESIGN AND IMPLEMENTATION DETAILS 
4) PROBLEMS ENCOUNTERED 
5) SAMPLE I/O

# INTRODUCTION:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This documentation describes the use of accompanying C code
that implements the methods described in main.c .Makefile is provided
,one who want to test can give a promp to console "make" and create executable
named avcalc.
make MakeFile
./advcalc
.
.
.


# GENERAL PURPOSE: 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;It is actually language implementation compatible with console.
Interpreter take input as infix expression from user and evaluate its
value and also handling some error cases. Infix expressions include
various operations :
A+B →Returns summation of A and B
A*B → Returns multiplication of A and B
A-B → Returns substraction of A and B
A&B → Returns bitwise A and B
A|B → Returns bitwise A or B
Xor(A,B) → Returns bitwise A xor B.
Ls(A,B) → Returns the result of A shifted B bits to the left.
Rs(A,B) → Returns the result of A shifted B bits to the right.
Lr(A,B) → Returns result of A rotated B times to the left.
Rr(A,B) → Returns the result of A rotated B times to the right.
Not(A)→Return bitwise complement of A.
Also <CTRL-D> is used for terminated interpreter screen. And also in
this language ‘%’ is used for command lines. They should not be
included in evaluation process.



# DESIGN AND IMPLEMENTATION DETAILS:
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Given implementation used fgets method and reads the user

input untill terminator command encountered.(Ctrl-D→ EOF in Unix

Env.) Firstly, code convert the infix(input)expression into postfix which

are stored in array. In algorithm, we use also stack which is used for

determining predecende and so correct order in postfix expression.

Operators are also have corresponding priority relative to their

precendence order and they are stored in stack.If reading pointer

match with ‘)’, the code starts popping from stack till stack top is ‘(‘.

Integer and variables are pushed to the postfix array directly. But in
case of methods(ls,rr,rs…) we assing them some special chars like # ->

rs and / -> lr etc. Then, we push them into the stack to determine the

correct order of final postfix.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;After settling postfix array, postfix array is sent to evalPost()

method which printing result. It is important to mention that in this

method, another stack (evalStack) which we insert numerical values

of postfix is used . In general this code iterates until matching with

operator or method signature. If so, it pops last two value or one(in

case of not function) and evaluate them.For example if evalStack is

empty and we trace in postfix like 12+ then we should pop 1 and 2

and push them into evalStack. It continious like this way and we print

evalStack top element which is our correct result.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;String or char variables are stored in Hashmap struct. Hash

(index) calculation is provided with implementation. So if key is

already exist, it will be overwrited.Otherwise, new value will be

stored in correct position of corresponding key.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Implementation also provides error checking.Firstly, basic

stack(balanceStack) used in form of array to throw an error statement

in case of unbalancing paranthesis.If current char equals ‘(‘ ,char will

be pushed to the stack. But, if char equals ‘)’ then peek of stack

should be pop. In the end if there is inbalance throw an error.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;We identified integers, variables,operators, paranthesis and

functions as distinc integer values:

Integers and variables → 1

Operators → 2

(Opened Paranthesis,Closed P.) ->3,4

Functions -> 5

The code starts with int error = 0 then if matches any of one

mentioned above, it is reassigned corresponding values. It facilitate

error handling for example. If error equals 1 meaning previous string

or char was integer and current char matches with opened p. (2 (…))

Which is not possible case so the code will throw an error.

# PROBLEMS ENCOUNTERED 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;When dealing with error handling,(For example 1++2 or 1 2 -3)

we tried to approach in many ways. But, they are not efficient as

much as we predicted.Thus, we developed a different approach which

is assigning values for different type of chars or strings as we

mentioned above.


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Also, it is important to mention that Token struct can be used in

implementation and it can have char value and Token type like

Token.Type = Operator,Method… In this implementation we stored all

information in array and prefer to store them as char values.



# SAMPLE I/O

&nbsp;&nbsp;&nbsp; > x=2

&nbsp;&nbsp;&nbsp;> x*2

&nbsp;&nbsp;4

&nbsp;&nbsp;&nbsp;> y = x - 4 * (x + x)
&nbsp;&nbsp;&nbsp;> y = -12

&nbsp;&nbsp;&nbsp;(((x) //Paranthesis Imbalance

&nbsp;&nbsp;Error!

&nbsp;&nbsp;&nbsp;> x+y+1*-1

&nbsp;&nbsp;Error! //Two operator next to each other which is not possible in this

language, they are defined unary operator.

&nbsp;&nbsp;&nbsp;<Ctrl-D>
