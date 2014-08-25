
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	void *p;
	p = malloc( 1024 );
	p = realloc( p, 2048 );
	free( p );
	return 0;
}
