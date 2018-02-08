#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arena.h"
#include "maq.h"
#include "compila.tab.h"


/*
 * Expande o modo DEBUG criado em maq.c
 * para arena.c. Nesse modo sao impressos
 * todas as modificacoes feitas na arena.
 * Por exemplo, movimento de robos, retirada
 * de e deposito de cristais
 */
#define DEBUG
#ifdef DEBUG
    #define D(X) X
#else
#define D(X)
#endif

/*
 * Condicoes de jogo.
 * "MAX_ROBOS" e o numero de robos por time
 * "MAX_TURNOS" e a quantidade de turnos do
 *     jogo caso nenhum time venca a partida
 * "MAX_TIME" e a quantidade de times na partida
 *     (pensamos em permitir batalhas alem da 1x1)
 * "MAX_INSTR" e o numero de instrucoes que podem
 *     ser executadas durante um turno
 */
#define MAX_ROBOS 5
#define MAX_TURNOS 250
#define MAX_TIME 2
#define MAX_INSTR 25

/*
 * Struct da Arena.
 * Nossa arena e composta de um mapa e de um 
 * numero definido de robos.
 * A struct e definida aqui devido a problemas
 * relacionados a dependencias e inclusoes
 */
typedef struct{
    Maquina robots[10];
    Mapa mapa;
} Arena;

static char *robs[] = {"rob0", "rob1", "rob2", "rob3", "rob4",
                "rob5", "rob6", "rob7", "rob8", "rob9"};

/*
 * Definicao das variaveis globais.
 * "positions" guarda a posicao de robos sendo
 *     positions[k]=pos.X e positions[k+1]=pos.Y
 * "arena" e a unica instancia da struct arena. 
 *     Usamos "arena" como o 'runner' do jogo.
 * "ids" e a variavel usada para acessar cada 
 *     robo pelo 'nome'
 * "instrSize" representa o numero de instrucoes
 * em cada programa - e usada na funcao readProg
 * "display" representa o stream que sera enviaado
 * para apres
 */
static int positions[MAX_ROBOS*MAX_TIME*2];
static Arena arena;
static int ids=0;
static INSTR prog[2000];
FILE *display;

/*
 * O jogo inicia nesse ponto.
 *
 * Recebe como argumento qual programa sera testado
 *
 * "CriaArena" e "insereExercito" sao
 * responsaveis pela inicialização e 
 * "Atualiza" e responsavel pelo 'GameLoop'
 */
int main(int argc, char *argv[]){
    srand(time(NULL));    
    // Abre o pipe que fara a comunicacao com a parte 
    // visual do "apres"
    display = popen("./apres", "w");

    criaArena();
    insereExercito();
    Atualiza();
    return 0;
}

/*
 * Instancia a arena e guarda a posicao dos robos
 * A arena agora já tem posições pré-definidas para
 * as bases e os robôs, porém a disposição dos cristais
 * e dos tipos de terreno é aleatória
 */
