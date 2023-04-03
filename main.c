    #include <stdio.h>
    #include <ctype.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #define TABLE_SIZE 128//Hashtable Size.
    #define BITS 64

    char balanceStack[256];//Used for checking paranthesis balance.(i.e. stack)
    int flag = 1;//Maintaining the taking prompt from user. Check line 237.
    bool isEval = true;//Boolean for whether result should be calculated or not.

    bool flag2 = true;//Boolean for error checking.
    int error = 0;//Getting different value for each type of token.İndicating error s.t. 2+-5 or a b etc.
    int balanceTop=-1;//İndicating the top of balanceStack.
    int counter = 0;//Checking for left hand side of assignment.
    bool breaker=false;//For breaking the loops when errors occured.
    typedef struct {//Struct for stored entries in Hashmap.
        char* key;
        long long value;
    } Entry;
    typedef struct{//Hashmap struct
        Entry* entries[TABLE_SIZE];
    } HashTable;
    long long  hash(char* key) {//To determine the index of values to be stores in Hashmap.
        long long hash = 0;
        for (int i = 0; i < strlen(key); i++) {
            hash += key[i];
            hash = hash % TABLE_SIZE;
        }
        return hash;
    }
    HashTable* create_table() {//İnsantiate the Hashmap.
        HashTable* table = malloc(sizeof(HashTable));
        memset(table->entries, 0, sizeof(Entry*) * TABLE_SIZE);
        return table;
    }
    void set(HashTable* table, char* key, long long value) {//Setting value into Hashmap.
        long long hash_value = hash(key);
        if (table->entries[hash_value] == NULL) {
            table->entries[hash_value] = malloc(sizeof(Entry));
            table->entries[hash_value]->key = strdup(key);
        }
        table->entries[hash_value]->value = value;//If exists then overwrite.
    }
    long long get(HashTable* table, char* key) {//Getting values from Hashmap.
        long long  hash_value = hash(key);
        if (table->entries[hash_value] == NULL) {
            return 0;//Undefined Variables =0
        }
        return table->entries[hash_value]->value;
    }
    HashTable* table ;
    char stack[256];//Used for infix to postfix.Also beneficial for indicating the operator precedence.
    int top =-1 ;//stack top index.
    int pTop =0;//Postfix outer top index of postFix.
    char postfix[256][256];//2-D array that is in the form of postfix of given prompt.
    int innertop=0;// inner top index of postFix.
    void pushPostfix(char x){//Push the char to the postfix array.
        postfix[pTop][innertop]=x;
    }
    int isDifferent(char x){// to indicate the string in prompt.
        if(!isdigit(x) && x!='+'&& x!='-'&& x!='*'&& x!='&'&& x!='|'&& x!='=' && x!='('&& x!=')'){
            return 1;
        }
        return 0;
    }
    void printPostfix(){//Print postfix for easy debugging.
         for (int i = 0; i < 256; i++) {
                if (postfix[i][0] == '\0') {
                    // reached end of array
                    break;
                }
                for (int j = 0; j < 256; j++) {
                    if (postfix[i][j] == '\0') {
                        // reached end of string
                        break;
                    }
                    printf("%c", postfix[i][j]);
                }
                printf("\n"); // print newline after each string
            }
    }
    void push (char x){//push to the stack.
        stack[++top]=x;
    }
    char pop(){//pop from the stack.
        if(top == -1){
            return -1;//cannot pop anymore.
        }
        else
            return stack[top--];
    }
    void balancePush(char x){//push into balanceStack(which we use for paranthesis balance check.)
        balanceStack[++balanceTop]=x;
    }
    char balancePop(){//pop from balanceStack.
        if(balanceTop==-1){
            return -1;
        }
        return balanceStack[balanceTop--];
    }
    int priority(char x){//getting priority value for pushing balance.Crucial for determine the precendese so postfix form.
        if(x=='~'){//not
            return 41;
        }
        if(x=='^'){//ls
            return 41;
        }
        if(x=='$'){//xor
            return 41;
        }
        if(x=='#'){//rs
            return 41;
        }
        if(x=='/'){//lr
            return 41;
        }
        if(x=='{'){//rr
            return 41;
        }
        if(x=='('){
            return -2;
        }
        if(x=='='){
            return -1;
        }
        if(x=='+'|| x=='-'){
            return 2;
        }
        if(x=='*'){
            return 3;
        }
        if(x=='&'){
            return 1;
        }
        if(x=='|'){
            return 0;
        }//Methods are all have most priority.
        return INT_MIN;
    }
    long long evalPost() {//Evaluate the postfix array and getting result.If error would occur, method should not be used..
        long long  evalStack[256];//Stack for evaluating postfix.so if we have 12+ in postfix, pushed as 3 to evalStack.
        int evalTop = -1;//Top index for evalStack.
        for (int i = 0; i < pTop; i++) {
            char* str = postfix[i];
            if (isdigit(*str)) {
                // convert the string to an integer
                long long num = atoll(str);//strtol(str, &endptr, 10);//atoll is used because of that it can handle long long values.
                evalStack[++evalTop] = num;//pushing the returning value into the evalStack.

            } else if(*str =='='){
                set(table,postfix[0],evalStack[evalTop]);//Assume xyz = ... so postfix[0]= "xyz". Setting the value to the corresponding hash index.
                return 1;
            }
            else if(*str == '~'){//Used for not operator. We should deal with only previos value because "not" method takes only one parameter.
                long long a = evalStack[evalTop--];//pop and get the value.
                evalStack[++evalTop]= ~a;//push the not value of a.
            }
            else if (*str =='+' ||*str =='-' ||*str =='*' ||*str =='|' ||*str =='&' || *str == '^'
            || *str == '$' || *str == '#'|| *str == '/'|| *str == '{'){
                // operator, pop two operands and apply operator. Because all these operator means when they relate with 2 parameter.
                long long b = evalStack[evalTop--];
                long long  a = evalStack[evalTop--];
                switch (*str) { // Below cases code assigns corresponding chars for methods when taking prompt.Clarified in later lines.
                    case '^'://ls
                        evalStack[++evalTop] = a << b;
                        break;
                    case '$'://xor
                        evalStack[++evalTop] = a ^ b;
                        break;
                    case '#'://rs
                        evalStack[++evalTop] = a >> b;
                        break;
                    case '/'://lr
                        evalStack[++evalTop] = (a << b ) |(a >> (BITS-b));
                        break;
                    case '{'://rr
                        evalStack[++evalTop] = (a >> b ) |(a << (BITS-b));
                        break;
                    case '+':
                        evalStack[++evalTop] = a + b;
                        break;
                    case '-':
                        evalStack[++evalTop] = a - b;
                        break;
                    case '*':
                        evalStack[++evalTop] = a * b;
                        break;
                    case '&':
                        evalStack[++evalTop] = a & b;
                        break;
                    case '|':
                        evalStack[++evalTop] = a | b;
                        break;
                }
            } else{//case of *str == variable.
                evalStack[++evalTop] = get(table,str);//get the corresponding value of variable from hashtable.
            }
        }
        printf("%lld\n",evalStack[evalTop]);//evalStack[evalTop] is our final result. Print in long long format for 64 bit integers.
        return evalStack[evalTop];
    }
    int main() {
        table = create_table();//İnitialize the Hashtable struct.
        while(flag==1) {//flag is used for maintaining the main loop as mentioned at line 10.
            counter=0;//counter should be equals 0 after each iteration.
            flag2=true;//It is used when error occured.Preventing postfix from evaluated.Clarified in later lines.
            error = 0;//Error (check for error which has different value for each type of token.Clarified in later lines.)
            breaker=true;//same logic as above ones.
            isEval=true;//same logic as above ones.
            balanceTop=-1;//same logic as above ones.
            printf("> ");//Visualize for interpreter screen.





            char exp[256];//Storing given input.
            char *e, x;//pointer *e is used mostly to getting char values and x will be used later process.
            top=-1;//Reset all values to the default.
            pTop=0;//same logic
            innertop=0;//same logic.

            if (fgets(exp, sizeof(exp), stdin) == NULL) {//Ctrl D case handling.

                if (feof(stdin)) {
                    //printf("Received EOF, exiting...\n");//Useful for debug.
                    return -1;
                } else {
                    //Error when taking input.
                    return -1;
                }
            }



            int i;
            for (i = 0; exp[i] != '\0'; i++) {
                if (!isspace(exp[i])) {

                    isEval=true;
                    break;
                }
                isEval=false;
            }
            e = exp;//first char of exp assigned to pointer e.
            if(*e=='%'){//Representing comment.
                isEval=false;//Do not evaluate the resulting postfix.
            }



            while (*e != '\0' &&*e!='%' ) {//ignore both chars.
                if (isdigit(*e)) {//if *e(char) is digit then cont...
                    counter++;//increment counter by 1. If input contain = sign then we will check for error.(if counter >1 thrown an error.)
                pushPostfix(*e);//push the char *e in to the postfix.
                    e++;//look for next ones.
                    while (isdigit(*e)) {//if continued chars are digits then we shoulld push them second dimension of index.
                        innertop++;//to push into 2 nd dimension of postfix array.
                        pushPostfix(*e);
                        e++;
                    }
                    if(error==0 || error==2 || error==3){//Error 0 --> start of input, Error 2 --> operators. ,Error 3 --> '('
                        error=1;//Error value is 1. Error 1 --> digit or string variables.
                        //If conditions above satisfied then change the error.
                    }
                    else{//throw an error. For example error was 1 so previous token was 12 (assume) and we encounter another integer so error.
                        printf("%s","Error!\n");//Print error statement
                        pTop++;
                        innertop = 0;
                        isEval=false;//Do not evaluate
                        flag2=false;//Preventing from evaluation.
                        break;

                    }
                    pTop++;//increment index of postfix by 1.
                    innertop = 0;//setting innertop to 0.
                    continue;
                } else if (*e == '(') {
                    counter++;
                    if(error==0 || error == 3 || error==2||error==5){//Error 5 is --> ls,xr,xor,rr,not all methods
                        error = 3;//Update error to the 3.
                    } else{//throw an error otherwise.
                        printf("%s","Error!\n");
                        isEval=false;
                        flag2=false;
                        break;
                    }
                    balancePush('(');//push into the balanceStack for checking paranthesis balance.
                    push(*e);//push '(' to the stack for determining precedence.
                } else if (*e == ')') {
                    counter++;
                    if(error==1 || error==4) {//Error 4 -->')'
                        error=4;
                        if(balancePop()==-1){//If balanceTop -1 we can not pop so there is inbalance.
                            printf("%s","Error!\n");
                            isEval=false;
                            flag2=false;
                            break;
                        }
                     while ((x = pop()) != '(') {//pop from stack until reaching '(' it is algorithm for precedence order.
                            // printf("%c",x);
                            pushPostfix(x);//if x is not ( then push to postfix array.
                            pTop++;//increment pTop by 1.
                        }
                        innertop = 0;//set innerTop to zero.
                    } else{//Thorn an error if error condtions not satisfied.
                        printf("%s","Error!\n");
                        isEval=false;
                        flag2=false;
                        break;
                    }
                } else if (isspace(*e) != 0 ) {//ignore the spaces .
                    e++;

                    continue;
                } else if (*e == '*' || *e == '+' || *e == '-' || *e == '&' || *e == '|' || *e == '=') {//Operators.
                    if (error == 1|| error == 4 ) {// Error 1 --> integer or variables Error 4 --> )
                        error=2;
                        if(*e=='='&&counter>1){//condition of a+2 =... invalid LHS should contain only variable no other operator or tokens.
                            printf("%s","Error!\n");//Throw an error.
                            isEval=false;
                            flag2=false;
                            break;
                        }
                        while (priority(stack[top]) >= priority(*e)) {//If priority of stack[top] greater than current value then ...
                            pushPostfix(pop());//pop from stack and push it into the postfix.
                            pTop++;//increment pTop and look again for while condition.
                        }
                        push(*e);//If not then push it into the stack.
                    } else{//If error conditions not satisfied throw an error.
                        printf("%s","Error!\n");
                        isEval=false;
                        flag2=false;
                        break;
                    }
                }
                else if(*e ==',') {
                    if (error == 1 || error == 4) {//Error 1 --> integer and variables. Error --> 4
                        error = 2;
                        while (priority(stack[top]) <= 41 && top != -1 && priority(stack[top]) > -2) {//If we encounter with comma then pop all operators and methods untill you reach '('
                            pushPostfix(pop());//Pop from stack and push it to the postfix.
                            pTop++;//Increment by 1 beacuse popped value was pushed into postfix array.
                        }
                        e++;
                        continue;
                    }
                    else{//If not throw an error.
                        printf("%s","Error!\n");
                        isEval=false;
                        flag2=false;
                        break;
                    }
                }
                else {//For string variables or methods and invalid chars. All cases handled here.
                    pushPostfix(*e);//Postfix the first char.
                    if(*e<65 || (*e>90&&
                                 *e<97)){//check for invalid symbols like [,{,.,!,^ etc.
                        printf("%s","Error!\n");//Throw an error.
                        isEval=false;
                        flag2=false;
                        breaker=false;
                        break;
                    }
                    counter++;//in case of assignment expression so increment counter by 1. (Ex. ab+a = 5 Invalid!!!)
                    e++;
                    while (isDifferent(*e) == 1 && *e != ' ' && *e!='\n' && *e!=',') {//Check for if string continues or not.
                        if(*e<65 || (*e>90&&
                                     *e<97)){//Throw an error for invalid tokens.
                            printf("%s","Error!\n");
                            isEval=false;
                            flag2=false;
                            breaker=false;//to break the input process.
                            break;
                        }
                        innertop++;//Increment innertor.
                        pushPostfix(*e);//Push *e into the inner index.
                        e++;
                    }
                    if(!breaker)
                        break;//If breaker false break the loop. Then ask for new prompt from user.
                    if(strcmp(postfix[pTop],"ls")==0 ||strcmp(postfix[pTop],"xor")==0
                     ||strcmp(postfix[pTop],"lr")==0  ||strcmp(postfix[pTop],"rs")==0
                     ||strcmp(postfix[pTop],"rr")==0 ||strcmp(postfix[pTop],"not")==0){//Case of methods.
                        if(strcmp(postfix[pTop],"ls")==0){
                            push('^');//Arbitrary assignment for each methods. Facilitating of recognizing method when dealing with postfix.
                            postfix[pTop][innertop]='\0';//Remove from postfix.
                            postfix[pTop][--innertop]='\0';//Remove from postfix. Because we push them above in case of they are variables.
                            pTop--;
                        }
                        else if(strcmp(postfix[pTop],"xor")==0){
                            push('$');//Arbitrary assignment for each methods. Facilitating of recognizing method when dealing with postfix.
                            postfix[pTop][innertop]='\0';
                            postfix[pTop][--innertop]='\0';
                            postfix[pTop][--innertop]='\0';//Remove from postfix. Because we push them above in case of they are variables.
                            pTop--;//3 times because "xor" consists of 3 chars.
                        }
                        else if(strcmp(postfix[pTop],"lr")==0){
                            push('/');//Arbitrary assignment for each methods. Facilitating of recognizing method when dealing with postfix.
                            postfix[pTop][innertop]='\0';
                            postfix[pTop][--innertop]='\0';//Remove from postfix.
                            pTop--;
                        }
                        else if(strcmp(postfix[pTop],"rs")==0){
                            push('#');//Arbitrary assignment for each methods. Facilitating of recognizing method when dealing with postfix.
                            postfix[pTop][innertop]='\0';
                            postfix[pTop][--innertop]='\0';//Remove from postfix.
                            pTop--;
                        }
                        else if(strcmp(postfix[pTop],"rr")==0){
                            push('{');//Arbitrary assignment for each methods. Facilitating of recognizing method when dealing with postfix.
                            postfix[pTop][innertop]='\0';
                            postfix[pTop][--innertop]='\0';//Remove from postfix.
                            pTop--;
                        }
                        else if(strcmp(postfix[pTop],"not")==0){
                            push('~');//Arbitrary assignment for each methods. Facilitating of recognizing method when dealing with postfix.
                            postfix[pTop][innertop]='\0';
                            postfix[pTop][--innertop]='\0';
                            postfix[pTop][--innertop]='\0';//Remove each three char from postfix.
                            pTop--;
                        }
                        if(error==1 || error==4 || error==5){//Error 1 --> variable or integer,Error 4 --> ) Error 5--> methods
                            printf("%s","Error!\n");//They cannot be next each other directly. So throw an error.
                            isEval=false;
                            flag2=false;
                            break;
                        }
                        error=5;//Update the error for corresponding values which is 5 for methods.
                    }

                    else{//Error handling for variables
                        if(error==0|| error==2 ||error==3){ //Variables can locate at the most beginning and after operator and (.
                            error=1;
                        } else{//Else throw an error.
                            printf("%s","Error!\n");
                            isEval=false;
                            flag2=false;
                            break;
                        }
                    }
                    pTop++;
                    innertop = 0;
                    continue;//Iterate again after updating pTop and innertop values.
              }
                e++;
            }
            if(balanceTop!=-1 && flag2){//Checking for balanceTop and if not -1 meaning that there is inbalance.
                printf("%s","Error!\n");//And flag2 is used for preventing from throwing error 2 times.So we assigned it as false at upper error conditions.
                isEval=false;
            }
            if((error==2|| error==3|| error==5) &&flag2){//Case if input ends with operator or ( or ls,xr ...
                printf("%s","Error!\n");//Throw an error.
                isEval=false;
            }
            while (top != -1) {//Popping reamining tokens from stack and pushing into the postfix array.

                pushPostfix(pop());
                pTop++;
                innertop = 0;
            }
           if(isEval) evalPost();//if isEval true then returning numerical result. We assigned it false when error occurred.
            memset(postfix,'\0', sizeof(postfix));//Reset postfix array.
            memset(stack,'\0', sizeof(stack));//Reset stack.
            top =-1;
            pTop=0;
            innertop=0;
        }
        return 0;
    }



