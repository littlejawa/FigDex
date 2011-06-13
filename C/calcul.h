#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "generic.h"

#if defined (__cplusplus)
extern "C" {
#endif

typedef struct
{
	unsigned int order[128] ;
	unsigned int nbPoints ;
} t_fig ;

void createList(void *param) ;

int initCalcul(void) ;
void freeCalcul(void) ;

int waitNewFigs(void) ;
void lockList(void) ;
void unlockList(void) ;

void execOnList(ExecFunc fun, void *param) ;

#if defined (__cplusplus)
}
#endif

#endif
