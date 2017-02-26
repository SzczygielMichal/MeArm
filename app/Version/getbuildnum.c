#include "getbuildnum.h"
#include "app\version\version.h"

/**
* Pobiera numer build'u / kompilacji
*/
unsigned int Get_BuildNumberFromMainProg(void)
{
  unsigned int *ptr;

  ptr = (unsigned int*)0x0805FE00;
  return *ptr;
}

/**
* Pobiera numer build'u / kompilacji
*/
unsigned int GetBuildNumber()
{
  return REVISION; // Zwraca numer rewizji idczytany z repozytorium
}





















