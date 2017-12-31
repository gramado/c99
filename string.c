/*
 * File: string.c
 *
 * Descrição:
 *     Operações com strings.
 *     Parte da biblioteca da linguagem C.
 *
 * Ambiente:
 *     User mode.
 *
 * Versão: 1.0, 2016 - Created.
 */

 
#include "stddef.h"
#include "string.h"


/*
 * strcmp:
 *     Compara duas strings.
 */
int strcmp(char * s1, char * s2)
{
	int i;
	for(i = 0; s1[i] == s2[i]; ++i)
	{
		if( s1[i] == '\0' ){ return (int) 0; };
	};
	
done:	
	return((int) (s1[i] - s2[i]));
};

/*
 * strncmp:
 *     Compara duas strings.
 */
int strncmp(char *s1, char *s2, int len)
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

/*
 * str_cmp:
 *     Compara strings.
 */
int str_cmp(unsigned char *str1, unsigned char *str2)
{
	while(*str1 == *str2 && *str1 != 0 && *str2 != 0){ 
        str1++; 
        str2++; 
    };
    
	// mesma quantidade de chars iguais
    if(*str1 == *str2){ return (int) 0; };
	if(*str1 > *str2){ return (int) 1; };
	if(*str1 < *str2){ return (int) -1; };
	
    return (int) -1;
};

void *memoryZeroMemory(void* ptr, size_t cnt)
{
    volatile char *vptr = (volatile char *)ptr;
    while (cnt) {
        *vptr = 0;
        vptr++;
        cnt--;
    }
    return (void*) ptr;
};

/*
 * memcpy:
 */
void *memcpy(void *v_dst, const void *v_src, unsigned long c)
{
	const char *src = v_src;
	char *dst = v_dst;

	/* Simple, byte oriented memcpy. */
	while(c--){ *dst++ = *src++; };
	
	return v_dst;
};

/*
 * strcpy: 
 *     Copia uma string.
 */
char *strcpy(char *to, const char *from)
{
	int i = 0;

	while( to[i] = from[i] ){ i += 1; };

	return (to);
};


/*
 * strcat:
 *     Acrescenta uma string ao fim de outra.
 */
char *strcat(char *to, const char *from)
{
	char *ret = to;

	while(*to){ to += 1; };
	
	strcpy(to, from);
	
	return (ret);
};


/*
 *  bcopy:
 *  Copia.
 */
void bcopy(char *from, char *to, int len){
	while(len--){ *to++ = *from++; };
    return;
};


/*
 *  bzero:
 *      Preenche com zeros.
 */
void  bzero(char *cp, int len){
	while(len--) { *(cp + len) = 0; };
    return;
};


/*
 * strlen:
 *     Tamanho de uma string.
 */ 
size_t strlen(const char *s)
{	
    size_t i = 0;
	
	for (i = 0; s[i] != '\0'; ++i){ ; };
	return ( (size_t) i );
};


/*
int kstrlen(const char *s);
int kstrlen(const char *s)
{	
    int i = 0;
	
	for (i = 0; s[i] != '\0'; ++i) {
	;
	};
	
done:
	return((int) i);
};
*/



/*
// Compares two strings. 
//Returns -1 if str1 < str2, 
//0 if they are equal or 
//1 otherwise.
int strcmp(char *str1, char *str2)
{
    int i = 0;
    
    int failed = 0;
    
    while( str1[i] != '\0' && str2[i] != '\0' )
    {
        if(str1[i] != str2[i])
        {
            failed = 1;
            break;
        }
        i++;
    }
 
 
   // why did the loop exit?
    if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
    {    
	    failed = 1;
    }; 
 
	
	return failed;
};

 

int strcmp(const char *a, const char *b)
{
    while(*a && *b) 
	{
        if(*a++ != *b++) 
		    return 1;
    }
    
	if(*a || *b) 
	    return 1;
		
    return 0;
}
*/





/* Copyright (c) 2011, 2012 Jonas 'Sortie' Termansen. */
size_t strcspn(const char* str, const char* reject)
{
	
	//size_t reject_length = 0;
	int reject_length = 0;
	
	while ( reject[reject_length] )
		reject_length++;
	
	
	
	//for ( size_t result = 0; 1; result++ )
    int result;
    for( result = 0; result = 1; result++ )
	{
		char c = str[result];
		if ( !c )
			return (size_t) result;
		//bool matches = 0;
		int matches = 0;
		int i;
		for( i = 0; i < reject_length; i++ )
		{
			if ( str[result] != reject[i] )
				continue;
			matches = 1;
			break;
		}
		if ( matches )
			return (size_t) result;
	}
}



/* Copyright (c) 2011, 2012 Jonas 'Sortie' Termansen. */
size_t strspn(const char* str, const char* accept)
{
	//size_t accept_length = 0;
	int accept_length = 0; 
	while ( accept[accept_length] )
		accept_length++;
	
	
	//for ( size_t result = 0; true; result++ )
	int result;
    for( result = 0; result = 1; result++ )
	{
		char c = str[result];
		if ( !c )
			return (size_t) result;
		
		//bool matches = false;
		int matches = 0;
		int i;
		//for ( size_t i = 0; i < accept_length; i++ )
		for( i=0; i<accept_length; i++ )
		{
			if ( str[result] != accept[i] )
				continue;
			matches = 1;
			break;
		}
		if ( !matches )
			return (size_t) result;
	}
}




/* Copyright (c) 2011, 2012 Jonas 'Sortie' Termansen. */
/*
char* strtok_r(char* str, const char* delim, char** saveptr)
{
	if ( !str && !*saveptr )
		return NULL;
	if ( !str )
		str = *saveptr;
	str += strspn(str, delim); // Skip leading
	if ( !*str )
		return *saveptr = NULL;
	size_t amount = strcspn(str, delim);
	if ( str[amount] )
		*saveptr = str + amount + 1;
	else
		*saveptr = NULL;
	str[amount] = '\0';
	return str;
}
*/



/* Copyright (c) 2011, 2012 Jonas 'Sortie' Termansen. */
/*
char* strtok(char* str, const char* delim)
{
	static char* lasttokensaveptr = NULL;
	return strtok_r(str, delim, &lasttokensaveptr);
}
*/


/*apple*/
char *
strtok_r(char *s, const char *delim, char **last)
{
    char *spanp;
    int c, sc;
    char *tok;

    if (s == NULL && (s = *last) == NULL)
    {
	return NULL;
    }

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = *s++;
    for (spanp = (char *)delim; (sc = *spanp++) != 0; )
    {
	if (c == sc)
	{
	    goto cont;
	}
    }

    if (c == 0)		/* no non-delimiter characters */
    {
	*last = NULL;
	return NULL;
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;)
    {
	c = *s++;
	spanp = (char *)delim;
	do
	{
	    if ((sc = *spanp++) == c)
	    {
		if (c == 0)
		{
		    s = NULL;
		}
		else
		{
		    char *w = s - 1;
		    *w = '\0';
		}
		*last = s;
		return tok;
	    }
	}
	while (sc != 0);
    }
    /* NOTREACHED */
}

/*apple*/
char *
strtok(char *s, const char *delim)
{
    static char *last;

    return strtok_r(s, delim, &last);
}


//
// End.
//

