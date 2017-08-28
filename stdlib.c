/*
 * File: stdlib.c
 *
 * Descrição: 
 *     stdlib. Parte da lib C da API 32bit.
 * Versão: 1.0, 2016 - Created.
 */

 
#include "types.h" 
#include "stddef.h"
#include "mm.h"
#include "heap.h"
#include "stdlib.h"

 
//#define NULL ((void*)0)

static int randseed = 1234;


//
// -----------------
// Começo do Heap support.
// Na verdade é o gerenciamento de meória necessário para stdlib.c
//
//

//Variáveis internas. 

//int mmStatus;
unsigned long last_valid;         //Último heap pointer válido. 
unsigned long last_size;          //Último tamanho alocado.
unsigned long mm_prev_pointer;    //Endereço da úntima estrutura alocada.


//
// Funções locais.
//
int stdlibInitMM();

/*
 * stdlib_strncmp:
 *     Compara duas strings.
 *     Obs: Função de uso interno.
 */
int stdlib_strncmp(char *s1, char *s2, int len);

/*
unsigned long heap_set_new_handler( unsigned long address );
unsigned long heap_set_new_handler( unsigned long address )
{
    unsigned long Old;

    Old = kernel_heap_start;
    
    kernel_heap_start = address;

    return (Old);
};
*/


/*
 * heapSetLibcHeap:
 *    Configura o heap usado pela libc em user mode.
 *    Reconfiguração total do heap.
 *    @todo: Salvar em estrutura.
 */
void heapSetLibcHeap(unsigned long HeapStart, unsigned long HeapSize)
{
	struct heap_d *h;    //Heap struct.    
	
	
	//
	// Check limits.
	//
	
	if(HeapStart == 0){
		return;
	}
	
	if(HeapSize == 0){
		return;
	}	
	
	//
	// start, end, pointer, available.    
	//
	
	heap_start = (unsigned long) HeapStart;                 
	heap_end   = (unsigned long) (HeapStart + HeapSize);
	g_heap_pointer    = (unsigned long) heap_start;            
	g_available_heap  = (unsigned long) (heap_end - heap_start); 
	
	//A estrutura fica no início do heap.??!!
	h = (void*) heap_start;
	
	//Configurando a estrutura.
	h->HeapStart     = (unsigned long) heap_start;             
	h->HeapEnd       = (unsigned long) heap_end;
	h->HeapPointer   = (unsigned long) g_heap_pointer;            
	h->AvailableHeap = (unsigned long) g_available_heap; 	
	
	
	//
	// Configura o ponteiro global em heap.h.
	//
	
	Heap = (void*) h;
	
	//
	// Lista de heaps.
	//
	
	//Configuração inicial da lista de heaps. Só temos 'um' ainda.
	heapList[0] = (unsigned long) Heap; //Configura o heap do kernel.
	heapList[1] = (unsigned long) 0;
	heapList[2] = (unsigned long) 0;
	//...
	
	//Contagem? ainda em zero.?!
	
done:	
	return;
};


/*
 * AllocateHeap:
 *     Aloca memória no heap do kernel.
 *
 * *IMPORTANTE: Aloca BLOCOS de memória dentro do heap do processo Kernel.
 *
 * Obs: A estrutura usada aqui é salva onde ??
 *
 * @todo: 
 *     Ao fim dessa rotina, os valores da estrutura devem ser armazenas no 
 * header, lá onde foi alocado espaço para o header, assim tem-se informações 
 * sobre o header alocado.
 *  A estrutura header do heap, é uma estrutura e deve ficar antes da 
 *  área desejada. partes={header,client,footer}.
 *
 * 2015 - Created.
 * sep 2016 - Revision.
 * ...
 */
