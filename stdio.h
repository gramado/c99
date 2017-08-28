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


//
// Prompt support.
//

//Buffer. @todo: Pertence ao Shell??
#define PROMPT_MAX_DEFAULT 256 
char prompt[PROMPT_MAX_DEFAULT];    
unsigned long prompt_pos;
unsigned long prompt_max;
unsigned long prompt_status;


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

/*
 * FILE:
 *     Estrutura padr�o para arquivos.    
 *     
 */
typedef struct _iobuf FILE; 
struct _iobuf 
{
	char *_ptr;    //Current position of file pointer (absolute address).
	int   _cnt;
	char *_base;   //Pointer to the base of the file.
	int   _flag;   //Flags (see FileFlags).
	int   _file;
	int   _charbuf;
	int   _bufsiz;
	char *_tmpfname;
};
//Fluxo padr�o:
FILE *stdin;
FILE *stdout;
FILE *stderr;


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


//
// End.
//
