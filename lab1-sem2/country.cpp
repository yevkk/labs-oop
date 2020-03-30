#include "country.h"

#include <cmath>

Country::Country() {

}

Country::Country(const std::string& name, const ProductionList& production_list) :
   _name(name),
   _production_list(production_list) {

}

std::string Country::getName() {
   return _name;
}

ProductionList Country::getProductionList() {
   return _production_list;
}

void Country::setProductionList(const ProductionList &production_list)
{
   _production_list = production_list;
}

//======================================================

CountryIEPolicy1::CountryIEPolicy1(const std::string &name, const ProductionList &production_list,
                                   const ConsumptionList &consumption_list) :
   Country(name, production_list),
   _consumption_list(consumption_list) {

}

ConsumptionList CountryIEPolicy1::getConsumptionList() {
   return _consumption_list;
}

void CountryIEPolicy1::simulation(std::vector<ProductIE>& products, double random_precision) {
   //EXPORT: all products from production list;
   //IMPORT: all products from consumption list;

   auto production_list = getProductionList();

   for(auto &production_item:production_list) {
      for(auto &product:products) {
         if (production_item.first == product.getProduct()){
               int real_production = Random::randomDouble(production_item.second * (1 - random_precision),
                                                          production_item.second * (1 + random_precision));
               product.incExport(real_production);
         }
      }
   }

   for(auto &consumption_item:_consumption_list) {
      for(auto &product:products) {
         if (consumption_item.first == product.getProduct()){
               int real_consumption = Random::randomDouble(consumption_item.second * (1 - random_precision),
                                                           consumption_item.second * (1 + random_precision));
               product.incImport(real_consumption);
         }
      }
   }
}

//======================================================

CountryIEPolicy2::CountryIEPolicy2(const std::string &name, const ProductionList &production_list,
                                   const ConsumptionList &consumption_list) :
   Country(name, production_list),
   _consumption_list(consumption_list) {

}

ConsumptionList CountryIEPolicy2::getConsumptionList() {
   return _consumption_list;
}

void CountryIEPolicy2::simulation(std::vector<ProductIE>& products, double random_precision) {
   auto production_list = getProductionList();
   auto consumption_list = getConsumptionList();

   std::vector<int> produced_amount(production_list.size()); //i-th element corresponds to production_list[i];
   std::vector<int> consumpted_amount(consumption_list.size()); //i-th element corresponds to consumption_list[i];

   for (unsigned i = 0; i < produced_amount.size(); i++)
      produced_amount[i] = Random::randomDouble(production_list[i].second * (1 - random_precision),
                                                production_list[i].second * (1 + random_precision));

   for (unsigned i = 0; i < consumpted_amount.size(); i++) {
       consumpted_amount[i] = Random::randomDouble(consumption_list[i].second * (1 - random_precision),
                                                   consumption_list[i].second * (1 + random_precision));
   }

   for (unsigned i = 0; i < production_list.size(); i++) {
       for (unsigned j = 0; j < consumption_list.size(); j++) {
           if (consumption_list[j].first == production_list[i].first) {
               if (consumpted_amount[j] >= produced_amount[i]) {
                   consumpted_amount[j] -= produced_amount[i];
                   produced_amount[i] = 0;
               } else {
                   produced_amount[i] -= consumpted_amount[j];
                   consumpted_amount[j] = 0;
               }
           }
       }
   }

   for (unsigned j = 0; j < consumption_list.size(); j++) {
      if(consumption_list[j].first->getRawList().empty()) {
          auto product = consumption_list[j];
          bool flag = true;

          for(unsigned i = 0; i < production_list.size(); i++) {
             if (product.first == production_list[i].first) {
                produced_amount[i] -= consumpted_amount[j];
                flag = false;
                break;
             }
          }

          if (flag) {
             for (unsigned i = 0; i < products.size(); i++) {
                if (product.first == products[i].getProduct()) {
                   products[i].incImport(consumpted_amount[j]);
                   break;
                }
             }
          }
      }

      for (auto &product:consumption_list[j].first->getRawList()) {
         if (!product.first->getRawList().empty()) continue;

         bool flag = true;
         for(unsigned i = 0; i < production_list.size(); i++) {
            if (product.first == production_list[i].first) {
               produced_amount[i] -= product.second * consumpted_amount[j];
               flag = false;
               break;
            }
         }

         if (flag) {
            for (unsigned i = 0; i < products.size(); i++) {
               if (product.first == products[i].getProduct()) {
                  products[i].incImport(product.second * consumpted_amount[j]);
                  break;
               }
            }
         }
      }
   }

   for (unsigned i = 0; i < production_list.size(); i++) {
      if (!production_list[i].first->getRawList().empty()) continue;

      for (unsigned j = 0; j < products.size(); j++) {
         if (production_list[i].first == products[j].getProduct()) {
            if (produced_amount[i] >= 0) {
               products[j].incExport(produced_amount[i]);
            } else {
               products[j].incImport(abs(produced_amount[i]));
            }

            break;
         }
      }
   }
}

