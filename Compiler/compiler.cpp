#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <map>
//#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

char CheckIfMnemo(char* str);
char CheckIfDirct(char* str);

void ParseDirct(char* dirct, char* line);
void ParseMnemo(char* mnemo, char* line);

void compile_ORG(char* line);
void compile_LABEL(char* line);
void compile_DATA(char* line);
void compile_MDATA(char* line);

int getOpcode(char* mnem);
int getOpSize(char* mnem);

std::string stripString(char* str);

std::string undefinedLabel(char* label);

// Opcodes
#define STOP_OPC 0
#define LOADA_OPC 1
#define LOADI_OPC 2
#define LOADR_OPC 3
#define STOREA_OPC 4
#define STORER_OPC 5
#define ADDA_OPC 6
#define ADDI_OPC 7
#define ADDR_OPC 8
#define SUBA_OPC 9
#define SUBI_OPC 10
#define SUBR_OPC 11
#define MULA_OPC 12
#define MULI_OPC 13
#define MULR_OPC 14
#define DIVA_OPC 15
#define DIVI_OPC 16
#define DIVR_OPC 17
#define JZ_OPC 18
#define JNZ_OPC 19
#define JL_OPC 20
#define JLE_OPC 21
#define JG_OPC 22
#define JGE_OPC 23
#define JMP_OPC 24
#define MOD_OPC 25
#define PUSH_OPC 26
#define POP_OPC 27
#define INC_OPC 28
#define DEC_OPC 29
#define SHL_OPC 30
#define SHR_OPC 31
#define INT_OPC 32
#define ANDA_OPC 33
#define ANDI_OPC 34
#define ANDR_OPC 35
#define ORA_OPC 36
#define ORI_OPC 37
#define ORR_OPC 38
#define XORA_OPC 39
#define XORI_OPC 40
#define XORR_OPC 41
#define CMPA_OPC 42
#define CMPI_OPC 43
#define CMPR_OPC 44
#define CALL_OPC 45
#define VAR_OPC 46
#define PTR_OPC 47
#define LOADV_OPC 48
#define LOADP_OPC 49
#define STOREP_OPC 50
#define STOREV_OPC 51

// Instruction sizes
#define STOP_SIZE 1
#define LOADA_SIZE 2
#define LOADI_SIZE 2
#define LOADR_SIZE 2
#define STOREA_SIZE 2
#define STORER_SIZE 2
#define ADDA_SIZE 2
#define ADDI_SIZE 2
#define ADDR_SIZE 2
#define SUBA_SIZE 2
#define SUBI_SIZE 2
#define SUBR_SIZE 2
#define MULA_SIZE 2
#define MULI_SIZE 2
#define MULR_SIZE 2
#define DIVA_SIZE 2
#define DIVI_SIZE 2
#define DIVR_SIZE 2
#define JZ_SIZE 2
#define JNZ_SIZE 2
#define JL_SIZE 2
#define JLE_SIZE 2
#define JG_SIZE 2
#define JGE_SIZE 2
#define JMP_SIZE 2
#define MOD_SIZE 2
#define PUSH_SIZE 1
#define POP_SIZE 1
#define INC_SIZE 1
#define DEC_SIZE 1
#define SHL_SIZE 2
#define SHR_SIZE 2
#define INT_SIZE 2
#define ANDA_SIZE 2
#define ANDI_SIZE 2
#define ANDR_SIZE 2
#define ORA_SIZE 2
#define ORI_SIZE 2
#define ORR_SIZE 2
#define XORA_SIZE 2
#define XORI_SIZE 2
#define XORR_SIZE 2
#define CMPA_SIZE 2
#define CMPI_SIZE 2
#define CMPR_SIZE 2
#define CALL_SIZE 2
#define VAR_SIZE 2
#define PTR_SIZE 2
#define LOADV_SIZE 2
#define LOADP_SIZE 2
#define STOREP_SIZE 2
#define STOREV_SIZE 2

