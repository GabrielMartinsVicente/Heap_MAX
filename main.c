//Gabriel Martins Vicente
/* Sua tarefa será construir um heap (binário, max-heap), e criar métodos para criar a árvore e a 
partir de um array, inserir, excluir, e buscar valores na árvore. Seu objetivo é explicar o funcionamento 
dos  métodos  que  você  irá  criar.  Para  isso,  deve  buscar  em  sites  acadêmicos,  exemplos  de 
implementação de árvores heap. 
Para  testar,  você deve usar  um  array  com,  no  mínimo, 500  itens,  gerados  randomicamente  e 
criar métodos para testar se a estrutura criada obedece a regra de criação que você determinou. */

/* Sites de Referência: https://joaoarthurbm.github.io/eda/posts/heap/
                        https://www.cos.ufrj.br/~rfarias/cos121/aula_09.html
                        https://www.ime.usp.br/~pf/analise_de_algoritmos/aulas/heap.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* 2 Propriedades para a Estrutura de dado HEAP MÁXIMO:

1) O valor de um nó é maior ou igual ao valor de seus filhos;

2) O Heap é uma árvore binária completa ou quase-completa 
da esquerda para a direita.


*/

void vetor_aleatorio(int tamanho, int vetor[]){ /* Função tem o objetivo de inserir valores aleatórios 
                                                    no vetor */

  for(int i = 0; i<tamanho; i++){
    vetor[i] = 1 + (rand() % 1000); //Valor aleatório de 1 à 1000
  }
  
}

struct heap{  //Estrutura de dado HEAP MÁXIMO

  int *p;      //Ponteiro onde vai apontar para o endereco de memória do vetor/árvore
  int tamanho; //Tamanho do vetor (capacidade de memória)
  int tail;    //Indica o indice no *p do último valor armazenado caso tenha

};

struct heap* construtor_heap(int tamanho){

  struct heap *heap = (struct heap*)malloc(sizeof(struct heap)); //Cria na memória uma estrutura heap
  heap->tamanho = tamanho;  //Inseri o valor da capacidade
  heap->tail = -1;          /* Inicia com valor -1 pelo mesmo motivo da estrutura de pilha. Quando for 
                                inserido o 1° valor, tail obtem o valor 0, identificando que o último e
                                somente valor esta na posição 0 do vetor */
  heap->p = (int*)malloc(sizeof(int)*tamanho); //Cria na memória o vetor com os valores int e *p recebe o endereco de memória
  return heap;    //Retorna o endereço de memória do heap criado
    
}

int pai(int index) { //Função retorna o indice da posição do nó pai
  
  return (index-1)/2;
  //Obs: Como a função retorna um valor int, não é preciso preocupar com valores decimais (após a vírgula)
}

int esquerda(int index) {  //Função retorna o indice da posição do nó filho esquerdo
  
  return 2 * index + 1;
  //Obs: Como a função retorna um valor int, não é preciso preocupar com valores decimais (após a vírgula)
}

int direita(int index) { //Função retorna o indice da posição do nó filho direito
  
    return 2 * (index + 1);
  //Obs: Como a função retorna um valor int, não é preciso preocupar com valores decimais (após a vírgula)
}

bool estaVazio(struct heap *heap){ //Verifica se meu heap esta vazio

  if(heap->tail == -1){
    return true;
  }
    
  else{ 
    return false;
  }
    
}

bool estaValidoIndice(struct heap *heap,int indice){ //Verificando se o indice é válido

  if(indice >= 0 && indice <= heap->tamanho){
    //Verificando se o indice esta no limite estipulado pelo heap: 0 <= indice <= tamanho (capacidade)

    return true;
  }

  else{

    return false;
  }
  
}

bool isFolha(struct heap *heap,int indice){ //Verificando se é folha

  if(indice > pai(heap->tail) && indice <= heap->tail){
   /* Para saber se é uma folha, necessariamente deve ser maior que o último nó pai e 
      estar dentro do limite de capacidade :

      último indice nó pai <= indice <= tamanho (capacidade)

        */
      
    return true;
  }

  else{
  
    return false;
  }
  
}

int max_indice(struct heap *heap,int pai,int direita,int esquerda){// Verificar entre os indices qual tem o valor maior

  /* Obs: Por conta da Propriedade 2) da estrutura Heap Máximo e também por conta de uma verificação no início na 
    função heapfy, não existe necessidade de verificar o valor nó a esquerda */

  if(heap->p[pai] > heap->p[esquerda]){
  // Se o valor: nó_pai > que o nó_esquerda
    
    if( estaValidoIndice(heap,direita) == true ){
      // Se o valor a direita for válido
      
      if(heap->p[pai] < heap->p[direita]){
        // Se o valor: nó_pai < nó_direita, retorna indice_direita
        
        return direita;
      }
    }

    return pai; 
    //Se nó_pai > nó_esquerda e nó_pai > nó_direita, retorna indice_pai 
 
  } else{

    if( estaValidoIndice(heap,direita) == true ){
      // Se o valor a direita for válido
      
      if( heap->p[esquerda] < heap->p[direita]){
        // Se o valor nó_esquerda < nó_direita
        
        return direita;
        // Retorna indice_direita 
        
      }
      
    }

    return esquerda;
    //Se nó_esquerda > nó_direita e nó_esquerda > nó_pai
    //retorna nó_esquerda 
    
  }
  
}

