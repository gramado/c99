/*
 * File: stdio.c 
 *
 * Descrição:
 *     Rotinas de input/output para User Mode.
 *     Standard C language. C99.
 *
 * versão 1.0, 2015~2016.
 */


#include "stdio.h"
#include "types.h" 

// Testando define.

#define SYSTEMCALL_READ_FILE 123  //@todo: Colocar o número certo.
#define VK_RETURN 12
#define VK_BACKSPACE 11
//PRINT_BUF_LEN


/*
 * stdio_system_call:
 *     Protótipo de função interna. #encapsulada
 *     As funções padrão de stdio chamarão os recursos do
 * kernel atravéz dessa rotina.
 *     System call usada pelo módulo stdio. 
 *     Interrupção de sistema, número 200, chama vários serviços do Kernel 
 * com a mesma interrupção.
 *     Essa é a chamada mais simples.
 *
 * Argumentos:
 *    eax = arg1, o Número do serviço.
 *    ebx = arg2. 
 *    ecx = arg3.
 *    edx = arg4.
 */
void *stdio_system_call( unsigned long ax, 
                         unsigned long bx, 
				         unsigned long cx, 
				         unsigned long dx );

						 
/*
 * fclose:
 *     Close a file.
 *     If work, return 0.
 */
int fclose(FILE *stream)
{
	//
	// @todo: Implementar.
	//
	
	//provisório.
	if( (void*) stream != NULL ){
		stream = NULL;
	};		
	
	//...
	
done:
	return (int) 0;
};


/*
 * fopen:
 *     Open a file.
 *     @todo: Abrir onde? saída padrão?
 *     @retorna o que? o endereço do buffer?
 */
FILE *fopen( const char *filename, const char *mode )
{
	unsigned char *a = (unsigned char *) filename;
	unsigned long m = (unsigned long) mode;
	
	
	unsigned char buffer[512];
	
	//
	// @todo: Criar filtros para os argumentos. Retornar NULL
	// se os argumentos forem inválidos.
	//
	
	//@todo: Chamar uma  rotina que carreggue um arquivo...usar a mesma chamada pela api.
	//stdio_system_call(SYSTEMCALL_READ_FILE, (unsigned long) a, (unsigned long) m, 0);
	
	
	//Criando as estruturas.
	//struct _iobuf *stream;
	FILE *stream;

	
	stream = (FILE *) &buffer[0];
	  
	//f._base = ??;  //colocamos aqui o endereço onde o arquivo foi carregado
	//f._ptr = stdin->_base;
	stream->_cnt = PROMPT_MAX_DEFAULT;
	stream->_file = 0;
	stream->_tmpfname = (char*) filename;	
	
	
	//if( (void*) a == NULL ){
	//    printf("Error: Invalid name.");
	//    return NULL;	
	//};
	
	//return (FILE *) stream; 
    return NULL;	
};


/*
 * scroll:
 *     Scroll the screen in text mode.
 *     @todo: Mudar para tmScroll(void);
 *     @todo: Criar gmScroll(void);
 *     @todo: Essa rotina precisa ser revisada e aprimorada.
 */
