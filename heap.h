/*
 * File: heap.h
 *
 * Descrição:
 *     O propósito desse header é dar suporte ao gerenciamento de
 * do heap que será usado para a alocação de memória para um programa
 * em user mode. 
 *     Um suporte oferecido pela biblioteca libC99.
 *     Obs: O kernel poderá usar tanto o heap do processo quanto o heap 
 * do desktop ao qual o processo pertence.
 * 
 * Histórico:
 *     Versão 1.0, 2016 - Esse arquivo foi criado por Fred Nora.
 */
 
 
//Contagem de heap.
#define HEAP_COUNT_MAX  256 
 
//@todo: 

//#define HEAP_START  &HeapBuffer[0] //0xC0100000
//#define HEAP_END    &HeapBuffer[(32*1024)-(1)] //0xC02FFFF0    //0xC02FFFF0. 
//#define HEAP_SIZE   (HEAP_END - HEAP_START) 
//talvez deva ser assim
unsigned long HEAP_START; 
unsigned long HEAP_END;
unsigned long HEAP_SIZE;
 
//@todo: padronizar 
unsigned long heapCount;            //Conta os heaps do sistema.

unsigned long heap_start;    //Start.
unsigned long heap_end;      //End.

unsigned long g_heap_pointer;       //Pointer.
unsigned long g_available_heap;     //Available. 
 
// 
// Heap pointer:
//     Isso deve apontar para o heap buffer atual. Quando acabar o heap atual
// deve-se chamar o kernel para criar mais heap dentro da working set do processo. 
//
 
void *Heap;    


//
// *** HEAP ***
//

static unsigned char HeapBuffer[32*1024];  //32KB, provisório.



/*
 * heap_d:
 *     Estrutura para heap.
 *     Cada processo tem seu heap.
 *     Cada heap tem uma lista encadeada de blocos.  
 */  
typedef struct heap_d heap_descriptor_t;
struct heap_d 
{
	int Id;
    int Used;
    int Magic;
    //int ObjectType; //tipo de objeto ao qual pertence o heap.(process, ...)	
	
	unsigned long HeapStart;             
	unsigned long HeapEnd;
	unsigned long HeapPointer;            
	unsigned long AvailableHeap; 	
	
	//struct mmblock_d *nextblock; //lista linkada de blocos. 
};
heap_descriptor_t *libcHeap;
//...


//Heap list.
//obs:. heapList[0] = The Kernel Heap !!!    
unsigned long heapList[HEAP_COUNT_MAX];     


//
// Protótipos.
//

int heapInit();
void heapSetLibcHeap( unsigned long HeapStart, unsigned long HeapSize);


//
// Alloc and Free.
//

unsigned long AllocateHeap( unsigned long size );
void *AllocateHeapEx(unsigned long size);
unsigned long FreeHeap( unsigned long size );

//
// End.
//