unsigned long AllocateHeap(unsigned long size)
{
	struct mmblock_d *Current;	
	//struct mmblock_d *Prev;		
	
	//
	// Se não há heap disponível, não há muito o que fazer.
	//
	
	// Available heap.
	if(g_available_heap == 0)
	{
	    //
		// @todo: Tentar crescer o heap para atender o size requisitado.
		//
		
		//try_grow_heap() ...
		
		//
		// @todo: Aqui poderia parar o sistema e mostrar essa mensagem.
		//
		
	    printf("AllocateHeap fail: g_available_heap={0}\n");
        refresh_screen();		
        return (unsigned long) 0;
		//while(1){};
	};
	
	//
    // Size limits. (Min, max).
    //
 	
	//Se o tamanho desejado é igual a zero.
    if(size == 0){
	    printf("AllocateHeap error: size={0}\n");
		refresh_screen();
		return (unsigned long) g_heap_pointer;
	};
	
	//Se o tamanho desejado é maior ou igual ao espaço disponível.
	if(size >= g_available_heap)
	{
	    //
		// @todo: Tentar crescer o heap para atender o size requisitado.
		//

		//try_grow_heap() ...

		printf("AllocateHeap error: size >= g_available_heap\n");
		refresh_screen();
		return (unsigned long) 0;
	};
    
	//Salvando o tamanho desejado.
	last_size = (unsigned long) size;
	
	//
	// Contador de blocos.
	//
	
try_again:	

    mmblockCount++;  
    
	if(mmblockCount >= MMBLOCK_COUNT_MAX){
        printf("AllocateHeap Error: mmblockCount limits!");
		refresh_screen();
		while(1){};
    };
	
    //Identificadores.	
	
	//
	// O Header do header do bloco é o inicio da estrutura que o define. (hã???)
	//

	//
	// Pointer Limits. 
	// (Não vamos querer um heap pointer fora dos limites do heap)
	//
	
	//
	// Se o 'g_heap_pointer' atual esta fora dos limites do heap, então 
	// devemos usar o último válido que provavelmente está nos limites.
	//
	
	if( g_heap_pointer < HEAP_START || g_heap_pointer >= HEAP_END )
	{
	    //Checa os limites do último last heap pointer válido.
	    if( last_valid < HEAP_START || last_valid >= HEAP_END ){
            printf("AllocateHeap Error: last valid heap pointer limits");
		    refresh_screen();
		    while(1){}		
	    };
		//Havendo um last heap pointer válido.
		//?? isso não faz sentido.
		g_heap_pointer = (unsigned long) last_valid + last_size;
		goto try_again;
	};
	

    //
	// Agora temos um 'g_heap_pointer' válido, salvaremos ele.
	// 'last_valid' NÃO é global. Fica nesse arquivo.
	//
	
	last_valid = (unsigned long) g_heap_pointer;
	
	//
	// Criando um bloco.
	//
	
	//
	// Estrutura mmblock_d interna.
	// Configurando a estrutura para o bloco atual.
	//
	
    //	
	// Obs: A estutura deverá ficar lá no espaço reservado para o header. 
	// (antes da area alocada).
	// Current = (void*) g_heap_pointer;
    //		
	
	//O endereço do ponteiro da estrutura será o pointer do heap.
	Current = (void*) g_heap_pointer;    
	
	if( (void*) Current != NULL )
	{
	    Current->Header = (unsigned long) g_heap_pointer;  //Endereço onde começa o header.
	    Current->headerSize = MMBLOCK_HEADER_SIZE;         //Tamanho do header. TAMANHO DA STRUCT.  
        Current->Id = mmblockCount;                        //Id do mmblock.
	    Current->Used = 1;                //Flag, 'sendo Usado' ou 'livre'.
	    Current->Magic = 1234;            //Magic number. Ver se não está corrompido.
        Current->Free = 0;                //not free.
	    //Continua...		
	
	    //
	    // Mensuradores. (tamanhos) (@todo:)
	    //
	
	    //
	    // @todo:
	    // Tamanho da área reservada para o cliente.
	    // userareaSize = (request size + unused bytes)
	    // Zera unused bytes, já que não foi calculado.
	    //	

        //
	    // User Area base:
	    // Onde começa a área solicitada. Isso fica logo depois do header.
	    //
	
	    Current->userArea = (unsigned long) Current->Header + Current->headerSize;    

	    //
	    // Footer:
        // O footer começa no 'endereço do início da área de cliente' + 'o tamanho dela'.
	    // O footer é o fim dessa alocação e início da próxima.
	    //
	
	    Current->Footer = (unsigned long) Current->userArea + size;
	
	    //
	    // Heap pointer. 
	    //     Atualiza o endereço onde vai ser a próxima alocação.
	    //
	
	    //if ( Current->Footer < HEAP_START){
	    //    Current->Used = 0;                //Flag, 'sendo Usado' ou 'livre'.
	    //    Current->Magic = 0;            //Magic number. Ver se não está corrompido.	    
	    //	goto try_again;	
	    //}
	
        
		//
		// Obs: O limite da contagem de blocos foi checado acima.
		//
		
	    //
	    // Coloca o ponteiro na lista de blocos.
	    //
	
	    mmblockList[mmblockCount] = (unsigned long) Current;
	
	    //
		// Salva o ponteiro do bloco usado como 'prévio'.
	    // Obs: 'mm_prev_pointer' não é global, fica nesse arquivo.
	    //
		
		mm_prev_pointer  = (unsigned long) g_heap_pointer; 
	
	    //
		// *IMPORTANTE.
	    // Atualiza o ponteiro. Deve ser onde termina o último bloco 
		// configurado.
	    //
		
		g_heap_pointer = (unsigned long) Current->Footer;	

	
	    //
	    // Available heap:
	    // Calcula o valor de heap disponível para as próximas alocações.
	    //
	
	    g_available_heap = (unsigned long) g_available_heap - (Current->Footer - Current->Header);		
	
	    //
	    // Retorna o ponteiro para o início da área alocada.
		// Obs: Esse é o valor que será usado pela função malloc.
		//
		
		return (unsigned long) Current->userArea;
		//Nothing.
		
	}else{
		
	    //Se o ponteiro da estrutura de mmblock for inválido.
		
		printf("AllocateHeap fail: struct.\n");
		//@todo: Deveria retornar.
		//goto fail;
		return (unsigned long) 0;
	};
	

	//
	// @todo: 
	// Checar novamente aqui o heap disponível. Se esgotou, tentar crescer.
	//
	

    //
	 //IMPORTANTE
	 //
	 // @todo:
	 // Colocar o conteúdo da estrutura no lugar alocado para o header.
	 //O header conterá informações sobre o heap.
	//
	//
	
	
	
	// errado #bugbug.
	//Prev = (void*) mm_prev_pointer;
	
	//if( (void*) Prev != NULL)
	//{
	 //   if( Prev->Used == 1 && 
	//	    Prev->Magic == 1234 && 
	//		Prev->Free == 0)
	//	{
	//	    Current->Prev = (void*) Prev;
    //        Prev->Next = (void*) Current;			
	//	};		
	//};
	//
	
	
//
// Fail.
//	
fail:
    //Se falhamos, retorna 0. Que equivalerá à NULL.
    return (unsigned long) 0;	
};


