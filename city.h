#ifndef CITY
#define CITY

typedef struct City City;
struct City {
  int id;
  char name[101];
  int population;
  int areaId;
  int elevation;
  int distance; // used for path calculation
  int weight;   // for finding shortest path
};

typedef struct {
  City **cities;
  int count;
  int allocated;
} CityList;

City *createCity(CityList *list);
City *createCitySpecified(CityList *list, int id, char *name, int population,
                          int areaId, int elevation);
void deleteCity();

CityList *createCityList();
void readCityList(CityList *list, char *filePath, char *fromCity, char *toCity);
void addCityToList(CityList *list, City *city);
City *getCityFromList(CityList *list, int cityId);
// City *getCityFromListByDistance(CityList *list, int distance);
void deleteCityList(CityList *list);
void printCityList(CityList *list);

#endif