// List of mnemonics
const char* mnems[] = {"stop", "loada", "loadi", "loadr", "loadv", "loadp", "storea", "storer", "storev", "storep", "adda", "addi", "addr", 
"suba", "subi", "subr", "mula", "muli", "mulr", "diva", "divi", "divr",
"jz", "jnz", "jl", "jle", "jg", "jge", "jmp", "mod", "push", "pop", "inc", "dec",
"shl", "shr", "int", "anda", "andi", "andr", "ora", "ori", "orr", "xora", "xori", "xorr",
"cmpa", "cmpi", "cmpr", "call", "var", "ptr"};

int mnemLen = sizeof(mnems)/sizeof(mnems[0]);

const char* jumps[] = {"jz", "jnz", "jl", "jle", "jg", "jge", "jmp, call"};
int jmplen = sizeof(jumps)/sizeof(jumps[0]);

// List of directives
const char* dircts[] = {"org", "label", "data", "mdata"};
int dirctLen = sizeof(dircts)/sizeof(dircts[0]);

// Compiler config
uint32_t src_counter = 0; // position from which the code starts
std::vector<int> code;  // code segment
std::map<std::string, int> labels;

std::vector<int> CompileToMemory(char* sourceFile)
{
    char line[64];
    char* nread;
    size_t len = 64; 
    char delim[] = " ";
    char* token;

    char temp[64];
    // 1. Getting the assembly code source file
    FILE *stream = fopen(sourceFile, "r");
    if(stream == NULL)
    {
        printf("Source file could not be opened for compilation.\n");
        exit(1);
    }

    // 2. Parsing the assembly code line-by-line
    while((nread = fgets(line, len, stream)) != NULL)
    {
        if(strcmp(nread, "") == 0 || strcmp(nread, "\n") == 0) 
            continue;

        //printf("\n%s\n", nread);
        if(nread[strlen(nread)-1] == '\n')
        {
            nread[strlen(nread)-1] = '\0';
        }

        strcpy(temp, nread);
        token = strtok(nread, delim);

        for(int i = 0; token[i]; i++)
            token[i] = tolower(token[i]);

        if(CheckIfDirct(token) == 1)
            ParseDirct(token, temp); 
        else if(CheckIfMnemo(token) == 1)
            ParseMnemo(token, temp); 
    }

    // Closing operation
    fclose(stream);
   /* printf("\n\nFinal compilation result: \n");
    printf("[");
        std::for_each(code.begin(), code.end(),[](int i){
            printf("%d,", i);
        });
    printf("]");*/
    return code;
}

void CompileToFile(char* sourceFile, std::string output)
{
    std::vector<int> machineCode = CompileToMemory(sourceFile);
    if(machineCode[machineCode.size()-1] != 0)
    {
        std::cout << "\nThe program has no stop at the end! compilation failed\n" << std::endl;
        exit(-1);
    } 
    std::ofstream ofile(output);

    std::stringstream machineCodeString;
    std::copy(machineCode.begin(), machineCode.end(), std::ostream_iterator<int>(machineCodeString, " "));

    ofile << machineCodeString.str().c_str();
    ofile.close();
}

char CheckIfMnemo(char* str)
{   
    for(int i = 0; i < mnemLen; i++)
    {
        if(strcmp(str, mnems[i]) == 0)
            return 1;
    }
    return 0;
}

char CheckIfDirct(char* str)
{
    for(int i = 0; i < dirctLen; i++)
    {
       if(strcmp(str, dircts[i]) == 0)
            return 1;
    }
    return 0;
}

void ParseDirct(char* dirct, char* line)
{
    //printf("Parsing dirct: %s\n", dirct);
    if(strcmp(dirct, "org") == 0)
    {
        compile_ORG(line);
    }
    if(strcmp(dirct, "label") == 0)
    {
        compile_LABEL(line);
    }
    if(strcmp(dirct, "data") == 0)
    {
        compile_DATA(line);
    }
    if(strcmp(dirct, "mdata") == 0)
    {
        compile_MDATA(line);
    }
}

