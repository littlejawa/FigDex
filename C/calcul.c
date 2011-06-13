#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calcul.h"

struct _node
{
	unsigned int size ;
	struct _node **children ;
} resultTree ;

int initTree(struct _node *tree, unsigned int nbPoints)
{
	tree->children = (struct _node **)malloc(nbPoints * sizeof(struct _node *)) ;
	if (tree->children == NULL)
		return -1 ;
	memset(tree->children, 0, nbPoints * sizeof(struct _node *)) ;
	tree->size = nbPoints ;
	return 0 ;
}

/*
	fact()

	Définition de la fonction "factorielle".
*/
unsigned int fact(int val)
{
	unsigned int res = 1 ;

	while (val)
	{
		res *= val-- ;
	}

	return res ;
}

/*
	invert()

	Inverse l'ordre des points APRES LE PREMIER.
	Ceci est nécessaire dans la comparaison, pour que le programme détermine
	qu'une figure peut être dessinée dans un sens ou dans l'autre.
	Ex : 1234 = 1432

	Paramètres :
		 fig [IN/OUT]		- en entrée : figure à inverser
											en sortie : la figure est inversée
		 nbPoints [IN]	- nombre de points composant la figure.
*/
void invert(unsigned int *fig, unsigned int nbPoints)
{
	unsigned int val ;
	unsigned int deb = 1, fin = nbPoints-1 ;

	while (deb < fin)
	{
		val = fig[fin] ;
		fig[fin] = fig[deb] ;
		fig[deb] = val ;

		deb++ ;
		fin-- ;
	}
}

void symetrie(unsigned int *fig, unsigned int nbPoints)
{
	unsigned int val ;
	unsigned int deb = 2, fin = nbPoints ;
	int pos1, pos2 ;
	unsigned int i ;

	while (deb < fin)
	{
		pos1 = -1 ;
		pos2 = -1 ;

		for (i = 0 ; i < nbPoints ; i++)
		{
			if (fig[i] == deb)
				pos1 = i ;
			if (fig[i] == fin)
				pos2 = i ;
			if (pos1 != -1 && pos2 != -1)
				break ;
		}

		val = fig[pos1] ;
		fig[pos1] = fig[pos2] ;
		fig[pos2] = val ;

		deb++ ;
		fin-- ;
	}
}

/*
	displayFunc()

	Fonction utilisée pour l'affichage. Format compatible avec qlist_exec()
	pour permettre un affichage rapide de l'ensemble des figures de la liste.

	Parametres :
		 elem [IN]		- pointeur sur structure t_fig
		 unused [//]	- non utilisé

	Retour :
		toujours 0 (pour continuer le traitement)
*/
int displayFunc(void *elem, void *unused)
{
	static int i = 1 ;
	unsigned int j ;
	t_fig *fig = (t_fig *)elem ;


	printf("%04d - ", i++) ;
	for (j = 0 ; j < fig->nbPoints ; j++)
		printf("%d", fig->order[j]) ;

	printf("\n") ;
	return 0 ;
}



/*
	compareFigures()

	Fonction de comparaison des figures.
	Compatible avec le format de qlist_get() afin de permettre une recherche
	rapide dans la liste.

	Paramètres :
		elem [IN]		- pointeur sur structure t_fig
		key [IN]		- pointeur sur tableau d'entier représentant la figure
									à comparer

	Return :
		0 si les figures sont similaires
		!= 0 sinon
*/
int compareFigures(struct _node *tree, unsigned int *schema, int nbPoints)
{
	unsigned int i ;

	if (nbPoints <= 1)
		return 0 ;

	if (schema[0] < schema[1])
		i = schema[1] - schema[0] - 1 ;
	else
		i = tree->size - (schema[0] - schema[1]) - 1 ;

	if (tree->children[i] == NULL)
		return -1 ;

	return compareFigures(tree->children[i], schema + 1, nbPoints-1) ;
}




/*
	roll()

	Fonction de rotation d'une figure.
	Permet de construire un tableau d'entier formant la figure "tournée".

	Paramètres :
		fig [IN]		- figure de référence
		val [IN]		- degré de rotation (en nombre de points)
		res [OUT]		- résultat de la rotation
*/
void roll(t_fig *fig, unsigned int val, unsigned int *res)
{
	unsigned int buf[128] ;
	unsigned int i, j ;
	unsigned int nbPoints = fig->nbPoints ;

	for (i = 0 ; i < nbPoints ; i++)
	{
		buf[i] = (fig->order[i] + val) % nbPoints ;
		if (buf[i] == 0)
			buf[i] = nbPoints ;
	}

	i = 0 ;
	while (buf[i] != 1)
		i++ ;

	for (j = 0 ; j < nbPoints ; j++)
	{
		res[j] = buf[i] ;
		i = (i + 1) % nbPoints ;
	}
}