void heapfy(struct heap *heap,int indice){ 
  /* Função tem o objetivo de comparar o nó_pai, nó_direita e nó_esquerda e fazer o MAIOR valor entre os 3 assumir
    o nó_pai, isto é tornando a estrutura um HEAP MÁXIMO*/

  if (isFolha(heap,indice) || !estaValidoIndice(heap,indice)){
    return;
    //Se a posicção do indice for uma folha a aplicação do heapfy é inútil, porque ja atende as Propriedades do heap
    //Se o valor for inválido não faz sentido aplicar o heapfy
  }
    
  int indece_max = max_indice(heap,indice,esquerda(indice),direita(indice));
  // Variável indece_max recebe o indice do valor maior entre os 3 

  if(indece_max != indice){
    //indece_max != do nó_pai, ou seja, necessário ter que fazer a troca dos valores
    
    int aux = heap->p[indice];              //Variável auxiliar para fazer a troca
    heap->p[indice] = heap->p[indece_max];
    heap->p[indece_max] = aux;

    heapfy(heap,indece_max);
    /* A recursividade da função verifica se a troca dos 2 nós influência na necessidade de ter mais trocas subsequentes,
        portanto, no valor do antigo nó_pai é verificado se nessa nova posição o valor é maior que seus filhos */

  }

}

struct heap* insercao(struct heap *heap,int valor){

  /* Observações importantes com relação a Inserção de um Heap Máximo:

       A adição de um novo elemento sempre é feita na próxima 
      posição livre do array, isto é, tail + 1. Essa estratégia
      garante que o Heap sempre será completo ou quase completo
      da esquerda para a direita.

  */

  if (heap->tail >= (heap->tamanho)){
    printf("\nO Heap esta cheio\n");
    return heap;
    //Verificando se o heap esta cheio
  }

  heap->tail += 1;
  heap->p[heap->tail] = valor;

  int i = heap->tail;
  while (i > 0 && heap->p[pai(i)] < heap->p[i]) {

    /* While funciona como um pseudo heapfy, basicamente vai fazer a troca do novo valor, se o mesmo
      for maior que o seu nó_pai */
    int aux = heap->p[i];
    heap->p[i] = heap->p[pai(i)];
    heap->p[pai(i)] = aux;
    i = pai(i);

  }

  return heap;
}

struct heap* remover(struct heap *heap){

  /* Observações importantes com relação a Remoção de um Heap Máximo:

      A remoção em um heap é sempre feita na raiz.

      E para manter a propriedade de ser completo ou quase-completo da esquerda para a direita,
      trocamos o valor da raiz com a última folha e removemos essa última folha. */
  
  if (estaVazio(heap) == true){
    printf("\nO heap está vazio\n");
    return heap;
    //Verificando se o heap esta vazio
  } 
    
  else{
        
  heap->p[0] = heap->p[heap->tail]; //Raiz recebe o valor da última folha
  heap->tail -= 1;

  heapfy(heap,0);
  /* A verificação dos valores começa pela raiz para manter as Propriedades da estrutura HEAP MÁXIMO,
    por conta da recursividade da função heapfy é garantido que a estrutura vai se tornar um Heap máximo pelas
    aplicações sucessivas do algoritmo heapify */
    
     } 

  return heap;
  
  }

bool busca(struct heap *heap, int valor){//Verificar se o valor esta na estrutura Heap Máximo
  
  for(int i = 0; i<= heap->tail; i++){//Interação por todo o heap

    if(heap->p[i] == valor){ 

      printf("\nValor se encontra no heap com o indice = %d\n",i);
      return true;
      //Se encontrar o valor return true
    }
  }

  return false; 
  //Se não encontrar o valor return false
}

bool validacao(struct heap *heap){ //Verifica se a Estrutura condiz com as Propriedades do Heap-Máximo

  for(int i = 0; i <= heap->tail;i++){

    if(heap->p[i] == 0){ //Verifica a Propriedade 2)
      return false;
      /* Se dentro dos intervalos de valores dentro do heap existir um valor nulo (no caso o valor 0),
          então a estrutura não esta sendo completada da esquerda para a direita, não condiz com a Propriedade 2) */
    }
    
  }

  for(int i = heap->tail; i>= 0; i--){ //Verifica a Propriedade 1)

    /* Observação: A interação dos valores começa pelos últimos valores, no caso as folhas,
       é interessante observar que não vai ser necessário fazer a interação da raiz, pois pelos filhos ja é feito
       a verificação */

    if(heap->p[pai(i)] < heap->p[i]){
      return false;
      //Se nó_filho > nó_pai não condiz com a propriedade 1)
    }
    
  }

  return true;
  //Se as 2 Propriedades estiverem ok, a estrutura esta coerente
  
}

int main() {

  srand(time(NULL));
  
  int capacidade = 500 + (rand() % 501);  //Valor aleatório de 500 à 1000
  struct heap *heap;
  int vetor[capacidade];

  vetor_aleatorio(capacidade, vetor); //Vetor aleatório construído
  heap = construtor_heap(capacidade); //Construção do heap

  
  for(int i = 0; i<capacidade; i++){ //Inserção de todos os valores
    
    insercao(heap,vetor[i]);
  }
  
  if(validacao(heap)){printf("\n\nHEAP está OK\n");} //Saída de validação
  else{printf("\n\nHEAP NÃO ESTA OK\n");}

  
  return 0;
}