void ParseMnemo(char* mnemo, char* line)
{
    //printf("Parsing mnemo: %s\n", mnemo);
    char delim[] = " ";
    char* mnemToken = strtok(line, delim);

    for(int i = 0; i < jmplen; i++)
    {
        if(strcmp(mnemToken, jumps[i]) == 0)
        {
            //it is a jump, now check if label is correct
            char* jumpTarget = strtok(NULL, delim);
            if(labels.find(jumpTarget) == labels.end())
            {
                //not found
                std::string undefLabel = undefinedLabel(jumpTarget);
                std::cout << undefLabel << std::endl;
                
                //insert jump instruction into code
                int opcode = getOpcode(mnemToken);
                code.insert(code.end(), opcode);

                //insert undefined label into code
                for(int i = 0; i < undefLabel.length(); i++)
                {
                    code.insert(code.end(), undefLabel[i]);
                }

                /*printf("[");
                std::for_each(code.begin(), code.end(),[](int i){
                    printf("%d,", i);
                });
                printf("]");*/
            }
            else
            {
                //insert jump instruction into code
                int opcode = getOpcode(mnemToken);
                code.insert(code.end(), opcode);

                int counterDiff = labels.at(jumpTarget) - (src_counter + 2); // This line may throw an exception
                code.insert(code.end(), opcode);
                code.insert(code.end(), counterDiff);
            }
            int opsize = getOpSize(mnemToken);
            src_counter += opsize;
            return;
        }
    }
    //
    // This is not a jump
    int opcode = getOpcode(mnemToken);
    code.insert(code.end(), opcode);
    int opsize = getOpSize(mnemToken);
    if(opsize > 1)
    {
        char* arg = strtok(NULL, delim);
        int t = atoi(arg);
        if(t == 0)
        {
            //since atoi means UB, we need to double check apparently :/
            t = (int)arg[0];
            if(t >= 48 && t <= 57)
            {
                t -= 48;
            }
        }
        code.insert(code.end(), t);
    }
    src_counter += opsize;

    /* printf("[");
        std::for_each(code.begin(), code.end(),[](int i){
            printf("%d,", i);
        });
    printf("]");*/
    return;
}   

void compile_ORG(char* line)
{
    char delim[] = " ";
    char* orgToken = strtok(line, delim);
    uint16_t paramToken = atoi(strtok(NULL, delim));

    printf("%s %d\n", orgToken, paramToken);

    if(src_counter > paramToken)
    {
        printf("Error when compiling org %d: counter value is incorrect", paramToken);
        exit(1);
    }
    else if(src_counter < paramToken)
    {
        // Probably will need to modify this function here ...
        src_counter = paramToken;
    }
}

void compile_LABEL(char* line)
{
    char delim[] = " ";
    char* orgToken = strtok(line, delim);
    std::string paramToken = strtok(NULL, delim);

    if(labels.find(paramToken) == labels.end())
    {
        //not found, adding label to our label list
        labels.insert({paramToken, src_counter});

        for(auto it = labels.cbegin(); it != labels.cend(); ++it)
        {
            std::cout << it->first << " " << it->second << std::endl; 
        }
    }
    else
    {
        //such label already exists!
        printf("label '%s' already defined!", paramToken);
        exit(1);
    }
}

void compile_DATA(char* line)
{
    char delim[] = " ";
    char* orgToken = strtok(line, delim);
    std::string paramToken = strtok(NULL, delim);

    for(int i = 0; i < paramToken.length(); i++)
    {
        if(paramToken[i] != '\n')
            code.insert(code.end(), (int)paramToken[i] -0x30);
    }

   /* printf("[");
    std::for_each(code.begin(), code.end(),[](int i){
        printf("%d,", i);
    });
    printf("]");*/
}

void compile_MDATA(char* line)
{
    // Won't be implementing this for now
    printf("mdata is not implemented");
}

std::string undefinedLabel(char* label)
{
    std::string newLabel = "?";
    newLabel += label;
    return newLabel;
}