void scroll(void)
{
	//Início da tela.
	unsigned short *p1 = (unsigned short *) SCREEN_START;
	
	//Início da segunda linha.
    unsigned short *p2 = (unsigned short *) (SCREEN_START + 2 * SCREEN_WIDTH);
    unsigned short i,j;
	
	//linhas.
	//usa o valor default se passar dos limites.
	if( g_rows == 0 || g_rows >= SCREEN_MAX_HEIGHT ){
		g_rows = SCREEN_HEIGHT;
	};
	
	//24 vezes.
    for( i=0; i < g_rows -1; i++ )
	{
	    //80 vezes.
		
		//Se a variável nao foi inicializada, usar valor default.
		if(g_columns == 0 || g_columns >= SCREEN_MAX_WIDTH){ 
		    g_columns = SCREEN_WIDTH;
		};
		
        
		//modo texto
	    if(g_using_gui == 0)
		{	
		    for( j=0; j < g_columns; j++ ){
                *p1++ = *p2++;
		    };
		};
		
		//modo gráfico.
	    if(g_using_gui == 1)	
		{	
		    for( j=0; j < g_columns; j++ )
			{
                //*p1++ = *p2++;
		        //@todo:
			};
		};
		
	};
	
	
	//
	// Limpando a última linha.
	//
	
	//80 vezes.
    
	//modo texto
	if(g_using_gui == 0){
	    for( i=0; i < g_columns; i++ ){ *p1++ = 0x07*256 + ' '; };
	}
	
	//modo gráfico.
	if(g_using_gui == 1)
	{
	    for( i=0; i < g_columns; i++ )
		{ 
	        //*p1++ = 0x07*256 + ' ';
			//@todo:	
		};    
	};
		
    
done:
	return;
};


/*
 * app_clear:
 *     Limpa a tela.
 *     0x800000 = Endereço virtual da memoria de vídeo.
 *     g_current_vm; 0x000B8000fis = 0x800000vir
 * @todo: Mudar para tmClearScreen(int color);
 */
int app_clear(int color)
{
    unsigned int i = 0;	
	char *vm = (char *) 0x00800000;  //Endereço virtual da VGA. b8000Fis.    

	//modo texto
	if(g_using_gui == 0)
	{	
	    while(i < (SCREEN_WIDTH*SCREEN_HEIGHT*2)) 
        { 
            vm[i] = 219; 
            i++; 
        
            vm[i] = color; 
            i++; 
        };
	};
	
	//modo gráfico.
	if(g_using_gui == 1){
	    g_cursor_x = 0;
	    g_cursor_y = 0;
	};
	    
done:
    return (int) 0; 
};
 

/*
 * app_print:
 *     g_current_vm; 0x000B8000fis = 0x800000vir
 */
int app_print( char *message, unsigned int line, int color )
{ 
    char *vm = (char *) 0x800000;    
    unsigned int i; 
	
	i = (line*SCREEN_WIDTH*2); 

	if(g_using_gui == 0)
	{		
        while(*message!=0) 
        { 
            if(*message=='\n')
            { 
                line++; 
                i = (line*SCREEN_WIDTH*2); 
                *message++; 
            }
            else
            { 
                vm[i] = *message; 
                *message++; 
                i++; 
                vm[i] = color; 
                i++; 
            };
            //Nothing. 			
        };
	    //nothing.
	};
	
	//
	// Mais algo ?
	//   
	
done:
    return (int) 0; 
}; 



/*
 *    Construindo a função printf ......
 */


/*
 * prints:
 *
 */
static int prints( char **out, const char *string, int width, int pad)
{
    register int pc = 0, padchar = ' ';

    
	//
	// filtro: e se width for menor que 0??
	//
	
	//if(width < 0){
	//	return (int) 1;
	//}
	
	if(width > 0) 
	{
	    register int len = 0;
		register const char *ptr;
		
		for(ptr = string; *ptr; ++ptr) ++len;
		if(len >= width) width = 0;
		else width -= len;
		if(pad & PAD_ZERO) padchar = '0';
	};
	
	if( !(pad & PAD_RIGHT) ) 
	{
		for( ; width > 0; --width){
		    printchar(out,padchar);
			++pc;
		};
	};
	
	for( ; *string ; ++string){
		printchar(out, *string);
		++pc;
	};
	
	for( ; width > 0; --width){
		printchar(out,padchar);
		++pc;
	};

done:
	return pc;
};


/*
 * printi:
 *
 */