/*
 * AllocateHeapEx:
 *     Aloca heap.
 *     Obs: Onde ??
 */
void *AllocateHeapEx(unsigned long size){
	return (void *) AllocateHeap(size); 
};


/*
 * FreeHeap:
 * @todo: Implementar essa função.
 * Objetivo: Liberar o bloco de memória, configurando a sua estrutura.
 * Libera memória.
 * O argumento é a diferença entra o ponteiro antigo e o novo ponteiro 
 * desejado. 
 * Ponteiros do início da área do cliente.
 * ??
 */
unsigned long FreeHeap(unsigned long size){  		
	return (unsigned long) g_heap_pointer;    //#suspensa! @todo:
};


/*
 * heapInit:
 *     Iniciar a gerência de Heap na libC.
 */
int heapInit()
{
	//Internas.
	int i = 0;

    //Globals.	
	//...@todo:
	
	unsigned long Max = (unsigned long) ((1024*32) - 1);
	//
	//
	HEAP_START = (unsigned long) &HeapBuffer[0];
	HEAP_END   = (unsigned long) &HeapBuffer[Max];
	HEAP_SIZE  = (unsigned long) (HEAP_END - HEAP_START); 
	
	heap_start  = (unsigned long) HEAP_START;  
    heap_end    = (unsigned long) HEAP_END;  
	g_heap_pointer     = (unsigned long) heap_start;    //Heap Pointer.	
    g_available_heap   = (unsigned long) (heap_end - heap_start);    	// Available heap.
	heapCount = 0;      // Contador.
	
	
	//Test. (Cria e inicializa uma estrutura)
	//heapSetLibcHeap(HEAP_START,HEAP_SIZE);
	
	
	//Último heap pointer válido. (*IMPORTANTE)
	last_valid = (unsigned long) g_heap_pointer;
	last_size = 0;
	
	
	//Check Heap Pointer.
	if(g_heap_pointer == 0){
	    printf("init_heap fail: Heap pointer!\n");
		goto fail;
	};
	
	//Check Heap Pointer overflow.
	if(g_heap_pointer > heap_end){
        printf("init_heap fail: Heap Pointer Overflow!\n");
		goto fail;
    };	
	
    //Heap Start.
	if(heap_start == 0){
	    printf("init_heap fail: HeapStart={%x}\n" ,heap_start);
	    goto fail;
	};
	
	//Heap End.
	if(heap_end == 0){
	    printf("init_heap fail: HeapEnd={%x}\n" ,heap_end);
	    goto fail;
	};
	
	//Check available heap.
	if(g_available_heap == 0)
	{
	    //@todo: Tentar crescer o heap.
		
		printf("init_heap fail: Available heap!\n");
		goto fail;
	};
	
	// Heap list ~ Inicializa a lista de heaps.
	while(i < HEAP_COUNT_MAX){
        heapList[i] = (unsigned long) 0;
		++i;
    };
	
	
	//KernelHeap = (void*) x??;
	
	//More?!
	
// Done.
done:
    printf("Done.\n");	
	return (int) 0;
// Fail. Falha ao iniciar o heap do kernel.
fail:
    printf("init_heap: Fail!\n");
	
	/*
	printf("* Debug: %x %x %x %x \n", kernel_heap_start, 
	                                  kernel_heap_end,
									  kernel_stack_start,
									  kernel_stack_end);	
	refresh_screen();	 
    while(1){}		
	*/
    
	return (int) 1;
};


