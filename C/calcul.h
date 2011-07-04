#ifndef _CALCUL_H_
#define _CALCUL_H_

#if defined (__cplusplus)
extern "C" {
#endif

typedef struct
{
	unsigned int order[128] ;
	unsigned int nbPoints ;
} t_fig ;

int initCalcul(unsigned int nbPoints) ;
//void freeCalcul(unsigned int nbPoints) ;

void loadList(int fd, unsigned int nbPoints) ;
void createList(unsigned int nbPoints, int outStream) ;


#if defined (__cplusplus)
}
#endif

#endif
