# HASS - Hypothetical Architecture Simulator System

Esse projeto foi desenvolvido como uma atividade avaliativa para a disciplina de Arquitetura de Computadores, usando uma arquitetura hipótetica vista em sala de aula e que foi criada pelo professor Eduardo Molina Cruz

Para essa atividade dois Simuladores foram feitos, um simulador monociclo que recompensaria até no máximo o conceito B, e um simulador pipeline que valeria um conceito A.

Para ambos os Simuladores dessa Arquitetura Hipotética, era Requisito o desenvolvimento e implementação das seguintes instruções:

Format R
 - Opcode 00 - add
 - Opcode 01 - sub
 - Opcode 02 - mul
 - Opcode 03 - div
 - Opcode 04 - cmp_equal
 - Opcode 05 - cmp_neq
 - Opcode 15 - load
 - Opcode 16 - store
 - Opcode 63 - syscall - Saida do programa (Mov r0, 0)

 Format I:
 - Opcode 00 - jump
 - Opcode 01 - jump_cond
 - Opcode 02 - nop
 - Opcode 03 - mov