void criaArena(){
    int id=0;
    int terr;
    int base=0;
    int cris=0;
    int ntime=5;
    int ocup, i, j, k;
    i = 0;
    j = 0;
    k = 0;

    // Posições importantes:
    // Bases: (1,0) e (18,11)
    // Time1: (0,0); (0,1); (2,0); (3,1); (3,0)
    // Time2: (19, 11); (17,11); (16,11); (19, 10); (17,10)

    // Posiciona os robôs nos tiles pré-determinados
    for (int l = 0; l < 240; l++){
        if (l == 0 || l == 1 || l == 24 || l == 25 || l == 36
            || l == 203 || l == 214 || l == 215 || l == 238 || l == 239){
            ocup = id;
            terr = 1;
            cris = 0;
            arena.mapa.tiles[i][j] = Inicializa(terr, 0, ocup, base);
            id++;
        }

        // Posiciona as bases nos tiles pré-determinados
        else if (l == 12 || l == 227){
            cris = 0;
            ocup = -1;
            base = 1;
            terr = 1;
            arena.mapa.tiles[i][j] = Inicializa(terr, 0, -1, base);
        }

        // Demais tiles: setamos o atributo ocupado para -1 e geramos
        // os terrenos e o número de cristais de forma aleatória
        else {
            ocup = -1;
            terr = rand() % 101;
                                                // Terrenos | Probabilidade
            if (terr <= 40) terr = 0;           // GRAMA    | 40 %
            else if (terr <= 60) terr = 1;      // ESTRADA  | 20 %
            else if (terr <= 65 ) terr = 2;     // MONTANHA | 5 %
            else if (terr <= 80) terr = 3;      // AGUA     | 15 %
            else terr = 4;                      // AREIA    | 20 %

            // Chance de um tile possuir cristais: 60%
            // Número máximo de cristais por tile: 10
            cris = rand() % 101;
            if (cris <= 60) cris = rand() % 11;
            else cris = 0;
            arena.mapa.tiles[i][j] = Inicializa(terr, cris, ocup, base); 
        }

        // Caso tenha algum robo no tile, salva a posicao
        // para que no futuro ele seja iniciado com as
        // posicoes corretas
        if (ocup > -1) {
            positions[k] = i;
            k++;
            positions[k] = j;
            k++;
        }

        // Parte grafica: para cada tile, mandamos a arena recolorir
        // o tile com a cor específica de seu terreno (no futuro, cada
        // tipo de terreno terá uma textura própria)
        switch(terr){
            case GRAMA:
                fprintf(display, "terr %d %d 0\n", i,j);
                break;
            case ESTRADA:
                fprintf(display, "terr %d %d 1\n", i,j);
                break;
            case MONTANHA:
                fprintf(display, "terr %d %d 2\n", i,j);
                break;
            case AGUA:
                fprintf(display, "terr %d %d 3\n", i,j);
                break;
            case AREIA:
                fprintf(display, "terr %d %d 4\n", i,j);
                break;
        }

        // Parte gráfica: mandamos a arena recolorir os tiles que
        // representam as bases com a cor branca
        if (base == 1){
            base = 0;
            cris = 0;
            fprintf(display, "terr %d %d %d\n", i, j, ntime);
            ntime++;
        }

        // Parte grafica: se houver cristais no tile, mandamos a 
        // arena imprimir um pequeno cristal no tile correspondente
        if (cris >= 1 && cris < 3 && base == 0)
            fprintf(display, "cris %d %d\n", i, j);

        // se houver 3 ou mais cristais, imprimimos a imagem de um
        // pequeno lote de cristais
        else if (cris >= 3) 
            fprintf(display, "lotecris %d %d\n", i, j);

        fflush(display);
        
        j++;
        if(j==12){
            j=0;
            i++;
        }
    }
}

/*
 * Insere todos os robos na arena
 *
 * Cria, para cada um dos robos um programa correspondente
 * usando a funcao readProg e instancia-os na arena usando
 * cria_maquina de maq.c
 * 
 * Manda tambem os comandos necessarios para que esses
 * robos sejam criados no "apres"
 *
 */
void insereExercito(){
    int k = 0;
    int x = 0;

    if (display == NULL) {
        fprintf(stderr,"Não encontrei o programa de exibição\n");
        exit(1);
    }

    //Cria cada robo, individualmente, com programas distintos
    ids = 0;
    for(int i = 0; i < MAX_ROBOS*MAX_TIME; i++){
        int j = k+1;
        if (i == 5) x = 1;

        FILE* p = stdin;
        p = fopen(robs[i],"r");    
        int instrSize = compilador(p, prog, ids);

        arena.robots[i] = cria_maquina(prog, instrSize, ids, positions[k], positions[j], 100, x);

        // Para cada robo, associa-o com o sprite de seu time
        // e o desenha em sua posicao inicial
        if (x == 0) fprintf(display, "rob A.png\n");
        else fprintf(display, "rob B.png\n");
        fprintf(display, "%d 99 99 %d %d\n", ids, positions[k], positions[j]);
        printf("Robô %d posicionado\n", ids);
        ids++;
        k+=2;
        fflush(display);
    }
}