int getOpcode(char* mnem)
{
    if(strcmp(mnem, "stop") == 0)
        return STOP_OPC;
    if(strcmp(mnem, "loada") == 0)
        return LOADA_OPC;
    if(strcmp(mnem, "loadi") == 0)
        return LOADI_OPC;
    if(strcmp(mnem, "loadr") == 0)
        return LOADR_OPC;
    if(strcmp(mnem, "storea") == 0)
        return STOREA_OPC;
    if(strcmp(mnem, "storer") == 0)
        return STORER_OPC;
    if(strcmp(mnem, "adda") == 0)
        return ADDA_OPC;
    if(strcmp(mnem, "addi") == 0)
        return ADDI_OPC;
    if(strcmp(mnem, "addr") == 0)
        return ADDR_OPC;
    if(strcmp(mnem, "suba") == 0)
        return SUBA_OPC;
    if(strcmp(mnem, "subi") == 0)
        return SUBI_OPC;
    if(strcmp(mnem, "subr") == 0)
        return SUBR_OPC;
    if(strcmp(mnem, "mula") == 0)
        return MULA_OPC;
    if(strcmp(mnem, "muli") == 0)
        return MULI_OPC;
    if(strcmp(mnem, "mulr") == 0)
        return MULR_OPC;
    if(strcmp(mnem, "diva") == 0)
        return DIVA_OPC;
    if(strcmp(mnem, "divi") == 0)
        return DIVI_OPC;
    if(strcmp(mnem, "divr") == 0)
        return DIVR_OPC;
    if(strcmp(mnem, "jz") == 0)
        return JZ_OPC;
    if(strcmp(mnem, "jnz") == 0)
        return JNZ_OPC;
    if(strcmp(mnem, "jl") == 0)
        return JL_OPC;
    if(strcmp(mnem, "jle") == 0)
        return JLE_OPC;
    if(strcmp(mnem, "jg") == 0)
        return JG_OPC;
    if(strcmp(mnem, "jge") == 0)
        return JGE_OPC;
    if(strcmp(mnem, "jmp") == 0)
        return JMP_OPC;
    if(strcmp(mnem, "mod") == 0)
        return MOD_OPC;
    if(strcmp(mnem, "push") == 0)
        return PUSH_OPC;
    if(strcmp(mnem, "pop") == 0)
        return POP_OPC;
    if(strcmp(mnem, "inc") == 0)
        return INC_OPC;
    if(strcmp(mnem, "dec") == 0)
        return DEC_OPC;
    if(strcmp(mnem, "shr") == 0)
        return SHR_OPC;
    if(strcmp(mnem, "shl") == 0)
        return SHL_OPC;
    if(strcmp(mnem, "int") == 0)
        return INT_OPC;
    if(strcmp(mnem, "anda") == 0)
        return ANDA_OPC;
    if(strcmp(mnem, "andi") == 0)
        return ANDI_OPC;
    if(strcmp(mnem, "andr") == 0)
        return ANDR_OPC;
    if(strcmp(mnem, "ora") == 0)
        return ORA_OPC;
    if(strcmp(mnem, "ori") == 0)
        return ORI_OPC;
    if(strcmp(mnem, "orr") == 0)
        return ORR_OPC;
    if(strcmp(mnem, "xora") == 0)
        return XORA_OPC;
    if(strcmp(mnem, "xori") == 0)
        return XORI_OPC;
    if(strcmp(mnem, "xorr") == 0)
        return XORR_OPC;
    if(strcmp(mnem, "cmpa") == 0)
        return CMPA_OPC;
    if(strcmp(mnem, "cmpi") == 0)
        return CMPA_OPC;
    if(strcmp(mnem, "cmpr") == 0)
        return CMPR_OPC;
    if(strcmp(mnem, "call") == 0)
        return CALL_OPC;
    if(strcmp(mnem, "loadv") == 0)
        return LOADV_OPC;
    if(strcmp(mnem, "loadp") == 0)
        return LOADP_OPC;
    if(strcmp(mnem, "storev") == 0)
        return STOREV_OPC;
    if(strcmp(mnem, "storep") == 0)
        return STOREP_OPC;
    if(strcmp(mnem, "var") == 0)
        return VAR_OPC;
    if(strcmp(mnem, "ptr") == 0)
        return PTR_OPC;
    return 999;
}

