#include "city.h"
#include <stdlib.h>

#ifndef STREET
#define STREET

typedef struct {
  int fromCityId;
  int toCityId;
  int length;
  City *fromCity;
  City *toCity;
} Street;

typedef struct {
  int count;
  int allocated;
  Street **streets;
} StreetList;

Street *createStreet(StreetList *list);
void deleteStreet();

StreetList *createStreetList();
void readStreetList(StreetList *list, CityList *cityList, char *filePath);
void addStreetToList(StreetList *list, Street *street);
void printStreetList(StreetList *list);
void deleteStreetList(StreetList *list);

CityList *calculatePaths(CityList *cities, StreetList *list);

void addCitiesToStart(CityList *cities, StreetList *sList, City *startCity);
City *getTargetCity(StreetList *sList);
City *findClosestCity(StreetList *sList, City *fromCity);

#endif
