#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdint.h>
#include "lib.h"


#define memory_size 2048

uint16_t memory[memory_size];
uint16_t reg[8];
uint16_t computador_ligado = 1;
uint16_t previsao_correta;


uint16_t if_ir;
uint16_t if_branch;
uint16_t if_current_pc;
uint16_t if_next_pc;
uint16_t if_pc = 1;


uint16_t id_opcode;
uint16_t id_destiny;
uint16_t id_op1;
uint16_t id_op2;
uint16_t id_immediate;
uint16_t id_format;
uint16_t id_pc;
uint16_t id_ir;
uint16_t id_branch;
uint16_t id_current_pc;
uint16_t id_next_pc; 
uint16_t id_cond = 0;

uint16_t ex_format; 
uint16_t ex_opcode;
uint16_t ex_destiny;
uint16_t ex_op1;
uint16_t ex_op2;
uint16_t ex_immediate;
uint16_t ex_format;
uint16_t ex_branch;
uint16_t ex_current_pc;
uint16_t ex_next_pc; 
uint16_t ex_pc;
uint16_t ex_cond = 0;


typedef enum{
    livre,ocupado
} Estado_tabela;

typedef struct {
    uint16_t destiny_pc;
    uint16_t predicted_taken;
    uint16_t jump_pc;
    Estado_tabela estado;
} BranchPredictionEntry;


#define table_memory_size 128
BranchPredictionEntry branch_prediction_table[table_memory_size];

void init_table() {
    for (int i = 0; i < table_memory_size; ++i) {
       	branch_prediction_table[i].destiny_pc = 0;
       	branch_prediction_table[i].predicted_taken = 0;
       	branch_prediction_table[i].jump_pc = 0;  
       	branch_prediction_table[i].estado = livre;
    }
}


void add(uint16_t destino, uint16_t r1, uint16_t r2){

    reg[destino] = reg[r1] + reg[r2];
    
}

void sub(uint16_t destino, uint16_t r1, uint16_t r2){

    reg[destino] = reg[r1] - reg[r2];
    
}

void mul(uint16_t destino, uint16_t r1, uint16_t r2){

    reg[destino] = reg[r1] * reg[r2];
    
}

void division(uint16_t destino, uint16_t r1, uint16_t r2){

    reg[destino] = reg[r1] / reg[r2];
    
}


void cmp_equal(uint16_t destino, uint16_t r1, uint16_t r2){

    reg[destino] = (reg[r1] == reg[r2]);
    
}

void cmp_neq(uint16_t destino, uint16_t r1, uint16_t r2){

    reg[destino]= (reg[r1] != reg[r2]);
    
}

void load(uint16_t destino,uint16_t endereco){

    reg[destino] = memory[endereco];

}

void store(uint16_t endereco, uint16_t r2){

    memory[endereco] = reg[r2];
}

void mov(uint16_t r1, uint16_t immediate){

    reg[r1] = immediate;
}

void syscall(){

    if(reg[0] == 0){
   	    computador_ligado = 0;
   	     ex_cond = 0;
         id_cond = 0;
    }
}

void jump(uint16_t destino){

    previsao_correta = (destino == ex_next_pc); 
       
}

void jump_cond(uint16_t destino, uint16_t condicao ){

    if(condicao == 1){
        previsao_correta = (destino == ex_next_pc);
        printf("Realizado o jump condicional, pc atual: %d\n", ex_next_pc);
 	}
 	else{
	    previsao_correta = ((ex_current_pc + 1) == ex_next_pc);
	    printf("Jump condicional nao realizado, pc atual: %d\n", ex_next_pc);
 	}  
}

