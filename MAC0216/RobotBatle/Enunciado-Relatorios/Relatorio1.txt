Integrantes do grupo:
Larissa Sala
Matheus Laurentys
Nathalia Borin

Observações:
1)Para realizar chamadas de funções, seguir a ordem:

    SAVE
    CALL A
    REST
A:      ALC X     
    <sua função aqui>
    FRE X
    RET

2)Sempre alocar e liberar a mesma quantia de memória
    ALC X
    FRE X

3)Compilar os programas de teste com:
    cc -c teste -o teste.o
    cc maq.o pilha.o teste.o -o teste
    (como está no Makefile)