/*
	compareEquivalent()

	Fonction effectuant les comparaisons nécessaires pour trouver si la figure
	que l'on veut ajouter est déjà présente dans la liste.
	On opère toutes les rotations possibles sur la figure, et on teste la
	présence de chaque rotation, ainsi que de leurs inverses.

	Cette fonction est compatible au format de qlist_get() pour une recherche
	facilitée dans la liste.

	Paramètres :
		elem [IN]		- pointeur sur structure t_fig représentant la figure
									à tester dans la liste
		key [IN]		- pointeur sur structure t_fig représentant la figure
									que l'on veut ajouter

	Return :
		0 si la figure est présente dans la liste
		-1 sinon
*/
int compareEquivalent(t_fig *challenger, int nbPoints)
{
	unsigned int j ;
	unsigned int rotation[128] ;
	unsigned int sym[128] ;

	for (j = 0 ; j < nbPoints ; j++)
	{
		roll(challenger, j, rotation) ;
		memcpy(sym, rotation, 128*sizeof(unsigned int)) ;

		if (compareFigures(&resultTree, rotation, nbPoints) == 0)
			return 0 ;

		invert(rotation, nbPoints) ;
		if (compareFigures(&resultTree, rotation, nbPoints) == 0)
			return 0 ;

		symetrie(rotation, nbPoints) ;
		if (compareFigures(&resultTree, rotation, nbPoints) == 0)
			return 0 ;

		symetrie(sym, nbPoints) ;
		if (compareFigures(&resultTree, sym, nbPoints) == 0)
			return 0 ;
	}

	return -1 ;
}

int add(struct _node *tree, unsigned int *schema, unsigned int nbPoints)
{
	unsigned int forward, backward ;

	if (nbPoints <= 1)
		return -1 ;

	if (schema[0] < schema[1])
	{
		forward = schema[1] - schema[0] - 1 ;
		backward = tree->size - (schema[1] - schema[0]) - 1 ;
	}
	else
	{
		forward = tree->size - (schema[0] - schema[1]) - 1 ;
		backward = schema[0] - schema[1] - 1 ;
	}

	if (tree->children[forward] == NULL)
	{
		tree->children[forward] = (struct _node *)malloc(sizeof(struct _node *)) ;
		if (tree->children[forward] == NULL)
			return -1 ;
		if (initTree(tree->children[forward], tree->size) != 0)
		{
			free(tree->children[forward]) ;
			tree->children[forward] = NULL ;
			return -1 ;
		}
	}
	tree->children[forward]->children[backward] = tree ;

	return add(tree->children[forward], schema + 1, nbPoints-1) ;
}

void createList(void *param)
{
	unsigned int i, j ;
	t_fig precedante ;
	t_fig courrante ;
	unsigned int nbPoints = *(unsigned int *)param ;

	if (initTree(&resultTree, nbPoints) != 0)
		return ;

	courrante.order[0] = 1 ;
	courrante.nbPoints = nbPoints ;

	for (i = 0 ; i < fact(nbPoints - 1) ; i++)
	{
	    //construitune
		for (j = 1 ; j < nbPoints ; j++)
		{
			unsigned int val = 1 ;
			unsigned int x ;

			if (i == 0)
				val = courrante.order[j-1] + 1 ;
			else if (j != nbPoints - 1)
			{
				val = precedante.order[j] ;
				val += (i % fact(nbPoints - (j+1)))?0:1 ;

				val = val % nbPoints ;
				if (val == 0)
					val = nbPoints ;
			}

			for (x = 0 ; x < j ; x++)
			{
				if (courrante.order[x] == val)
				{
					val++ ;
					if (val > nbPoints)
						val = 1 ;
					x = -1 ;
				}
			}

			courrante.order[j] = val ;
			if (j == 1 && val > nbPoints/2 + nbPoints%2)
				return ;
		}

		memcpy(&precedante, &courrante, sizeof(t_fig)) ;

		// see if this figure or equivalent already exists in the list
		if (compareEquivalent(&courrante, nbPoints) != 0)
		{
			// element was not found ! add it to the list
			if (add(&resultTree, &courrante.order, nbPoints) != 0)
			{
				// ERROR !
			}
			displayFunc(&courrante, NULL) ;
		}
	}

	return ;
}
