//
//  assembler.c
//  assmbler
//
//  Created by WangShunchang on 2/27/19.
//  Copyright © 2019 WangShunchang. All rights reserved.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "map.h"
#include <assert.h>
#include <stdint.h>

int var = 16;
map dmap;
map symmap;
map cmap;
map jmap;

void destlist(){
    insertKey(dmap, "",  "000");
    insertKey(dmap, "M",  "001");
    insertKey(dmap, "D",  "010");
    insertKey(dmap, "MD", "011");
    insertKey(dmap, "A",  "100");
    insertKey(dmap, "AM", "101");
    insertKey(dmap, "AD", "110");
    insertKey(dmap, "AMD","111");
}
void compList(){
    insertKey(cmap, "0",  "0101010");
    insertKey(cmap, "1",  "0111111");
    insertKey(cmap, "-1", "0111010");
    insertKey(cmap, "D",  "0001100");
    insertKey(cmap, "A",  "0110000");
    insertKey(cmap, "!D", "0001101");
    insertKey(cmap, "!A", "0110001");
    insertKey(cmap, "-D", "0001111");
    insertKey(cmap, "-A", "0110011");
    insertKey(cmap, "D+1","0011111");
    insertKey(cmap, "A+1","0110111");
    insertKey(cmap, "D-1","0001110");
    insertKey(cmap, "A-1","0110010");
    insertKey(cmap, "D+A","0000010");
    insertKey(cmap, "D-A","0010101");
    insertKey(cmap, "A-D","0000111");
    insertKey(cmap, "D&A","0000000");
    insertKey(cmap, "D|A","0010101");
    insertKey(cmap, "M",  "1110000");
    insertKey(cmap, "!M", "1110001");
    insertKey(cmap, "-M", "1110011");
    insertKey(cmap, "M+1","1110111");
    insertKey(cmap, "M-1","1110010");
    insertKey(cmap, "D+M","1000010");
    insertKey(cmap, "D-M","1010011");
    insertKey(cmap, "M-D","1000111");
    insertKey(cmap, "D&M","1000000");
    insertKey(cmap, "D|M","1010101");

}
void jmaplist(){
    insertKey(jmap, "",    "000");
    insertKey(jmap, "JGT", "001");
    insertKey(jmap, "JEQ", "010");
    insertKey(jmap, "JGE", "011");
    insertKey(jmap, "JGT", "100");
    insertKey(jmap, "JNE", "101");
    insertKey(jmap, "JLE", "110");
    insertKey(jmap, "JMP", "111");
}
void symtable(){
    insertKey(symmap, "R0", "0");
    insertKey(symmap, "R1", "1");
    insertKey(symmap, "R2", "2");
    insertKey(symmap, "R3", "3");
    insertKey(symmap, "R4", "4");
    insertKey(symmap, "R5", "5");
    insertKey(symmap, "R6", "6");
    insertKey(symmap, "R7", "7");
    insertKey(symmap, "R8", "8");
    insertKey(symmap, "R9", "9");
    insertKey(symmap, "R10", "10");
    insertKey(symmap, "R11", "11");
    insertKey(symmap, "R12", "12");
    insertKey(symmap, "R13", "13");
    insertKey(symmap, "R14", "14");
    insertKey(symmap, "R15", "15");
    insertKey(symmap, "SCREEN", "16384");
    insertKey(symmap, "KBD", "24576");
    insertKey(symmap, "SP", "0");
    insertKey(symmap, "LCL", "1");
    insertKey(symmap, "ARG", "2");
    insertKey(symmap, "THIS", "3");
    insertKey(symmap, "THAT", "4");
}