/*
 * stdlibInitMM:
 *   Inicializa o memory manager.
 *   Obs: Essa é uma função local.
 */
int stdlibInitMM()
{	
    int Status = 0;
	int i = 0;	
	
	//
	// @todo: 
	// Inicializar algumas variáveis globais.
	// Chamar os construtores para inicializar o básico.
	//
	
	//
	// @todo: 
	// Clear BSS.
	// Criar mmClearBSS()
	//
	//
	
	//Heap.
	Status = (int) heapInit();
	if(Status != 0){
	    printf("init_mm fail: Heap.\n");
	    return (int) 1;
	};			
	
	//Lista de blocos de memória dentro do heap.
	while(i < MMBLOCK_COUNT_MAX){
        mmblockList[i] = (unsigned long) 0;
		++i;
    };
	
	//Primeiro Bloco.
    //current_mmblock = (void *) NULL;
	
	//
	// Continua...
	//
	
done:	
    return (int) Status;	
}; 


/*
 * libcInitRT:
 *     Inicializa o gerenciamento em user mode de memória virtual
 * para a biblioteca libC99.
 * Obs: *IMPORTANTE: Essa rotina deve ser chamada entes que a biblioteca
 * C seja usada.
 * Obs: Pode haver uma chamada à ela em crt0.s por exemplo.
 *
 */
