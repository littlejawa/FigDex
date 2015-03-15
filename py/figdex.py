#!/usr/bin/env python

import sys

def test(liste, resultats):
#  print "test", liste, resultats
  if (liste in resultats):
    return True
  # inversion des points apres le premier
  # Ceci est necessaire dans la comparaison, pour que le programme determine
  # qu'une figure peut etre dessinee dans un sens ou dans l'autre.
  # Ex : 1234 = 1432
  rev=list(liste[1:])
  rev.reverse()
  rev.insert(0, liste[0])
  if (rev in resultats):
    return True
    
  # cherche le point 1 pour commencer le dessin. Verifie qu'en partant de
  # celui-ci, on ne retrouve pas des figures existantes
  i=liste.index(1)
  if (i > 0):
    newList=list(liste[i:])
    newList = newList + list(liste[:i])
    if (test(newList, resultats)):
      return True
  return False

def calcul(listPrefix, remainingPoints, resultats):
#  print "calcul", listPrefix, remainingPoints, resultats
  if (len(remainingPoints) == 1):
    # terminus
    liste = list(listPrefix)
    liste.append(remainingPoints[0])
    if (test(liste, resultats) == False):
#      print "Ajout de", liste
      resultats.append(liste)
#    else:
#      print "Skip", liste
  else:
    for i in range(0, len(remainingPoints)):
      liste = list(listPrefix)
      liste.append(remainingPoints[i])
      fin = list(remainingPoints)
      del fin[i]
      calcul(liste, fin, resultats)
#      print "loop:", i, listPrefix, remainingPoints, resultats
  return resultats


if (len(sys.argv) < 2):
  print "param: nombre de points"
  sys.exit(0)

print "Calcul pour " + sys.argv[1] + " points"

remaining=[]
for i in range(0, int(sys.argv[1])):
  remaining.append(i+1)

res = calcul ([], remaining, [])

print "Resultats:"
print res