char *parse(char* line) {
    char *start = line;  //two pointers
    char *end = line;
    while (strchr(" ", *start) != NULL)
    {     start++;     // skip spaces
    }
    while (*end != '\0' && strchr("/\n\r", *end) == NULL) {
        end++;
    }
    *end = '\0';
    return start;
}
void RemoveSpaces(char* source)
{
    char* i = source;
    char* j = source;
    while(*j != 0)
    {
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = 0;
}

void helper(char* file1, char* file2){
    char *newline =(char*)calloc(200, sizeof(char));
    char *outline =(char*)calloc(200, sizeof(char));
    FILE *fp;               //initilize the input
    FILE *output;           //initilize the output
    fp = fopen(file1, "r");     //read input file
    output = fopen(file2 , "w"); //write outputfile
    if (fp == NULL)       //input empty, exit
    {   printf(" fp is null");
        exit(1);
    }
    if(output == NULL)
    {   printf(" output is null");
        exit(1);
    }
    fgets(newline , 200, fp);
    while (!feof(fp)){ // while not reach the end of file
        outline = parse(newline); // skip the white space
        if (outline[0] == '('){
            fprintf(output, "%s\n", outline); // L command start with (
        }
        else if (outline[0] == '@'){
            fprintf(output, "%s\n", outline); // A command start with @
        }
        else if (outline[0] == '/' || outline[0] == '\0' ){ //skips comments and blanks between rows
        }
        else {
            RemoveSpaces(outline);
            fprintf(output, "%s\n", outline);    // C command
        }
        fgets(newline , 200, fp); // turn file to char
    }
    
    fclose(fp);
    fclose(output);
    
}

char* IntToBin(int n){ // convert int to binary bits
    int c, d, count;
    char* pointer;
    count = 0;
    pointer = (char*)malloc(32+1);
    if(pointer == NULL)
        exit(1);
    for (c = 15; c >=0; c --){
        d = n >> c;
        if (d & 1)
            *(pointer+count) =  1 + '0';
        else
            *(pointer+count) = 0 + '0';
        count++;
    }
    *(pointer+count) = '\0';
    return pointer;
}

char* codetobinary(char* code){
    char d[100]; char comp[100]; char j[10];
    char* dcode; char* ccode; char* jcode;
    char* value; char* aint; char* asym;
    char* ansym;
    char* binary1 = malloc(17);
    char* vvalue = (char*)calloc(200, sizeof(char));
    if (code[0] == '@')
    { //a command @ number or @ symbol
        int address;
        int match = sscanf(code, "@%d", &address);
        if (match == 1){
        aint = IntToBin(address);
        return aint;}// num
        else {// code is symbol
            char symbol[100];
            match = sscanf(code, "@%s", symbol);
            value = lookupKey(symmap, symbol);
            if(value == NULL){ // symmbol not in symmap
            sprintf(vvalue , "%d", var);
            insertKey(symmap, symbol, vvalue);
            address = var++;
            ansym = IntToBin(address);
            return ansym;
            }
            else { //symbol in symmap
                address = atoi(value);
                asym = IntToBin(address);
                return asym;

            }
            
        }
    }
    else {   //c command
        RemoveSpaces(code);
        if (strchr(code,'=')!= NULL){ //dest = comp like D = M
            sscanf(code,"%[^=]=%s", d , comp);
            ccode = lookupKey(cmap, comp);
            dcode = lookupKey(dmap, d);
            sprintf(binary1,"111%s%s000", ccode,dcode);
            return binary1;
        } else{    //comp;j  like D;JMP
            sscanf(code,"%[^;];%s", comp, j);
            ccode = lookupKey(cmap,comp);
            jcode = lookupKey(jmap,j);
            sprintf(binary1,"111%s000%s", ccode, jcode);
            return binary1;
        }
       }

}
void symmapshow(map map) {
    for (int i=0; i< map->mapSize; i++) {
        char *key = map->pairs[i].key;
        char *vptr = map->pairs[i].value;
        printf("%d: %s, %s\n", i, key, vptr);
    }
    
}

void pass1(char* inFlie){
    char* line=(char*)calloc(200, sizeof(char));
    char* add =(char*)calloc(200, sizeof(char));
    char* code=(char*)calloc(200, sizeof(char));
    int addr = 0;
    FILE *fp = fopen(inFlie, "r"); //read whole program
    if (fp == NULL)       //input empty, exit
    {   printf(" fp is null");
        exit(1);
    }
    fgets(line , 200, fp);
    while (!feof(fp)){ // while not reach the end of file
        code = parse(line);
            if (code[0] == '('){ //L coomand
                char label[100];
                sscanf(code, "(%[^)])", label);
                sprintf(add, "%d", addr);
                if(lookupKey(symmap, label) == NULL){
                    insertKey(symmap, label, add);
                }
            }
            else if (code[0] == '\0'){//skip spaces
            }
            else{
                addr ++; //when encounter A or C construction
            }
        
            fgets(line, 200, fp);
        }
        fclose(fp);
    }

void pass2(char* inFile, char* hackFile){
    char* line = (char*)calloc(200, sizeof(char));
    char* binary =(char*)calloc(200, sizeof(char));
    char* code = (char*)calloc(200, sizeof(char));
    FILE *fp = fopen(inFile, "r");
    FILE *hf = fopen(hackFile, "w");
    if (fp == NULL)       //input empty, exit
    {   printf(" fp is null ");
        exit(1);
    }
    
    
    fgets(line , 200, fp);//read whole program
    while (!feof(fp)){ // while not reach the end of file
        code = parse(line);
        //printf("in the line");
        
            if(code[0] == '('){ //do nothing for l label
                //printf("lcommand");
                
            }
            else if (code[0] == '@'){//a command
                binary = codetobinary(code);
                fprintf(hf, "%s\n", binary);
                /* printf(" a command\n ");
                 printf("%s\n",code);
                 value = lookupKey(symmap, code);
                 if(value == NULL){// symbol not in map or integer

                 binary = codetobinary(nvalue);
                 fprintf(hf, "%s\n", binary);
                 n++;
                 }else{ // if symbol in map
                binary = codetobinary(avalue);
                fprintf(hf, "%s\n", binary);
                 */
            
            }
            else if (code[0] == '/' || code[0] == '\0' ){ //skips comments and blanks between rows
            }
            else { //c command
                // need to find jump dest comp combine together
               //printf(" c command \n");
                RemoveSpaces(code);
                binary = codetobinary(code);
                fprintf(hf, "%s\n", binary);
            }
            fgets(line , 200, fp);
        }
     
    
    
        fclose(fp);
        fclose(hf);
    }

void freemap(){
    freeMap(dmap);
    freeMap(cmap);
    freeMap(jmap);
    freeMap(symmap);
}


int main(int argc, char* argv[]){
    dmap= createMap(100);
    cmap= createMap(200);
    jmap = createMap(100);
    symmap = createMap(10000);
    jmaplist();
    compList();
    destlist();
    symtable();
    //helper(argv[1], argv[2]);
    symmapshow(symmap);
    pass1(argv[1]);
    symmapshow(symmap);
    pass2(argv[1], argv[2]);
    symmapshow(symmap);
    freemap();
    
    /*char* bo = "@S8";
    char* scwang = "";
    scwang = codetobinary(bo);
    printf("%s\n", scwang);
    symDump(symmap);
    */
}