int libcInitRT()
{
	int Status;
	
	Status = (int) stdlibInitMM();
	if(Status != 0){
		return 1; //error
	}
	
	//...
	
done:	
	return 0;
};


//
// -----------------
// Fim do Heap support.
//


/*
 * rand:
 *     Gera um número randômico.
 */
int rand(void){
	return (int) ( randseed = randseed * 1234 + 5 );
};



/*
 * malloc:
 *     Aloca memória para um programa em user mode.        
 *     
 * Explicação:
 *     O objetivo aqui é alocar memória para uma aplicação em user mode.
 * O Heap usado para isso é o Heap do processo ao qual aplucação pertence
 * ou o Heap do desktop ao qual a aplicação pertence.
 *
 *     Obs: Podemos chamar o kernel para que ele aloque memória em um Heap 
 * específico. 
 *     Obs: Podemos chamar um servidor gerente de memória virtual para
 * ele fazer esse trabalho. (Nada impede que o servidor utilize recursos 
 * do kernel).
 *     Obs: VÁRIOS SERVIÇOS DE ALOCAÇÃO DE MEMÓRIA PODEM CONVIVER TANTO EM
 * USER MODE QUANTO KERNEL MODE.
 *     @todo: O método usado nessa função ainda não foi definido.
 *
 * Histórico da função:
 *     Versão 1.0, 2015 - Created.
 *     Versão 1.0, 2016 - Implementada a função na biblioteca libC99.
 *     ... 
 */
void *malloc(size_t size)
{	
    void *ret;		
	unsigned long s = ( unsigned long) size;
	
	if(s < 0){
		return NULL;
	};
	
	if(s == 0){
	    s = 1;
	};

	//s = (s ? s : 1);	/* if s == 0, s = 1 */
	
	//??? @todo:
	ret = (void *) AllocateHeap(s);
	if( (void*) ret == NULL ){
	    //printf("malloc: falha ao alocar memoria!\n");
		//refresh_screen();
		return NULL;
	};
	
	
	/*
	if((void*) ret < KERNEL_HEAP_START){
	    printf("malloc: falha ao alocar memoria! Limits\n");
		refresh_screen();
		return NULL;		
	};
	*/
	
done:
    return (void*) ret; 
};



/*
 * free:
 *     The free() function frees the memory space pointed 
 * to by ptr, which must have been returned by a previous call 
 * to malloc(), calloc() or realloc(). 
 *
 * Otherwise, or if free(ptr) has already been called before, 
 * undefined behavior occurs.
 * 
 * If ptr is NULL, no operation is performed.
 *
 */
void free(void *ptr){	
   //@todo: Copiar o do kernel base.
   //essa rotina foi melhorada la no kernel base.
	return;  
};


/*
 * calloc: 
 *     Aloca e preenche com zero.
 *
 */
 
/*
void *calloc (size_t count, size_t size)
{
    size_t s = count * size;
    
	void *value = malloc (s);
    
	if (value != 0)
    {    
	    memset (value, 0, s);
	};
  return value;
};

*/



/*
  teste
 *
 */
