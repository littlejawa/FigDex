#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "calcul.h"

extern char stop ;

void sig_int_handler(int signum)
{
	printf("STOPPING\n") ;
    stop = 1 ;
}

int main (int argc, char **argv)
{
	int fd = 0 ;
	int tmpInt ;
	unsigned int nbPoints ;
	char fileName[1024] ;

	printf("Calculateur de figures Dexiennes v2.0\n") ;
	printf("2004-2011 Pex Computing\n\n") ;

	if (argc < 2 || atoi(argv[1]) < 3)
	{
		printf("Il faut mettre le nombre de points en parametre\n") ;
		printf("> 2 si possible (ca n'a pas grand interet sinon).\n") ;
		return 0 ;
	}

	// mise en place du handler sur CTRL+C - permet de s'arrêter proprement
	// ce sera particulièrement utile pour interrompre le calcul et le reprendre plus tard
	signal(SIGINT, sig_int_handler) ;

	nbPoints = (unsigned int)atoi(argv[1]) ;
	printf("Figures pour %u points\n\n", nbPoints) ;

	if (initCalcul(nbPoints) != 0)
	{
		printf("Erreur à l'initialisation\n") ;
		return -1 ;
	}

	// ouvre le fichier temporaire permettant la reprise du calcul après interruption
	sprintf(fileName, "precalc%u", nbPoints) ;
	fd = open (fileName, O_CREAT | O_APPEND | O_RDWR, 0666) ;
	if (fd == -1)
	{
		printf("Erreur: impossible d'ouvrir le fichier intermédiaire\n") ;
		return -1 ;
	}

	// vérifie le contenu du fichier.
	if (read(fd, &tmpInt, sizeof(int)) == sizeof(int))
	{
		// S'il n'est pas vide, on essaie de charger son contenu
		printf("Chargement fichier precalc...\n") ;
		loadList(fd, nbPoints) ;
	}
	else
	{
		// initialisation du fichier, si il était vide
		if (write(fd, &nbPoints, sizeof(unsigned int)) != sizeof(int))
		{
			printf("ERROR: initializing precalc file\n") ;
			close (fd) ;
			return -1 ;
		}
	}

	// maintenant, commencer le calcul. Les résultats sont écrits dans le fichier
	// et sur la sortie standard
	printf("Calcul...\n") ;
	createList(nbPoints, fd) ;

//	freeCalcul(nbPoints) ;

	close(fd) ;

	return 0 ;
}
