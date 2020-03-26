#ifndef SIMULATION_H
#define SIMULATION_H

#include "product_ie.h"
#include "country.h"

#include <vector>

class Simulation {
private:
   std::vector<std::vector<ProductIE>> _data; //each vector element represents a set of products data in some year

   unsigned _starting_year;

public:
   Simulation(unsigned starting_year, int years_count,
              const std::vector<std::shared_ptr<RawProduct>>& products,
              const std::vector<std::shared_ptr<Country>>& countries);

   unsigned getStartingYear();

   std::vector<std::vector<ProductIE>> getData();
};

#endif // SIMULATION_H
