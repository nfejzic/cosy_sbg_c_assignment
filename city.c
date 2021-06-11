#include "city.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// create a new city and add it to list
City *createCity(CityList *list) {
  City *city = (City *)malloc(sizeof(City));
  addCityToList(list, city);
  return city;
}

// create a new city with speicified values and add it to list
City *createCitySpecified(CityList *list, int id, char *name, int population,
                          int areaId, int elevation) {
  City *city = (City *)malloc(sizeof(City));
  city->id = id;
  *city->name = *name;
  city->population = population;
  city->areaId = areaId;
  city->elevation = elevation;

  addCityToList(list, city);

  return city;
}

void deleteCity(City *city) { free(city); }

// create new list of cities
CityList *createCityList() {
  CityList *list = (CityList *)malloc(sizeof(CityList));
  list->count = 0;
  list->allocated = 10;
  list->cities = (City **)calloc(list->allocated, sizeof(City *));
  return list;
}

// Read cities with their information from file
// Store the City structs into provided CityList
void readCityList(CityList *list, char *filePath, char *fromCity,
                  char *toCity) {
  FILE *inputFile = fopen(filePath, "r");
  if (inputFile == NULL) {
    perror(filePath);
    exit(1);
  }

  int readValues, cityId, population, areaId, elevation;

  do {
    readValues = fscanf(inputFile, "%d", &cityId);
    if (readValues != EOF && readValues > 0) {
      City *city = createCity(list);
      readValues = fscanf(inputFile, "%100s %d %d %d", city->name, &population,
                          &areaId, &elevation);
      city->id = cityId;
      city->population = population;
      city->areaId = areaId;
      city->elevation = elevation;
      city->weight = 0;

      // set city distance to 0 if it is the goal city
      if (strcmp(city->name, toCity) == 0) {
        city->distance = 0;
      } else if (strcmp(city->name, fromCity) == 0) {
        city->distance = -2; // or -2 if it is the start city
      } else {
        city->distance = -1;
        city->weight = -1;
      }
    }
  } while (readValues != EOF && readValues > 0);

  fclose(inputFile);
}

// add a city to specific list
void addCityToList(CityList *list, City *city) {
  if (list->count >= list->allocated) {
    list->allocated *= 2;
    list->cities = realloc(list->cities, list->allocated * sizeof(City *));
  }

  list->cities[list->count] = city;
  list->count++;
}

// get City from a list by it's id
City *getCityFromList(CityList *list, int cityId) {
  for (int i = 0; i < list->count; i++) {
    if (cityId == list->cities[i]->id) {
      return list->cities[i];
    }
  }
  return NULL;
}

void deleteCityList(CityList *list) {
  if (list == NULL)
    return;
  for (int i = 0; i < list->count; i++) {
    free(list->cities[i]);
  }

  free(list->cities);
  free(list);
}

void printCityList(CityList *list) {
  if (list->count > 0) {
    printf("The path is as follows:\n");

    for (int i = 0; i < list->count; i++) {
      printf("%s", list->cities[i]->name);

      if (i != list->count - 1) {
        printf(" -> ");
      }
    }

    printf("\n");
  }
}