/*
 * Escalonador
 *
 * Da, alternadamente, a chance de cada robo
 * executar funcoes e fazer calculos por um
 * 'tempo' igual a "MAX_INSTR"
 * Agora também checa e decrementa o contador
 * de ocupacao do robo "cont"
 */
void Atualiza(){

    for(int j=0; j<MAX_TURNOS; j++){

        for(int i=0; i<ids; i++){
            // Se o robo nao estiver ocupado, ele pode executar seu
            // programa normalmente
            if (arena.robots[i].cont == 0)
                exec_maquina(&arena.robots[i], MAX_INSTR);

            // Senao, decrementa o contador e imprime uma mensagem no log
            // Tambem restaura a vida do robo caso ele possa voltar no 
            // proximo turno
            else {
                arena.robots[i].cont--;
                D(printf("Robô %d não consegue se mexer!\n", i));
                if (arena.robots[i].vida <= 0 && arena.robots[i].cont == 0) {
                    arena.robots[i].vida = 100;
                    fprintf(display, "ress %d %d %d\n", i, arena.robots[i].position[0], arena.robots[i].position[1]);
                }
            }

            // Atualiza o estado, vida e numero de cristais de cada robo nos
            // graficos da UI na parte inferior da tela
            int estado = 0;
            if (arena.robots[i].vida <= 0) estado = 1;
            fprintf(display, "UI_atu %d %d %d %d %d %d\n", i, arena.robots[i].vida, arena.robots[i].cristais, estado, 
                arena.mapa.tiles[1][0].cristais, arena.mapa.tiles[18][11].cristais);
            fprintf(display, "atu\n");
            fflush(display);        
        }

        // Condições de parada:
        // No momento, para fins de debug, basta que um robo deposite um cristal
        // na base inimiga para finalizar o jogo
        if (arena.mapa.tiles[1][0].cristais >= 5 || arena.mapa.tiles[18][11].cristais >= 5) {
            if (arena.mapa.tiles[1][0].cristais >= 5){
                printf("Fim de jogo! O time vermelho ganhou, yay!\n");
                fprintf(display, "fim 1\n");
            }
            else{
                printf("Fim de jogo! O time azul ganhou, yay!\n");
                fprintf(display, "fim 2\n");
            }
            fflush(display);
            break;
        }

        // Numero maximo de turnos atingido
        else if (j == MAX_TURNOS-1){
            printf("Fim de jogo (por turnos)\n");
            fprintf(display, "fim 3\n");
            fflush(display);
            break;
        }
        fflush(display);
    }
}

/*
 * Instancia um "Tile"
 *
 * retorna um "Tile" definido por valores passados
 * como argumentos
 */
Tile Inicializa(Terreno terr, int cris, int ocup, int base){
    Tile t;
    t.terreno = terr;
    t.cristais = cris;
    t.ocupado = ocup;
    t.base = base;
    return t;
}

/*
 * Responde as chamadas de sistema realizadas pelos robos
 *
 * Retira informacoes da pilha de dados
 * de um robo identificado pelo argumento
 * Realiza tarefas especificas com base na
 * "solicitacao" feita pelo robo (instrucoes
 * da pilha)
 * 
 * Retorna valores como identificacao de problemas
 * ret == 1, tudo OK
 * ret == -1, algum problema
 *
 * O robo agora armazena as ultimas 4 respostas
 * na posicoes 7, 8, 9 e 10 de sua memoria
 */