/* 
void free2(void *ptr)
{
    void *new = (void *) ptr; 
    void *old  = (void *) g_heap_pointer; //head na sequencia de alocação.
	
	unsigned long size;

	
	if( ptr == 0){
		return;
	};
	
	// bugbug
	//if( (void*) ptr == NULL ){
	//	return;
	//};
	//
	
	if( (void *) g_heap_pointer == (void *) ptr )
	{
	    printf("free: Endereco diferente do alocado por malloc!\n");
		refresh_screen();
		while(1){}
		return;
	};
	
	//fail
    if( new == 0 ){
        return; 
    };
	
	//fail
	//o novo pointer tem que ser meno que o antigo.
    if ( new >= old ){ 
        return; 
    };
	
	
	
	//
	// Base do bloco que se deseja liberar.
	//
	
	unsigned long headerBase;
	
	//header base = inicio da area de cliente menos o tamanho do header.
	headerBase = (unsigned long) (new - 64); //headerSize	= 64
	
	//
	// Checando a validade do header.
	 // (checando na estrutura do bloco, que fica no header do bloco, no inicio do bloco). 
	 //
	 struct d_heap *current;	

	current = (void*) headerBase;
	
	
	//
	// *IMPORTANTE:
	//     Aqui current é o inicio do header e o inicio do bloco que se deseja
	//     liberar.
	//     O que devemos fazer é procurar na lista de blocos. heapList[]
	//     por uma entrada que tenha esse valor, ele é o ponteiro para a
	//     estrutura (header) do bloco que se deseja liberar. 
	//
	//     Procurando na lista, encontraremos um id, que deve bater  com o id que temos.
	//
	
	
	if((void*) current == NULL)
	{
		//printf
		return;
	};

	int ID;
	ID = current->Id;
	
	
	int i = 0;
	while(i <= HEAP_COUNT_MAX) 
	{
        if( (void*) current == (void*) heapList[i] )
		{
			if( current->Id == ID  && 
			    current->Used == 1 && 
				current->Magic == 1234 )
            {	
				goto free_block;
			};				
		};
    };

	//fail
	//...
fail:	
    current->Free = 0; //not free yet.
	return;
	
free_block:
    current->Free = 1; //torna ele livre pra ser usado.
	//@todo: memset(...) //zerar a memoria da area de cliente do bloco.
	goto done;
	return;	
	
	//
	// *IMPORTANTE:
	// o g_heap_pointer pode continuar no topo do header, 
	// onde estava antes de chamarem esssa função.
	//
	
done:
    printf("free: Done.");
    refresh_screen();	
	return;
};

*/


/*
 * system:
 *     Interpreta um comando e presta um serviço com base no comando.
 */