void execute_r(uint16_t opcode, uint16_t destiny, uint16_t op1, uint16_t op2){
    printf("Formato R\n");
    switch(opcode){
   	 case 0:
   		 printf("Soma do registrador %d com o registrador %d armazenada no registrador %d\n", op1, op2, destiny);
   		 add(destiny, op1, op2);
     		 
   		 printf("Resultado da Soma: %d\n", reg[destiny]);
   		 break;
     		 
   	 case 1:
   		 printf("Subtracao do registrador %d com o registrador %d armazenada no registrador %d\n", op1, op2, destiny);
   		 sub(destiny, op1, op2);
     		 
   		 printf("Resultado da Subtracao: %d\n", reg[destiny]);
   		 break;
     		 
   	 case 2:
   		 printf("Multiplicacao do registrador %d com o registrador %d armazenada no registrador %d\n", op1, op2, destiny);
   		 mul(destiny, op1, op2);
     		 
   		 printf("Resultado da Multiplicao: %d\n", reg[destiny]);
   		 break;
     		 
   	 case 3:
   		 printf("Divisao do registrador %d com o registrador %d armazenada no registrador %d\n", op1, op2, destiny);
   		 division(destiny, op1, op2);	 
   		 printf("Resultado da Divisao: %d\n", reg[destiny]);
   		 break;	 
   		 
   	 case 4:
   		 printf("Comparando igualdade:\n");
   		 printf("Valor do registrador %d: %d\n", op1, reg[op1]);
   		 printf("Valor do registrador %d: %d\n", op2, reg[op2]);
   		 cmp_equal(destiny,op1,op2);
   		 if(reg[destiny] == 1){
   		    printf("Sao iguais\n");
   		 }
   		 break;
     		 
   	 case 5:
   		 printf("Comparando se os valores dos registradores %d e %d sao diferentes para armazenar no registrador %d\n", op1, op2, destiny);
   		 cmp_neq(destiny,op1,op2);
   		 if(reg[destiny] == 1){
   		    printf("Sao diferentes\n");
   		 }
   		 break; 
   		 
   	 case 15:
   		 printf("Load do endereco de memoria %d para o registrador %d\n", op1, destiny);
   		 load(destiny, reg[op1]);
   		 printf("Resultado do Load: %d\n", reg[destiny]);
   		 break;
   		 
   	 case 16:
   		 printf("Store do registrador %d para o endereco de memoria %d\n", op1, op2);
   		 store(reg[op1], op2);	 
   		 printf("Resultado do Store : %d\n", memory[op1]);
   		 break;	 
   		 
   	 case 63:
   		 printf("\n\nEncerrando...\n");    
   		 syscall();
   		 break;
   		 
   	 default:
   		 printf("Erro: Opcode desconhecido\n");
   		 exit(1); 	 
    }
}

void execute_i(uint16_t opcode, uint16_t immediate, uint16_t op1){
   	 printf("Formato I\n");
   	 switch(opcode){    
   		 case 0:
   			 printf("Realizando jump incondicional para o indice de memoria %d\n", immediate);
   			 jump(immediate);
   			 printf("PC atual: %d\n", ex_next_pc);
   			 break;
   			 
   		 case 1:
   			 printf("Tentando realizar jump condicional para o indice de memoria %d\n", immediate);
   			 jump_cond(immediate, reg[op1]);
   			 break;
   			 
   		 case 3:
   			 printf("Mov do valor %d para o registrador %d\n", immediate, op1);
   			 mov(op1, immediate);
   			 printf("Valor armazenado no registrador %d: %d\n",op1, reg[op1]);
   			 break;
   			 
   		 default:
   			 printf("Erro: Opcode desconhecido\n");
   			 exit(1);
    
   	 }  
}

void decode_op1(){

    id_op1 = extract_bits(id_ir, 3, 3);
    
}

void decode_op2(){

    id_op2 = extract_bits(id_ir, 0, 3);
    
}

void decode_destiny(){

    id_destiny = extract_bits(id_ir, 6, 3);
}

void decode_opcodeR(){

    id_opcode = extract_bits(id_ir, 9, 6);
    
}

void decode_opcodeI(){

    id_opcode = extract_bits(id_ir, 13, 2);
    
}
 
void decode_regI(){
 
   id_op1 = extract_bits(id_ir, 10, 3);
   
 }
 
