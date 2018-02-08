# Batalha de Robôs

## TecProg-2017

## Projeto para a matéria MAC216 - 2017/2

Seja bem vindo à Batalha de Robôs!

O objetivo é simples: o primeiro time que depositar 5 cristais na base inimiga é o vencedor!

Para isso, você deve programar seus robôs para que encontrem a maneira mais eficiente de se locomover através do terreno, de atacar inimigos, pegar cristais e é claro, destruir bases.

## Linguagem 
A linguagem que os robôs utilizam é uma mistura de C e Python: a sintaxe é quase idêntica ao C, mas as variáveis não possuem tipos, como em Python.

Para alterar os programas dos robôs, basta editar os arquivos rob0, rob1, etc - cada arquivo desses corresponde ao programa de um dos 10 robôs.   
Os robôs do time azul são: rob0, rob1, rob2, rob3, rob4  
Os robôs do time vermelho são: rob5, rob6, rob7, rob8, rob9

### Peças básicas
> __Atribuições:__  
__Comum:__ x = 5;  
__Atributos:__ x = tile.cristais;
  
> __Controle de fluxo:__  
__Condicionais:__ if(x < 5) { *code* }    
		 else { *more code* }  
__Laços:__ while (True) { *code* }  
	  for (i = 1; i < 5; i*=2) { *code* }   
    
> __Chamadas de sistema__  
__VER:__ Atualiza os dados que o robô tem dos seus arredores - ver();  
__MOVE:__ Move o robô na direção indicada - move(NW);  
__ATACA:__ Ataca a direção indicada - ataca(NW);  
__RECOLHE:__ Recolhe cristal na direção indicada - recolhe(NW);  
__DEPOSITA:__ Deposita cristal na direção indicada - deposita(NW);  
__JOGA_CRISTAL:__ Joga um cristal na direção indicada, fazendo com o que o robô atingido fique atordoado e derrube seus cristais- joga_cristal(NW);  
__KAMIKAZE:__ Robô se explode e atordoa todos os robôs ao seu redor - kamikaze();   
__Resposta:__ Não é uma chamada de sistema, mas retorna o resultado da última chamada realizada (1 = sucesso, -1 = fracasso)
  
> __Funções:__  
__Declaração:__ func f(x, y) { return x+y } ou def g(x) { return; }  
__Chamadas:__ w = f(x, y) ou g(w);  
__Variáveis locais:__ Como em C, as variáveis declaradas dentro do seu programa possuem um escopo limitado: variáveis declaradas fora de funções são consideradas globais e acessíveis de qualquer lugar. Se você quer declarar uma nova variável, dentro de uma função, com um nome igual ao de uma variável global preexistente, basta usar a keyword *new*: 
  
```    
