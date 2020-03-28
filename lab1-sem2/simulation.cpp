#include "simulation.h"

Simulation::Simulation(unsigned starting_year, int years_count,
           const std::vector<std::shared_ptr<RawProduct>>& products,
           const std::vector<std::shared_ptr<Country>>& countries) :
   _starting_year(starting_year) {

   std::vector<ProductIE> basic_vector;
   for (auto &product:products)
      basic_vector.emplace_back(product);

   for (int i = 0; i < years_count; i++) {
      std::vector<ProductIE> current_year = basic_vector;

      for (auto &country:countries){
        country->simulation(current_year);
      }

      _data.push_back(current_year);
   }
}

unsigned Simulation::getStartingYear() {
   return _starting_year;
}

std::vector<std::vector<ProductIE>> Simulation::getData() {
   return _data;
}