void decode_immediate(){
 
    id_immediate = extract_bits(id_ir, 0, 10);
    printf("Binario imediato: %u\n", id_immediate);

 }
   
void decode_format(){

    id_format = extract_bits(id_ir, 15, 1);
    printf("\n\n\nNumero do formato: %d\n", id_format);
   
}

void instruction_fetch(){
   
   	 if_ir = memory[if_pc];
   	 uint16_t index = ex_pc % table_memory_size;
   	 
   	 if(if_pc == branch_prediction_table[index].jump_pc && branch_prediction_table[index].estado == ocupado){
   		 if(branch_prediction_table[index].predicted_taken == 1){
   			 if_branch = 1;
   			 if_current_pc = if_pc;
   			 if_pc = branch_prediction_table[index].destiny_pc;
   		 }
   		 else{
   			 if_branch = 0;
   			 if_current_pc = if_pc;
   			 if_pc += 1;
   		 }
   	 }
   	 else{
   		 if_branch = 0;
   		 if_current_pc = if_pc;
   		 if_pc += 1;
   	 }
   	 
   	 if_next_pc = if_pc;
}

void instruction_decode(){
    
        id_ir = if_ir;
        id_current_pc = if_current_pc;
        id_next_pc = if_next_pc;
        id_branch = if_branch;
   	 
        decode_format();
        
        if(id_format == 0){
            decode_opcodeR();
   	   	    decode_op1();  
   	   	    decode_op2();
   	   	    decode_destiny(); 
        }
        else{
   	   	    decode_opcodeI();
   	   	    decode_regI();
   	   	    decode_immediate();   	  	
        } 
}

void execute(){

        ex_current_pc = id_current_pc;
        ex_next_pc = id_next_pc;
        ex_branch = id_branch;
      	ex_format = id_format;

        if(ex_format == 0){
            ex_opcode = id_opcode;
     	    ex_destiny = id_destiny;
     	    ex_op1 = id_op1;
     	    ex_op2 = id_op2;
   	        execute_r(id_opcode, id_destiny, id_op1, id_op2);
        }
        else{
     	    ex_opcode = id_opcode;
     	    ex_immediate = id_immediate;
     	    ex_op1 = id_op1;
   	        execute_i(ex_opcode, ex_immediate, ex_op1);
   	   
            if(ex_opcode == 0 || ex_opcode == 1){
                if(ex_branch == 1){
                    if(previsao_correta == 0){
                        ex_cond = 0;
         	     		id_cond = 0;
         	     		if_pc = ex_immediate;
       			    }
       			}
                uint16_t index = ex_pc % table_memory_size;
           		branch_prediction_table[index].destiny_pc = ex_immediate;
           		branch_prediction_table[index].predicted_taken = previsao_correta;
           		branch_prediction_table[index].jump_pc = ex_current_pc;  
           		branch_prediction_table[index].estado = ocupado;
       		  
            }
      }    
}

const char* get_reg_name_str (uint16_t reg)
{
    static const char *str[] = {
      "r0",
      "r1",
      "r2",
      "r3",
      "r4",
      "r5",
      "r6",
      "r7"
    };

    return str[reg];
}


int main (int argc, char **argv)
{
    if (argc != 2) {
      printf("usage: %s [bin_name]\n", argv[0]);
      exit(1);
    }
    
    int i;
    
    load_binary_to_memory (argv[1], memory, memory_size);

    init_table();
    
    while(computador_ligado != 0){ 
  		 if(ex_cond == 1){
  		    execute();    
  		 }
  		 if(id_cond == 1){
			instruction_decode();
			ex_cond = 1;
  		 }
  		 if(computador_ligado != 0){
  		    instruction_fetch();
  		    id_cond = 1;
  		 }	 
    }
    
    for(i= 0; i< 50; i++){
   	  printf("%i ", (int)memory[i]);
   	 
	}
	printf("\n");
    return 0;
}


