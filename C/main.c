#include <stdlib.h>
#include "generic.h"

unsigned int createList(unsigned int nbPoints) ;

int main (int argc, char **argv)
{
	int parms ;

	printf("Calculateur de figures Dexiennes v2.0\n") ;
	printf("(c) 2011 Pex Computing\n\n") ;

	if (argc < 2 || atoi(argv[1]) < 3)
	{
		printf("Il faut mettre le nombre de points en parametre\n") ;
		printf("> 2 si possible (ca n'a pas grand interet sinon).\n") ;
		return 0 ;
	}

	printf("Figures pour %d points\n\n", atoi(argv[1])) ;
	parms = atoi(argv[1]) ;
	createList((void *)&parms) ;

	return 0 ;
}
