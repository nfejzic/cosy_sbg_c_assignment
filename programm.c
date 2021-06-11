#include <stdio.h>

#include "city.h"
#include "street.h"

// Passt sonst.  Rade
// For schleife ist entfernt. Code angepasst. Vielen dank für Feedback
int main(int argc, char *args[]) {
  if (argc < 6) {
    fputs(
        "Invalid amount of arguments! Arguments needed: \ncitiesFile areasFile "
        "streetsFile fromCity toCity",
        stderr);
  }

  CityList *cityList = createCityList();
  readCityList(cityList, args[1], args[4], args[5]);

  StreetList *sList = createStreetList();
  readStreetList(sList, cityList, args[3]);

  CityList *path = calculatePaths(cityList, sList);
  if (path->count > 0) {
    printCityList(path);
  } else {
    printf("There is no path from %s to %s.\n", args[4], args[5]);
  }

  // free cityList and street list
  deleteCityList(cityList);
  deleteStreetList(sList);

  // free the array of cities in path, and the path pointer
  free(path->cities);
  free(path);
}