int getOpSize(char* mnem)
{
       if(strcmp(mnem, "stop") == 0)
        return STOP_SIZE;
    if(strcmp(mnem, "loada") == 0)
        return LOADA_SIZE;
    if(strcmp(mnem, "loadi") == 0)
        return LOADI_SIZE;
    if(strcmp(mnem, "loadr") == 0)
        return LOADR_SIZE;
    if(strcmp(mnem, "storea") == 0)
        return STOREA_SIZE;
    if(strcmp(mnem, "storer") == 0)
        return STORER_SIZE;
    if(strcmp(mnem, "adda") == 0)
        return ADDA_SIZE;
    if(strcmp(mnem, "addi") == 0)
        return ADDI_SIZE;
    if(strcmp(mnem, "addr") == 0)
        return ADDR_SIZE;
    if(strcmp(mnem, "suba") == 0)
        return SUBA_SIZE;
    if(strcmp(mnem, "subi") == 0)
        return SUBI_SIZE;
    if(strcmp(mnem, "subr") == 0)
        return SUBR_SIZE;
    if(strcmp(mnem, "mula") == 0)
        return MULA_SIZE;
    if(strcmp(mnem, "muli") == 0)
        return MULI_SIZE;
    if(strcmp(mnem, "mulr") == 0)
        return MULR_SIZE;
    if(strcmp(mnem, "diva") == 0)
        return DIVA_SIZE;
    if(strcmp(mnem, "divi") == 0)
        return DIVI_SIZE;
    if(strcmp(mnem, "divr") == 0)
        return DIVR_SIZE;
    if(strcmp(mnem, "jz") == 0)
        return JZ_SIZE;
    if(strcmp(mnem, "jnz") == 0)
        return JNZ_SIZE;
    if(strcmp(mnem, "jl") == 0)
        return JL_SIZE;
    if(strcmp(mnem, "jle") == 0)
        return JLE_SIZE;
    if(strcmp(mnem, "jg") == 0)
        return JG_SIZE;
    if(strcmp(mnem, "jge") == 0)
        return JGE_SIZE;
    if(strcmp(mnem, "jmp") == 0)
        return JMP_SIZE;
    if(strcmp(mnem, "mod") == 0)
        return MOD_SIZE;
    if(strcmp(mnem, "push") == 0)
        return PUSH_SIZE;
    if(strcmp(mnem, "pop") == 0)
        return POP_SIZE;
    if(strcmp(mnem, "inc") == 0)
        return INC_SIZE;
    if(strcmp(mnem, "dec") == 0)
        return DEC_SIZE;
    if(strcmp(mnem, "shr") == 0)
        return SHR_SIZE;
    if(strcmp(mnem, "shl") == 0)
        return SHL_SIZE;
    if(strcmp(mnem, "int") == 0)
        return INT_SIZE;
    if(strcmp(mnem, "anda") == 0)
        return ANDA_SIZE;
    if(strcmp(mnem, "andi") == 0)
        return ANDI_SIZE;
    if(strcmp(mnem, "andr") == 0)
        return ANDR_SIZE;
    if(strcmp(mnem, "ora") == 0)
        return ORA_SIZE;
    if(strcmp(mnem, "ori") == 0)
        return ORI_SIZE;
    if(strcmp(mnem, "orr") == 0)
        return ORR_SIZE;
    if(strcmp(mnem, "xora") == 0)
        return XORA_SIZE;
    if(strcmp(mnem, "xori") == 0)
        return XORI_SIZE;
    if(strcmp(mnem, "xorr") == 0)
        return XORR_SIZE;
    if(strcmp(mnem, "cmpa") == 0)
        return CMPA_SIZE;
    if(strcmp(mnem, "cmpi") == 0)
        return CMPA_SIZE;
    if(strcmp(mnem, "cmpr") == 0)
        return CMPR_SIZE;
    if(strcmp(mnem, "call") == 0)
        return CALL_SIZE;
    if(strcmp(mnem, "loadv") == 0)
        return LOADV_SIZE;
    if(strcmp(mnem, "loadp") == 0)
        return LOADP_SIZE;
    if(strcmp(mnem, "storev") == 0)
        return STOREV_SIZE;
    if(strcmp(mnem, "storep") == 0)
        return STOREP_SIZE;
    if(strcmp(mnem, "var") == 0)
        return VAR_SIZE;
    if(strcmp(mnem, "ptr") == 0)
        return PTR_SIZE;
    return 999;
}