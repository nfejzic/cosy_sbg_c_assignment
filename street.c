#include "street.h"

#include <stdio.h>
#include <sys/cdefs.h>

// creates an empty street pointer and stores it into the given list
Street *createStreet(StreetList *list) {
  Street *street = (Street *)malloc(sizeof(Street));
  if (list != NULL) {
    addStreetToList(list, street);
  }

  return street;
}

// creates a new StreetList and returns it
StreetList *createStreetList() {
  StreetList *list = (StreetList *)malloc(sizeof(StreetList));
  list->allocated = 10;
  list->count = 0;
  list->streets = (Street **)calloc(list->allocated, sizeof(Street *));

  return list;
}

void addStreetToList(StreetList *list, Street *street) {
  if (list->count >= list->allocated) {
    list->allocated *= 2;
    list->streets = realloc(list->streets, list->allocated * sizeof(Street *));
  }
  list->streets[list->count] = street;
  list->count++;
}

// reads a file with specified streets, creates Street structs as specified in
// file and stores them into a provided StreetList
void readStreetList(StreetList *list, CityList *cityList, char *filePath) {
  if (list == NULL || cityList == NULL) {
    printf("The provided street list, or city list is NULL. Can't store the "
           "data.\nAborting!");
    return;
  }

  FILE *inputFile = fopen(filePath, "r");

  if (inputFile == NULL) {
    perror(filePath);
    exit(1);
  }

  int readValues, fromCityId, toCityId, length;

  do {
    readValues = fscanf(inputFile, "%d", &fromCityId);
    if (readValues != EOF && readValues > 0) {
      readValues = fscanf(inputFile, "%d %d", &toCityId, &length);

      Street *street = createStreet(list);

      street->fromCityId = fromCityId;
      street->fromCity = getCityFromList(cityList, fromCityId);
      street->toCityId = toCityId;
      street->toCity = getCityFromList(cityList, toCityId);
      street->length = length;
    }
  } while (readValues != EOF && readValues > 0);

  fclose(inputFile);
}

void deleteStreetList(StreetList *list) {
  for (int i = 0; i < list->count; i++) {
    free(list->streets[i]);
  }

  free(list->streets);
  free(list);
}

// Finds the path from city1 to city2 speicifed as program parameters in reverse
// order (starts from city2 and goes backwards to city 1). Stores the weight
// values into each city it encounters at the path. If city2 is reachable from
// city1, shortest path will be found using the weight values. Weight value in
// each city is the distance to the city2

CityList *calculatePaths(CityList *cityList, StreetList *list) {
  CityList *path = createCityList();
  City *end = NULL;
  int reached = 0;         // false
  int currentDistance = 0; // how many stops away from the city2

  while (reached == 0) {
    City *startCity;
    City *currentTarget;
    for (int s = 0; s < list->count; s++) {
      // if the city we're checking is reachable from a city, modify it's
      // data
      if (list->streets[s]->fromCity->distance == currentDistance) {
        startCity = list->streets[s]->fromCity;
        currentTarget = list->streets[s]->toCity;

      } else if (list->streets[s]->toCity->distance == currentDistance) {
        startCity = list->streets[s]->toCity;
        currentTarget = list->streets[s]->fromCity;

      } else {
        continue;
      }

      // only affect not yet visited cities
      // -1 not visited, -2 is goal
      if (currentTarget->distance < 0) {
        currentTarget->weight = list->streets[s]->length + startCity->weight;

        if (currentTarget->distance == -1) {
          currentTarget->distance = currentDistance + 1;
        }
        if (currentTarget->distance == -2) {
          end = currentTarget;
        }
      }
    }

    currentDistance++;
    if (currentDistance > cityList->count)
      // maximum distance is if we visit all cities
      reached = 1;
  }

  // if city2 is reachable from city1, find the shortest path
  if (end != NULL) {
    addCityToList(path, end);

    while (end != NULL && end->distance != 0) {
      end = findClosestCity(list, end);
      if (end != NULL)
        addCityToList(path, end);
    }
  }

  return path;
}

// Is called if city2 is reachable from city1
// Finds the city with smallest weight (distance from city2)
// connected to any given city
// Uses minSort
City *findClosestCity(StreetList *sList, City *fromCity) {
  int minWeight = -1;
  City *result = NULL;

  for (int i = 0; i < sList->count; i++) {
    // if there is a street connecting the two
    if (sList->streets[i]->fromCity == fromCity ||
        sList->streets[i]->toCity == fromCity) {
      City *targetCity = sList->streets[i]->toCity;

      // in case connection goes other way
      if (sList->streets[i]->toCity == fromCity) {
        targetCity = sList->streets[i]->fromCity;
      }

      // find closest (smallest weight)
      if (targetCity->distance != -1) {
        if (minWeight == -1)
          minWeight = targetCity->weight;

        // city is relevant if it's on the path between start and goal and
        // it is closer to the goal than other checked cities and
        // it is closer to the goal than the current city
        if (targetCity->weight != -1 && targetCity->weight <= minWeight &&
            targetCity->weight < fromCity->weight) {
          minWeight = targetCity->weight;
          result = targetCity;
        }
      }
    }
  }

  return result;
}