static int printi( char **out, int i, int b, int sg, int width, int pad, int letbase)
{
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = i;
	char print_buf[PRINT_BUF_LEN];
	
	if(i == 0){
		print_buf[0] = '0';
		print_buf[1] = '\0';		
		return prints(out, print_buf, width, pad);
	};
	
	if(sg && b == 10 && i < 0) 
	{
		neg = 1;
		u = -i;
	};

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';

	while(u) 
	{
		t = u % b;
		
		if(t >= 10)
		    t += letbase - '0' - 10;
		    *--s = t + '0';
		    u /= b;
	};

	if(neg) 
	{
		if( width && (pad & PAD_ZERO) ) 
		{
		    printchar(out, '-');
			++pc;
			--width;
		}
		else 
		{
			*--s = '-';
		};
	};
    //Nothing.
done:
	return pc + prints(out, s, width, pad);
};


/*
 * print:
 *
 */
static int print(char **out, int *varg)
{
	register int width, pad;
	register int pc = 0;
	register char *format = (char *)(*varg++);
	char scr[2];

	for(; *format != 0; ++format) 
	{
		if(*format == '%') 
		{
			++format;
			width = pad = 0;
			
			if(*format == '\0') 
			    break;
			
			if(*format == '%') 
			    goto out;
			
			if(*format == '-') 
			{
				++format;
				pad = PAD_RIGHT;
			};
			
			while(*format == '0') 
			{
				++format;
				pad |= PAD_ZERO;
			};
			
			for( ; *format >= '0' && *format <= '9'; ++format) 
			{
				width *= 10;
				width += *format - '0';
			};
			
			if( *format == 's' ) 
			{
				register char *s = *((char **)varg++);
				pc += prints (out, s?s:"(null)", width, pad);
				continue;
			}
			
			if( *format == 'd' ) 
			{
				pc += printi (out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			
			if( *format == 'x' ) 
			{
				pc += printi (out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			
			if( *format == 'X' ) 
			{
				pc += printi (out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			
			if( *format == 'u' ) 
			{
				pc += printi (out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			
			if( *format == 'c' ) 
			{
				/* char are converted to int then pushed on the stack */
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints (out, scr, width, pad);
				continue;
			}
		}
		else 
		{
		    out:
			    printchar (out, *format);
			    ++pc;
		};
		//Nothing.
	};
	
	if(out) **out = '\0';
	return pc;
};



/* 
 * printf:
 *     Imprime uma string formatada.
 *     Assuming sizeof(void *) == sizeof(int).
 * Obs: Temos uma chamada ao sistema para cada caractere.
 * 
 * Obs: Se passássemos apenas o ponteiro da string para o kernel pintá-la,
 * não estariamos usando o printf da biblioteca em user mode e sim 
 * a printf do kernel.
 */
int printf(const char *format, ...)
{    
	register int *varg = (int *)(&format);
	
    //sincroniza.  
   // vsync();
	
done:	
    //@todo: Implementar um tipo de retorno. (int).
	return print(0, varg);
};



int sprintf(char *out, const char *format, ...)
{
    register int *varg = (int *)(&format);
	
   // vsync();
	
done:
    //@todo: Conferir o padrão C para retorno dessa função.
	return print(&out, varg);
};


static void printchar(char **str, int c)
{
	//extern int putchar(int c);
	if (str) 
    {
		**str = c;
		++(*str);
	}
	else (void)putchar(c);
};


/*
 * putchar:
 *     Put a char in the screen.
 */
int putchar(int ch)
{    
    outbyte(ch);
    return (int) ch;    
};


/*
 * outbyte:
 *
 * @todo: Colocar no buffer de arquivo.
 */
void outbyte(int c)
{
    static char prev = 0;
        
    if( c <  ' '  && c != '\r' && c != '\n' && c != '\t' && c != '\b' ){
        return;
    };
	
	if( c == '\r' ) 
    {
        //Volta ao início da linha.
        g_cursor_x = 0;
        prev = c;
        return;    
    };        
       
    if( c == '\n' && prev != '\r' ) 
    {
        g_cursor_y++;      //Próxima linha.
        g_cursor_x = 0;    //Início da linha.
        prev = c;
        return;
    };
        
    if( c == '\n' && prev == '\r' ) 
    {
        g_cursor_y++;  //Próxima linha.
        prev = c;
        return;
	};

    //Tab.
    if( c == '\t' )  
    {
        g_cursor_x += (4);    //@todo: Criar a var -> 'g_tab_size'.
        prev = c;
        return;         
    };
        
    //Space 
    if( c == ' ' )  
    {
        g_cursor_x++; 
        prev = c;
        return;         
    };
        
    //Delete. 
    if( c == 8 )  
    {
        g_cursor_x--;         
        prev = c;
        return;         
    };
        
    //
    // Filtra as dimensões da janela onde esta pintando.
    //
checkLimits:	
	
	//
	// Colunas.
	//
	
	//Definindo a largura que usaremos.
	//A largura precisa ser maior que '0' e menor que o limite máximo.
	//Obs: @todo: Essa rotina de definição pode ir para o momento da inicialização
	//da biblioteca. Ela contunua aqui porque está funcionando como um filtro.
	//
    if(g_columns == 0 || g_columns >= SCREEN_MAX_WIDTH){
		g_columns = COLUMNS;
	};
	
	//O cursor não pode ultrapassar a largura definida.
	if( g_cursor_x > g_columns ){
        g_cursor_x = 0;
        g_cursor_y++;  
    }else{
        g_cursor_x++;    //Se não alcançamos o limite, apenas incrementa o x.
    };
    
    //
	// Linhas.
	//
	
	//Definindo a altura que usaremos.
	//A altura precisa ser maior que '0' e menor que o limite máximo.
	if(g_rows == 0 || g_rows >= SCREEN_MAX_HEIGHT){
		g_rows = ROWS;
	};
	
    //O cursor não pode ultrapassar a altura definida.	
	//se ultrapassar, chamaremos o scroll.
	//Obs: O scroll ainda não está implementado.
	//O scroll será feito depois que implementarmos o array de ponteiros
	//para estruturas de linha.
	if( g_cursor_y > g_rows ){ 
	    scroll();
        g_cursor_x = 0;             //O cursor deve fica na primeira coluna.
		g_cursor_y = (g_rows-1);    //O cursor deve ficar na última linha.
    };

    // Imprime os caracteres normais.
outputByte:    
	_outbyte(c);
done:    
    prev = c;    //Atualisa o prev. 	
	return;
};


/*
 * _outbyte:
 *     Just output a byte on the screen.
 *
 *     Obs: A função não se preocupa com o cursor.
 *          Essa rotina está preparada somente par ao modo gráfico.
 *          Talvez usaremos um selecionador de modo.   
 */
void _outbyte(int c)
{
    //Nothing. (Obs: Isso está funcionando).
//graphicMode:	
	stdio_system_call( 7, 8*g_cursor_x,  8*g_cursor_y, (unsigned long) c); 
	return;

/*
   unsigned long i, x, y;
	char ch = (char) c;
	char ch_atributo = (char) 7;    //g_char_attrib;
    char *vm = (char *) 0x00800000;  //g_current_vm; 0x000B8000fis;   	
	
    //
    // my_buffer_char_blt( 8*g_cursor_x, 8*g_cursor_y, COLOR_WHITE, c );
	//

	
	//
	// para modo gráfico.
	//
	//stdio_system_call( 130, g_cursor_x,  g_cursor_y, (unsigned long) c);


    //Configura.
	y = (g_cursor_y * 80 *2);
    x = g_cursor_x*2;    
    i = y + x;
    

	//Envia o char pra memória.	
    vm[i+0] = ch;             //char.
    vm[i+1] = ch_atributo;    //atributo (foreground,background).
	
	
	//
	// Ok.
	//
	
	*/
    
done:
    return;     
};


/*
 * printf_main:
 *     Rotina de teste para a função printf().
 *
 *    @todo: Esse teste deve ir pra outro arquivo ou programa.
 *
 * Obs:
 * if you compile this file with
 *   gcc -Wall $(YOUR_C_OPTIONS) -DTEST_PRINTF -c printf.c
 * you will get a normal warning:
 *   printf.c:214: warning: spurious trailing `%' in format
 * this line is testing an invalid % at the end of the format string.
 *
 * this should display (on 32bit int machine) :
 *
 * Hello world!
 * printf test
 * (null) is null pointer
 * 5 = 5
 * -2147483647 = - max int
 * char a = 'a'
 * hex ff = ff
 * hex 00 = 00
 * signed -3 = unsigned 4294967293 = hex fffffffd
 * 0 message(s)
 * 0 message(s) with %
 * justif: "left      "
 * justif: "     right"
 *  3: 0003 zero padded
 *  3: 3    left justif.
 *  3:    3 right justif.
 * -3: -003 zero padded
 * -3: -3   left justif.
 * -3:   -3 right justif.
 */
int printf_main(void)
{
	/*
	char *ptr = "Hello world!";
	char *np = 0;
	int i = 5;
	unsigned int bs = sizeof(int)*8;
	int mi;
	char buf[80];

	mi = (1 << (bs-1)) + 1;
	
//	vsync();
	
	printf("%s\n", ptr);
	printf("printf test\n");
	printf("%s is null pointer\n", np);
	printf("%d = 5\n", i);
	printf("%d = - max int\n", mi);
	printf("char %c = 'a'\n", 'a');
	printf("hex %x = ff\n", 0xff);
	printf("hex %02x = 00\n", 0);
	printf("signed %d = unsigned %u = hex %x\n", -3, -3, -3);
	printf("%d %s(s)%", 0, "message");
	printf("\n");
	printf("%d %s(s) with %%\n", 0, "message");
	
    sprintf(buf, "justif: \"%-10s\"\n", "left"); 
    printf("%s", buf);
	
    sprintf(buf, "justif: \"%10s\"\n", "right"); 
    printf("%s", buf);
	
    sprintf(buf, " 3: %04d zero padded\n", 3); 
    printf("%s", buf);
	
    sprintf(buf, " 3: %-4d left justif.\n", 3); 
    printf("%s", buf);
	
    sprintf(buf, " 3: %4d right justif.\n", 3); 
    printf("%s", buf);
	
    sprintf(buf, "-3: %04d zero padded\n", -3); 
    printf("%s", buf);
	
    sprintf(buf, "-3: %-4d left justif.\n", -3); 
    printf("%s", buf);
	
    sprintf(buf, "-3: %4d right justif.\n", -3); 
    printf("%s", buf);

	*/
done:
	return (int) 0;
};


/*
 * input:
 *     Coloca os caracteres digitados em uma string. 'prompt[]'
 */
unsigned long input(unsigned long ch)
{
	char c = (char) ch;    //converte.
	
	
	if( prompt_max == 0|| prompt_max >= PROMPT_MAX_DEFAULT){
		prompt_max = PROMPT_MAX_DEFAULT;
	}
	
	//Filtra limite.
	if(prompt_pos > prompt_max){ 
	    printf("input: The buffer is full!\n");	
	    return (unsigned long) 0;  //retornar 1?? 
	};
 
	//
	// Trata caractere digitado.
	//
	
	switch(c)
	{
	    //Enter.	
		case VK_RETURN:
		    prompt[prompt_pos] = (char )'\0';
            goto input_done;			
		    break;

	    //Backspace.
		case VK_BACKSPACE:
            
			//Se estamos no início da linha.
			if(prompt_pos <= 0){ 
			    prompt[prompt_pos] = (char ) '\0';
				break; 
			};
		    
			//Apaga o anterior.
			prompt_pos--;
			prompt[prompt_pos] = (char ) '\0'; //apaga o buffer
			g_cursor_x--;
			//printf("%c",' ');                  //apaga na tela. 
			putchar(' ');
			
			//Vamos deixar o sistema mostrar o cursor.
			
			//Mostra o cursor.
			//printf("%c",'$');	
			
			//posiciona bem onde foi pintado o cursor.
			//g_cursor_x--;     
			break;
			
		//Continua ...
		
		default:
		    
			//
			// Vamos deixar o sistema mostrar o cursor.
			//
			
			//Coloca carctere normal no buffer.
		    prompt[prompt_pos] = c;
			
			//Incrementa posicionamento no buffer.
		    prompt_pos++;
			
			//g_cursor_x--;
			
			//Apaga o cursor, que é o atual.
			//printf("%c",' ');                  //apaga a tela. 
			//putchar(' ');
			
            //Volta o posicionamento na tela porque o avançou.
			//g_cursor_x--; 
			
			//Mostra o char na tela.
			//printf("%c",(char) c);
			//putchar(c);

			//Mostra o cursor.
			//printf("%c",'$');	
			
			//posiciona bem onde foi pintado o cursor.
			//g_cursor_x--;     
			break;
			
	};
	
input_more:	
	return (unsigned long) 0;

input_done:	
    return VK_RETURN;	
};


/*
 * stdio_system_call:
 *     System call usada pelo módulo stdio.  
 *     Função interna. 
 *     As funções padrão de stdio chamarão recursos do kernel atravéz dessa 
 * rotina.
 *     Interrupção de sistema, número 200, personalizada para stdio.
 *     Chama vários serviços do Kernel com a mesma interrupção.
 *     Essa é a chamada mais simples.
 *
 * Argumentos:
 *    eax = arg1, o Número do serviço.
 *    ebx = arg2. 
 *    ecx = arg3.
 *    edx = arg4.
 */
void *stdio_system_call( unsigned long ax, 
                         unsigned long bx, 
				         unsigned long cx, 
				         unsigned long dx )
{
    int Ret = 0;	
	
    //System interrupt. 	
	asm volatile( " int %1 \n"
		          : "=a"(Ret)	
		          : "i"(200), "a"(ax), "b"(bx), "c"(cx), "d"(dx) );
done:
	return (void *) Ret; 
};


/*
// Return BUFFER_SIZE. 
int input_file_buffer_size(void);
int input_file_buffer_size(void)
{
  return (BUFFER_SIZE);
}
*/

void stdioInitialize()
{
	//buffers para as estruturas.
	unsigned char buffer0[512];
	unsigned char buffer1[512];
	unsigned char buffer2[512];
	
	
	stdin  = (FILE *) &buffer0[0];	
	stdout = (FILE *) &buffer1[0];	
	stderr = (FILE *) &buffer2[0];	
	
	  
	stdin->_base = &prompt[0];
	stdin->_ptr = stdin->_base;
	stdin->_cnt = PROMPT_MAX_DEFAULT;
	stdin->_file = 0;
	stdin->_tmpfname = "stdin";
	//...
	
	stdout->_base = &prompt_out[0];
	stdout->_ptr = stdout->_base;
	stdout->_cnt = PROMPT_MAX_DEFAULT;
	stdout->_file = 1;
	stdout->_tmpfname = "stdout";
	//...
	
	stderr->_base = &prompt_err[0];
	stderr->_ptr = stderr->_base;
	stderr->_cnt = PROMPT_MAX_DEFAULT;
	stderr->_file = 2;
	stderr->_tmpfname = "stderr";	
	//...
	
	int i;
	for(i=0; i<PROMPT_MAX_DEFAULT;i++)
	{
		prompt[i] = (char) '\0';
		prompt_out[i] = (char) '\0';
		prompt_err[i] = (char) '\0';
	};	
	
}

//
// End.
//