int system(const char *command)
{
    //
    // @todo: Checar se comando é válido, se os primeiros caracteres
	//        são espaço. Ou talvez somente compare, sem tratar o argumento.
    //	

	
	//
	//@todo:
	// Criar rotina para pular os caracteres em branco no início do comando.
	//
	
	//@todo: version, ...
	
	//
	//OBS: ESSES SÃO OS COMANDOS DO SISTEMA, USADOS POR TODOS OS PROGRAMAS
	//     QUE INCLUIREM A LIBC. 
	//
	
	//test - Exibe uma string somente para teste.
	if( stdlib_strncmp( (char *) command, "test", 4 ) == 0 ){
	    printf("system: Testing commands ...\n");
		goto exit;
	}; 	
  
	//ls - List files in a folder.
	if( stdlib_strncmp( (char *) command, "ls", 2 ) == 0 ){
	    printf("system: @todo: ls ...\n");
		goto exit;
	}; 
	
	//makeboot - Cria arquivos e diretórios principais.
	if( stdlib_strncmp( (char *) command, "makeboot", 8 ) == 0 )
	{
	    printf("system: @todo: makeboot ...\n");
		
		//ret_value = fs_makeboot();
		//if(ret_value != 0){
		//    printf("shell: makeboot fail!");
		//};
        goto exit;
    };
	
	//format.
	if( stdlib_strncmp( (char *) command, "format", 6 ) == 0 )
	{
	    printf("system: @todo: format ...\n");
		//fs_format(); 
        goto exit;
    };	
	
	//debug.
	if( stdlib_strncmp( (char *) command, "debug", 5 ) == 0 )
	{
	    printf("system: @todo: debug ...\n");
		//debug();
        goto exit;
    };
	
    //dir.
	if( stdlib_strncmp( (char *) command, "dir", 3 ) == 0 )
	{
	    printf("system: @todo: dir ...\n");
		//fs_show_dir(0); 
        goto exit;
    };

	//newfile.
	if( stdlib_strncmp( (char *) command, "newfile", 7 ) == 0 )
	{
	    printf("~newfile - Create empty file.\n");
		//fs_create_file( "novo    txt", 0);
        goto exit;
    };
	
	//newdir.
	if( stdlib_strncmp( (char *) command, "newdir", 7 ) == 0 )
	{
	    printf("~newdir - Create empty folder.\n");
		//fs_create_dir( "novo    dir", 0);
        goto exit;
    };
	
    //mbr - Testa mbr.
    if( stdlib_strncmp( (char *) command, "mbr", 3 ) == 0 )
	{
	    printf("~mbr\n");
		//testa_mbr();
		goto exit;
    }; 
	
    //root - Testa diretório /root.
    if( stdlib_strncmp( (char *) command, "root", 4 ) == 0 )
	{
	    printf("~/root\n");
		//testa_root();
		goto exit;
    }; 

	//start.
    if( stdlib_strncmp( (char *) command, "start", 5 ) == 0 )
	{
	    printf("~start\n");
		goto exit;
    }; 
	
	//help.
    if( stdlib_strncmp( (char *) command, "help", 4 ) == 0 )
	{
		//printf(help_string);
		//print_help();
		goto exit;
    };
	
	//cls.
    if( stdlib_strncmp( (char *) command, "cls", 3 ) == 0 )
	{
	    //black
	    //api_clear_screen(0);
        goto exit;
	};
	
	//save.
	if( stdlib_strncmp( (char *) command, "save", 4 ) == 0 )
	{
	    printf("~save root\n");
        goto exit;
    };
	
	//install.
	//muda um arquivo da area de transferencia para 
	//o sistema de arquivos...
	if( stdlib_strncmp( (char *) command, "install", 7 ) == 0 )
	{
	    printf("~install\n");
		//fs_install();
        goto exit;
    };
	
	
	//boot - Inicia o sistema.
	if( stdlib_strncmp( (char *) command, "boot", 4 ) == 0 )
	{
	    printf("~boot\n");
		//boot();
        goto exit;
    };

	//service
	if( stdlib_strncmp( (char *) command, "service", 7 ) == 0 )
	{
	    printf("~service - rotina de servicos do kernel base\n");
		//test_services();
        goto exit;
    };

	//slots - slots de processos ou threads.
	if( stdlib_strncmp( (char *) command, "slots", 5 ) == 0 )
	{
	    printf("~slots - mostra slots \n");
		//mostra_slots();
        goto exit;
    };
	
	
    //
    // Continua ...
    //
	
	//exit - Exit the current program
    if( stdlib_strncmp( (char *) command, "exit", 4 ) == 0 ){
		//exit(exit_code);
		exit(0);
		goto fail;
    };
		
    //reboot.
	if( stdlib_strncmp( (char *) command, "reboot", 6 ) == 0 ){
		apiReboot();
		goto fail;
    };

	//shutdown.
    if( stdlib_strncmp( (char *) command, "shutdown", 8 ) == 0 ){
		apiShutDown();
        goto fail;
    };
	

// Fail. Palavra não reservada.	
fail:
    return (int) 1;

//@todo: Esse exit como variavel local precisa mudar de nome	
//       para não confundir com a função exit de sair do processo.
//       uma opção é usar 'done:'. 
exit:    
    return (int) 0;
};


/*
 * stdlib_strncmp:
 *     Compara duas strings.
 *     Obs: Função de uso interno.
 */
int stdlib_strncmp(char *s1, char *s2, int len)
{
	int n = len;
	
	while(n > 0)
	{	
	    n--;
        
		if(*s1 != *s2){ return (int) 1; };
		*s1++; 
		*s2++;
	};		
			
	if (*s1 != '\0' || *s2 != '\0' ){	
	    return (int) 2;
	};		
done:
	return (int) 0;
};

//
// End.
//