//======================================================

CountryIEPolicy3::CountryIEPolicy3(const std::string& name, const ProductionList& production_list,
                 const std::vector<unsigned>& consumption_volumes_list) :
   Country(name, production_list),
   _consumption_volumes_list(consumption_volumes_list) {

}

void CountryIEPolicy3::setComsumptionVolumesList(const std::vector<unsigned> &new_list) {
   _consumption_volumes_list = new_list;
}

void CountryIEPolicy3::simulation(std::vector<ProductIE>& products, double random_precision) {
   auto production_list = getProductionList();

   std::vector<int> produced_amount(production_list.size()); //i-th element corresponds to production_list[i];
   std::vector<int> consumpted_amount(_consumption_volumes_list.size()); //i-th element corresponds to consumption_list[i];

   for (unsigned i = 0; i < produced_amount.size(); i++)
      produced_amount[i] = Random::randomDouble(production_list[i].second * (1 - random_precision),
                                                production_list[i].second * (1 + random_precision));

   for (unsigned i = 0; i < consumpted_amount.size(); i++)
      consumpted_amount[i] = Random::randomDouble(_consumption_volumes_list[i] * (1 - random_precision),
                                                  _consumption_volumes_list[i] * (1 + random_precision));

   for (unsigned i = 0; i < production_list.size(); i++) {
      for (auto &product:production_list[i].first->getRawList()) {
         bool flag = true;

         for(unsigned j = 0; j < production_list.size(); j++) {
            if (product.first == production_list[j].first) {
               produced_amount[j] -= product.second * produced_amount[i];
               flag = false;
               break;
            }
         }

         if (flag) {
            for (unsigned j = 0; j < products.size(); j++) {
               if (product.first == products[j].getProduct()) {
                  products[j].incImport(product.second * produced_amount[i]);
                  break;
               }
            }
         }
      }
   }

   for (unsigned i = 0; i < produced_amount.size(); i++) {
      produced_amount[i] -= consumpted_amount[i];
   }

   for (unsigned i = 0; i < production_list.size(); i++) {
      for (unsigned j = 0; j < products.size(); j++) {
         if (production_list[i].first == products[j].getProduct()) {
            if (produced_amount[i] >= 0) {
               products[j].incExport(produced_amount[i]);
            } else {
               products[j].incImport(abs(produced_amount[i]));
            }

            break;
         }
      }
   }

}

//======================================================

std::shared_ptr<Country> randomCountry(std::vector<std::shared_ptr<RawProduct>> products) {
   std::shared_ptr<Country> res;
   double random = Random::randomDouble(0, 1);

   std::vector<std::shared_ptr<RawProduct>> products_copy(products);
   ProductionList production_list;
   int production_list_size = Random::randomInt(0, 7);
   for(int i = 0; i < products_copy.size() && i < production_list_size; i++) {
      unsigned chosen_index = Random::randomInt(0, products_copy.size() - 1);
      auto tmp = products_copy[chosen_index];
      products_copy.erase(products_copy.begin() + chosen_index);
      production_list.emplace_back(tmp, Random::randomInt(1, 100));
   }

   if (random < 0.5) {
      ProductionList consumption_list;
      int consumption_list_size = Random::randomInt(0, 7);
      for(int i = 0; i < products.size() && i < consumption_list_size; i++) {
         unsigned chosen_index = Random::randomInt(0, products.size() - 1);
         auto tmp = products[chosen_index];
         products.erase(products.begin() + chosen_index);
         consumption_list.emplace_back(tmp, Random::randomInt(1, 100));
      }

      if (random < 0.25) {
         static int p1_created = 1;
         std::string name = "Country(P1) " + std::to_string(p1_created++);

         res = std::make_shared<CountryIEPolicy1>(CountryIEPolicy1(name, production_list, consumption_list));
      } else {
         static int p2_created = 1;
         std::string name = "Country(P2) " + std::to_string(p2_created++);

         res = std::make_shared<CountryIEPolicy2>(CountryIEPolicy2(name, production_list, consumption_list));
      }

   } else {
      std::vector<unsigned> consumption_volumes_list;
      for(unsigned i = 0; i < production_list.size(); i++) {
         consumption_volumes_list.emplace_back(Random::randomInt(1, 100));
      }

      if (random < 0.75) {
          static int p3_created = 1;
          std::string name = "Country(P3) " + std::to_string(p3_created++);

          res = std::make_shared<CountryIEPolicy3>(CountryIEPolicy3(name, production_list, consumption_volumes_list));
      } else {
          static int p3_created = 1;
          std::string name = "Country(P3) " + std::to_string(p3_created++);

          res = std::make_shared<CountryIEPolicy3>(CountryIEPolicy3(name, production_list, consumption_volumes_list));
      }

   }

   return res;
}
