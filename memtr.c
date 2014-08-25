

#define _GNU_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dlfcn.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static int is_initd = 0;
static int is_on = 0;
static int memtr_fd = -1;


typedef void *(*malloc_type)(size_t size);
typedef void (*free_type)(void *ptr);
typedef void *(*calloc_type)(size_t nmemb, size_t size);
typedef void *(*realloc_type)(void *ptr, size_t size);

static malloc_type orig_malloc;
static free_type orig_free;
//static calloc_type orig_calloc;
static realloc_type orig_realloc;


static void memtr_init()
{
	char *p;

	if (is_initd)
		return;

	is_initd = 1;

	p = getenv("MEMTR");
	if (p) {
		memtr_fd = open( p, O_CREAT|O_TRUNC|O_RDWR, 0666 );
	}

	orig_malloc = (malloc_type)dlsym( RTLD_NEXT, "malloc" );
	orig_free = (free_type)dlsym( RTLD_NEXT, "free" );
	//orig_calloc = (calloc_type)dlsym( RTLD_NEXT, "calloc" );
	orig_realloc = (realloc_type)dlsym( RTLD_NEXT, "realloc" );

	if (orig_malloc && orig_free && orig_realloc && memtr_fd >= 0) {
		is_on = 1;
	}
}


void itoa(unsigned long i, char *b)
{
	unsigned long shifter = i;
	char *p = b;
	do {
		++p;
		shifter = shifter/10;
	} while (shifter);
	*p = '\0';
	do {
		*--p = '0' + i%10;
		i = i/10;
	} while (i);
}
void itox(unsigned long i, char *b)
{
	static const char hd[16] = "0123456789abcdef";
	int k;
	*b = '0'; b++;
	*b = 'x'; b++;
	for (k=sizeof(long)-1; k>=0; k--) {
		unsigned long mask = 0xf0 << (k*8);
		*b = hd[ ((i & mask)>>4) >> (k*8) ]; b++;
		*b = hd[ (i & (mask>>4)) >> (k*8) ]; b++;
	}
	*b = 0;
}


static void memtr_print( const char *name, size_t size, void *ptr )
{
	static char buf1[128], buf2[32];
	char *p;
	int len;
	if (is_on) {
		p = buf1;

		len = strlen(name);
		memcpy( p, name, len );
		p += len;

		*p = '\t'; p++;

		itoa( (int)size, buf2 );
		len = strlen( buf2 );
		memcpy( p, buf2, len );
		p += len;

		*p = '\t'; p++;

		itox( (unsigned long)ptr, buf2 );
		len = strlen( buf2 );
		memcpy( p, buf2, len );
		p += len;

		*p = '\n'; p++;

		write( memtr_fd, buf1, p - buf1 );
	}
}

void *malloc(size_t size)
{
	void *p;

	memtr_init();
	memtr_print( "m<", size, 0 );
	p = orig_malloc( size );
	memtr_print( "m>", size, p );

	return p;
}

void free(void *ptr)
{
	memtr_init();
	memtr_print( "f<", 0, ptr );
	orig_free( ptr );
	memtr_print( "f>", 0, ptr );
}

/*
void *calloc(size_t nmemb, size_t size)
{
}*/

void *realloc(void *ptr, size_t size)
{
	void *p;
	memtr_init();
	memtr_print( "r<", size, ptr );
	p = orig_realloc( ptr, size );
	memtr_print( "r>", size, p );
	return p;
}

