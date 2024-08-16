#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdint.h>
#include "lib.h"


#define memory_size 2048
uint16_t ir;
uint16_t memory[memory_size];
uint16_t pc = 1;
uint16_t reg[8];
uint16_t computador_ligado = 1;
uint16_t opcode;
uint16_t destiny;
uint16_t op1;
uint16_t op2;
uint16_t immediate;
uint16_t format;
uint16_t increment = 0;


void decode_op1(uint16_t instruction){

	op1 = extract_bits(instruction, 3, 3);
    
}

void decode_op2(uint16_t instruction){

	op2 = extract_bits(instruction, 0, 3);
    
}

void decode_destiny(uint16_t instruction){

	destiny = extract_bits(instruction, 6, 3);
    
}

void decode_opcodeR(uint16_t instrucao){

	opcode = extract_bits(instrucao, 9, 6);

}

void decode_opcodeI(uint16_t instrucao){

	opcode = extract_bits(instrucao, 13, 2);
    
}
 
void decode_regI(uint16_t instrucao){
 
   op1 = extract_bits(instrucao, 10, 3);
   
 }
 
void decode_immediate(uint16_t instrucao){
 
	immediate = extract_bits(instrucao, 0, 10);
	printf("Binario imediato: %u\n", instrucao);

 }
   
void decode_format(uint16_t instrucao){

	format = extract_bits(instrucao, 15, 1);
	printf("\n\n\nNumero do formato: %d\n", format);
   
}

void decode_all(){
	decode_format(ir);

	if(format == 0){
  		 decode_opcodeR(ir);
  		 decode_op1(ir);
  		 decode_op2(ir);
  		 decode_destiny(ir);
	}
	else{
  		 decode_opcodeI(ir);
  		 decode_regI(ir);
  		 decode_immediate(ir);
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
	}
    
}


void jump(uint16_t destino){

	pc = destino;
	increment = 1;
    
}

void jump_cond(uint16_t destino, uint16_t condicao ){
    
	if(condicao == 1){
  		 pc = destino;
  		 increment = 1;
  		 printf("Realizado o jump condicional, pc atual: %d\n", pc);
	}
    
}

void execute_r(){
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
  		  printf("Encerrando...\n");    
  		  syscall();
  		  break;
  		 
  	  default:
  		  printf("Erro: Opcode desconhecido\n");
  		  exit(1);    	 
	}
}

void execute_i(){
  	  printf("Formato I\n");
  	  switch(opcode){ 	 
  		  case 0:
  			  printf("Realizando jump incondicional para o indice de memoria %d\n", immediate);
  			  jump(immediate);
  			  printf("PC atual: %d\n", pc);
  			  break;
  			 
  		  case 1:
  			  printf("Realizando jump condicional para o indice de memoria %d\n", immediate);
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

void execute(){

	if(format == 0){
  	 	execute_r();
	}
	else{
  	 	execute_i();
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

	while(computador_ligado != 0){
  		 increment = 0;
  		 ir = memory[pc];
  		 decode_all();
  		 execute();
  		 
  		 if(increment == 0){
        	pc += 1;
  		 }
	}
    
	for(i= 0; i< 50; i++){
   	  printf("%i ", (int)memory[i]);
   	 
	}
	printf("\n");
	return 0;
}



