int Sistema(int id){
    OPERANDO arg;
    OPERANDO ver;
    OPERANDO instr = desempilha(&arena.robots[id].pil);
    int posx = arena.robots[id].position[0];
    int posy = arena.robots[id].position[1];
    int old_posx = posx;
    int old_posy = posy;

    // Chamadas que nao precisam de direcao:
    // VER: guarda nas 7 primeiras posicoes da memoria 
    // os 6 tiles vizinhos e o proprio tile do robo
    if(instr.valor.ac==VER){
        ver.t = TILE;
        for (int i = 0; i < 7; i++) {
            switch (i) {
            case N:
                posx -= 2;  
                break;
            case NE:
                if(posx % 2 != 0) posy++;
                posx--;
                break;
            case SE:
                if(posx % 2 != 0) posy++;
                posx++;
                break;
            case S:
                posx+=2;
                break;
            case SW:
                if(posx % 2 == 0) posy--;
                posx++;
                break;
            case NW:
                if(posx % 2 == 0) posy--;
                posx--;
                break;
            case CN:
                break;
            }

            // Checa se os tiles estão na arena, retorna -1 caso contrario
            if (posx >= 0 && posx <= 19 && posy >= 0 && posy <= 11) {
                ver.valor.tile = arena.mapa.tiles[posx][posy];
                arena.robots[id].Mem[i] = ver;
            }
            else 
                arena.robots[id].Mem[i] = cria_operando(NUM, -1);

            posx = old_posx;
            posy = old_posy;
        }
        return 1;
    }

    // KMK: robo se explode causando dano a si mesmo e 
    // a qualquer robo nos tiles vizinhos (mesmo que
    // eles sejam do mesmo time)
    else if (instr.valor.ac == KMK){

        // Pega o id de todos os vizinhos
        int vizinhos[6] = {-1,-1,-1,-1,-1,-1};
        for (int i = 0; i < 6; i++){
            switch (i) {
            case N:
                posx -= 2;  
                break;
            case NE:
                if(posx % 2 != 0) posy++;
                posx--;
                break;
            case SE:
                if(posx % 2 != 0) posy++;
                posx++;
                break;
            case S:
                posx+=2;
                break;
            case SW:
                if(posx % 2 == 0) posy--;
                posx++;
                break;
            case NW:
                if(posx % 2 == 0) posy--;
                posx--;
                break;
            }
        
            if (posx >= 0 && posx <= 19 && posy >= 0 && posy <= 11)
                vizinhos[i] = arena.mapa.tiles[posx][posy].ocupado;

            posx = old_posx;
            posy = old_posy;   
        }
        
        // Danifica o proprio robo e detecta se ele ainda esta vivo
        arena.robots[id].vida -=100;
        D(printf("\nRobô %d se explodiu!\n", id));
        D(printf("\nRobô %d agora tem %d de vida restante!\n", id, arena.robots[id].vida));
        fprintf(display, "log %d %d %d %d %d %d %d %d\n", id, -1,-1,-1,-1,-1, 6, -1);
        // fprintf(display, "sfx 0\n");

        // Incrementa o contador do robo em 5 e atualiza seu sprite
        // na arena para refletir sua condicao ("morte")
        if (arena.robots[id].vida <= 0){
                    arena.robots[id].cont = 5;
                    D(printf("\nOh, não! Robô %d morreu!\n", id));
                    fprintf(display, "morte %d %d %d\n", id, posx, posy);
                    fprintf(display, "log %d %d %d %d %d %d %d %d\n", id, -1,-1,-1,-1,-1, 16, -1);
        }

        // Danifica os demais robos e detecta se eles ainda estao vivos
        int damaged[] = {-1, -1, -1, -1, -1, -1};
        int j = 0;
        for (int i = 0; i < 6; i++){
            if (vizinhos[i] != -1){
                int eid = vizinhos[i];
                if (arena.robots[eid].vida > 0){
                    arena.robots[eid].vida -=100;
                    D(printf("\nRobô %d foi danificado na explosão!\n", eid));
                    D(printf("\nRobô %d agora tem %d de vida restante!\n", eid, arena.robots[eid].vida));
                    damaged[j++] = eid;

                    // Incrementa o contador do robo em 5 e atualiza seu sprite
                    // na arena para refletir sua condicao ("morte")
                    if (arena.robots[eid].vida <= 0){
                        arena.robots[eid].cont = 5;
                        D(printf("\nOh, não! Robô %d morreu!\n", eid));
                        fprintf(display, "morte %d %d %d\n", eid, arena.robots[eid].position[0], arena.robots[eid].position[1]);
                    }
                }
            }
        }

        if (j == 1)
            fprintf(display, "log %d %d %d %d %d %d %d %d\n", damaged[0], damaged[1], damaged[2], damaged[3], 
                                                         damaged[4], damaged[5], 7, -1);
        else if (j == 2)
            fprintf(display, "log %d %d %d %d %d %d %d %d\n", damaged[0], damaged[1], damaged[2], damaged[3], 
                                                         damaged[4], damaged[5], 8, -1);            
        else if (j == 3)
            fprintf(display, "log %d %d %d %d %d %d %d %d\n", damaged[0], damaged[1], damaged[2], damaged[3], 
                                                         damaged[4], damaged[5], 9, -1);            
        else if (j == 4)
            fprintf(display, "log %d %d %d %d %d %d %d %d\n", damaged[0], damaged[1], damaged[2], damaged[3], 
                                                         damaged[4], damaged[5], 10, 11);            
        else if (j == 5)
            fprintf(display, "log %d %d %d %d %d %d %d %d\n", damaged[0], damaged[1], damaged[2], damaged[3], 
                                                         damaged[4], damaged[5], 12, 13);        
        else
            fprintf(display, "log %d %d %d %d %d %d %d %d\n", damaged[0], damaged[1], damaged[2], damaged[3], 
                                                         damaged[4], damaged[5], 14, 15);
    }

    // Demais chamadas: primeiro define o vizinho sobre o qual
    // a acao sera executada
    else{
        arg = desempilha(&arena.robots[id].pil);
        switch(arg.valor.v){
        case N:
            posx -= 2;  
            break;
        case NE:
            if(posx % 2 != 0) posy++;
            posx--;
            break;
        case SE:
            if(posx % 2 != 0) posy++;
            posx++;
            break;
        case S:
            posx+=2;
            break;
        case SW:
            if(posx % 2 == 0) posy--;
            posx++;
            break;
        case NW:
            if(posx % 2 == 0) posy--;
            posx--;
            break;
        case CN:
            break;
        }

        // Retorna -1 se estiver fora da arena
        if(posx < 0 || posy > 11 || posx > 19 || posy < 0) return -1;
            
        switch(instr.valor.ac){
            // Move o robo caso o tile nao esteja ocupado
            case MOV:
                if(arena.mapa.tiles[posx][posy].ocupado == -1 && arena.mapa.tiles[posx][posy].base == 0){
                    arena.robots[id].position[0] = posx;
                    arena.robots[id].position[1] = posy;
                    arena.mapa.tiles[posx][posy].ocupado = id;
                    arena.mapa.tiles[old_posx][old_posy].ocupado = -1;
                    printf("\nRobô %d se moveu para %s (posição %d, %d)\n", id, Direcao[arg.valor.v], posx, posy);

                    // Atualiza a posicao do robo na tela e redesenha
                    // possiveis cristais que tenham sido apagados
                    fprintf(display, "%d %d %d %d %d\n", id, old_posx, old_posy, posx, posy);
                    if (arena.mapa.tiles[old_posx][old_posy].cristais >= 1){
                        fprintf(display, "cris %d %d\n", old_posx, old_posy);
                    }

                    fprintf(display, "log %d %d %d %d %d %d %d %d\n", id, -1,-1,-1,-1,-1, 0, -1);

                    // Atualiza o contador de ocupacao do robo de acordo
                    // com o custo de cada terreno
                    Terreno terr = arena.mapa.tiles[posx][posy].terreno;

                    switch(terr){
                        case GRAMA:
                            arena.robots[id].cont +=1;
                            D(printf("Robô %d pisou na GRAMA: +1\n", id));
                            break;
                        case ESTRADA:
                            D(printf("Robô %d pisou na ESTRADA: +0\n", id));
                            break;
                        case MONTANHA:
                            arena.robots[id].cont +=4;
                            D(printf("Robô %d pisou na MONTANHA: +4\n", id));
                            break;
                        case AGUA:
                            arena.robots[id].cont +=3;
                            D(printf("Robô %d pisou na AGUA: +3\n", id));
                            break;
                        case AREIA:
                            arena.robots[id].cont +=2;
                            D(printf("Robô %d pisou na AREIA: +2\n", id));
                            break;
                    }

                    return 1;
                }
                else return -1;

            // Recolhe um cristal se o tile o tiver
            case REC:
                if(arena.mapa.tiles[posx][posy].cristais > 0){
                    arena.mapa.tiles[posx][posy].cristais--;
                    arena.robots[id].cristais++;
                    D(printf("\nRobô %d recolheu um cristal no %s (posição %d, %d)\n", id, Direcao[arg.valor.v], posx, posy));
                    fprintf(display, "log %d %d %d %d %d %d %d %d\n", id, -1,-1,-1,-1,-1, 2, -1);
                    return 1;
                }
                else return -1;

            // Deposita um cristal se o robo o tiver
            case DEP:
                if(arena.robots[id].cristais > 0){
                    arena.mapa.tiles[posx][posy].cristais++;
                    arena.robots[id].cristais--;
                    D(printf("\nRobô %d depositou um cristal no %s (posição %d, %d)\n", id, Direcao[arg.valor.v], posx, posy));
                    fprintf(display, "log %d %d %d %d %d %d %d %d\n", id, -1,-1,-1,-1,-1, 3, -1);
                    return 1;
                }
                else return -1;
            // Ataca um tile vizinho, pode conter um robo ou nao
            case ATQ:
                if(arena.mapa.tiles[posx][posy].ocupado > -1){
                    int eid = arena.mapa.tiles[posx][posy].ocupado;
                    if (arena.robots[eid].vida > 0) {
                        arena.robots[eid].vida -= 50;
                        D(printf("\nRobô %d atacou o robô %d\n", id, eid));
                        D(printf("\nRobô %d agora tem %d de vida restante!\n", eid, arena.robots[eid].vida));
                        fprintf(display, "log %d %d %d %d %d %d %d %d\n", id, eid,-1,-1,-1,-1, 1, -1);
                        // fprintf(display, "anim 0 %d %d %d %d %d\n", id, old_posx, old_posy, posx, posy);
                        // fprintf(display, "sfx 2\n");

                        if (arena.robots[eid].vida <= 0){
                            arena.robots[eid].cont = 5;
                            D(printf("\nOh, não! Robô %d morreu!\n", eid));
                            fprintf(display, "morte %d %d %d\n", eid, posx, posy);
                            fprintf(display, "log %d %d %d %d %d %d %d %d\n", eid, -1, -1, -1, -1, -1, 16, -1);                            
                        }

                        return 1;
                    }
                    else return -1;
                }
                else return -1;
            // Joga um cristal no tile vizinho, so funciona se tiver
            // um robo no tile: neste caso, o robo atingido fica incapacitado
            // por 2 turnos e derruba todos os seus cristais
            case JGC:
                if(arena.mapa.tiles[posx][posy].ocupado > -1 && arena.robots[id].cristais > 0){
                    int eid = arena.mapa.tiles[posx][posy].ocupado;
                    arena.robots[eid].cont +=2;
                    arena.robots[id].cristais--;
                    D(printf("\nRobô %d lançou um cristal no robô %d!\n", id, eid));
                    D(printf("\nRobô %d agora está incapacitado por 2 turnos!\n", eid));
                    if (arena.robots[eid].cristais > 0){
                        arena.mapa.tiles[posx][posy].cristais += arena.robots[eid].cristais;
                        arena.robots[eid].cristais = 0; 
                        D(printf("\nRobô %d derrubou todos os seus cristais!\n", eid));
                    }
                    fprintf(display, "log %d %d %d %d %d %d %d %d\n", id, eid,-1,-1,-1,-1, 4, 5);
                    // fprintf(display, "sfx 1\n");
                    return 1;
                }
                else return -1;

            default: ;
        }
    }
    return -1;
}

