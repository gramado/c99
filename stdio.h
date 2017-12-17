/*
 * File: stdio.h
 *
 * Descri��o:
 *     Parte da biblioteca C para API 32bit.
 *
 * @todo: 
 *     Usar padr�o C e colocar em outro arquivo o que n�o for daqui.
 *
 * Vers�o 1.0, 2016 - Created.
 */
 
 
//
// Posi��o virtual da mem�ria de v�deo.
//  VA=0x800000 = PA=0x000B8000.
//
#define SCREEN_START 0x800000 
#define ScreenStart SCREEN_START

#define COLUMNS 80
#define SCREEN_WIDTH COLUMNS

#define ROWS 25
#define SCREEN_HEIGHT ROWS

#define SCREEN_WIDTH COLUMNS
#define HEIGHT ROWS

//limite m�ximo. provis�rio.
#define SCREEN_MAX_HEIGHT 256  //linhas.
#define SCREEN_MAX_WIDTH  256  //colunas.

#define REVERSE_ATTRIB 0x70

#define PAD_RIGHT 1
#define PAD_ZERO 2

#define PRINT_BUF_LEN 12

#define KEY_RETURN   13    //@todo: Pertence ao teclado.


//#define EOF	(-1)

//#ifndef EOF
//#define	EOF	(-1)
//#endif

//
// Prompt support.
//

//#define	BUFSIZ	512
//#define	BUFSIZ	1024
//#define BUFSIZ 32768
//Buffer. @todo: Pertence ao Shell??
#define PROMPT_MAX_DEFAULT 256  //Pode ser maior ??

char prompt[PROMPT_MAX_DEFAULT];      //stdin
char prompt_out[PROMPT_MAX_DEFAULT];  //stdout 
char prompt_err[PROMPT_MAX_DEFAULT];  //stderr 
   
int prompt_pos;
int prompt_max;
int prompt_status;
//char prompt_text[] = "$> ";


//
// Obs: O tipo da vari�vel aqui � provis�rio. (UL).
//
//cursor
unsigned long g_cursor_x;
unsigned long g_cursor_y;

//char.
unsigned long g_char_attrib;

//columns and rows
//@todo: Esse precisa ser inicializado
//Obs: Se essas vari�veis forem 0, 
// as rotinas de stdio usar�o os valores default.
//COLUMNS e ROWS.
unsigned long g_columns;
unsigned long g_rows;

int g_using_gui; //modo gr�fico?




#define	STDIN_FILENO	0
#define	STDOUT_FILENO	1
#define	STDERR_FILENO	2


#ifndef FILENAME_MAX
#define	FILENAME_MAX	(260)
#endif

#define FOPEN_MAX	(20)
#define NUMBER_OF_FILES (20)

//unsigned long __iob[NUMBER_OF_FILES]





/*
 * FILE:
 *     Estrutura padr�o para arquivos.    
 *     
 */
typedef struct _iobuf FILE; 
struct _iobuf 
{
	char *_ptr;      //Current position of file pointer (absolute address).
	int   _cnt;      // number of available characters in buffer 
	char *_base;     //Pointer to the base of the file. the buffer
	int   _flag;     //Flags (see FileFlags). the state of the stream
	int   _file;      //UNIX System file descriptor
	int   _charbuf;   
	int   _bufsiz;
	char *_tmpfname;
};
//Fluxo padr�o:
FILE *stdin;
FILE *stdout;
FILE *stderr;

FILE *_io_table[NUMBER_OF_FILES];

#define stdin  (_io_table[0])	
#define stdout 	(_io_table[1])
#define stderr 	(_io_table[2])

/*
 * Prot�tipos do padr�o C.
 */
 
int printf(const char *format, ...);
int sprintf(char *out, const char *format, ...);
int putchar(int ch);
FILE *fopen( const char *filename, const char *mode );
int fclose(FILE *stream);


//
// Outras.
//

void scroll(void);
int app_clear(int color);
int drawBar(int color);  //??

int app_print(char *message, unsigned int line, int color);
static int prints(char **out, const char *string, int width, int pad);
static int printi(char **out, int i, int b, int sg, int width, int pad, int letbase);
static int print(char **out, int *varg);
static void printchar(char **str, int c);
void outbyte(int c);
void _outbyte(int c);
int printf_main(void);    //@todo: Isso � para testes.
unsigned long input(unsigned long ch);




void stdioInitialize();

//
// End.
//
