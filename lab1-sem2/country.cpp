#include "country.h"

#include <cmath>
#include <algorithm>
#include <iterator>

Country::Country() {

}

Country::Country(const std::string& name, const ProductionList& production_list) :
   _name(name),
   _production_list(production_list) {

}

std::string Country::getName() const {
   return _name;
}

ProductionList Country::getProductionList() const {
   return _production_list;
}

void Country::setProductionList(const ProductionList &production_list)
{
   _production_list = production_list;
}

//======================================================

HasConsumptionList::HasConsumptionList(const ConsumptionList& consumption_list) :
    _consumption_list(consumption_list) {};

ConsumptionList HasConsumptionList::getConsumptionList() const{
    return _consumption_list;
}

//======================================================

CountryIEPolicy1::CountryIEPolicy1(const std::string &name, const ProductionList &production_list,
                                   const ConsumptionList &consumption_list) :
   Country(name, production_list),
   HasConsumptionList(consumption_list) {

}

void CountryIEPolicy1::simulation(std::vector<ProductIE>& products, double random_precision) const {
   //EXPORT: all products from production list;
   //IMPORT: all products from consumption list;

   auto production_list = getProductionList();

   for(auto& productuin_item : production_list) {
       auto it = std::find_if(products.begin(), products.end(), [&](const ProductIE& product){return productuin_item.first == product.getProduct();});

       int real_production = Random::randomDouble(productuin_item.second * (1 - random_precision),
                                                  productuin_item.second * (1 + random_precision));
       it->incExport(real_production);
   }

   for (auto& consumption_item : _consumption_list) {
       auto it = std::find_if(products.begin(), products.end(), [&](const ProductIE& product){return consumption_item.first == product.getProduct();});

       int real_consumption = Random::randomDouble(consumption_item.second * (1 - random_precision),
                                                   consumption_item.second * (1 + random_precision));

       it->incImport(real_consumption);
   }
}

//======================================================

CountryIEPolicy2::CountryIEPolicy2(const std::string &name, const ProductionList &production_list,
                                   const ConsumptionList &consumption_list) :
   Country(name, production_list),
   HasConsumptionList(consumption_list) {

}

void CountryIEPolicy2::simulation(std::vector<ProductIE>& products, double random_precision) const {
   auto production_list = getProductionList();
   auto consumption_list = getConsumptionList();

   std::vector<int> produced_amount(production_list.size()); //i-th element corresponds to production_list[i];
   std::vector<int> consumed_amount(consumption_list.size()); //i-th element corresponds to consumption_list[i];

   for (unsigned i = 0; i < produced_amount.size(); i++)
      produced_amount[i] = Random::randomDouble(production_list[i].second * (1 - random_precision),
                                                production_list[i].second * (1 + random_precision));

   for (unsigned i = 0; i < consumed_amount.size(); i++) {
       consumed_amount[i] = Random::randomDouble(consumption_list[i].second * (1 - random_precision),
                                                   consumption_list[i].second * (1 + random_precision));
   }

   //removing produced and consumed items
   for (unsigned i = 0; i < production_list.size(); i++) {
          auto consumtion_item = std::find_if(consumption_list.begin(), consumption_list.end(),
                                              [&](const std::pair<std::shared_ptr<RawProduct>, unsigned>& element){ return element.first == production_list[i].first; });

          if (consumtion_item != consumption_list.end()) {
              auto j = std::distance(consumption_list.begin(), consumtion_item);

              if (consumed_amount[j] >= produced_amount[i]) {
                  consumed_amount[j] -= produced_amount[i];
                  produced_amount[i] = 0;
              } else {
                  produced_amount[i] -= consumed_amount[j];
                  consumed_amount[j] = 0;
              }
          }
    }

    //analysis of consumed items
    for (unsigned j = 0; j < consumption_list.size(); j++) {
        //if consumed item is raw
        if(consumption_list[j].first->getRawList().empty()) {
            auto production_item = std::find_if(production_list.begin(), production_list.end(),
                                                [&](const std::pair<std::shared_ptr<RawProduct>, unsigned>& element){ return consumption_list[j].first == element.first; });

            if (production_item != production_list.end()) {
                auto index = std::distance(production_list.begin(), production_item);
                produced_amount[index] -= consumed_amount[j];
            } else {
                auto it = std::find_if(products.begin(), products.end(), [&](const ProductIE& product){return consumption_list[j].first == product.getProduct();});
                it->incImport(consumed_amount[j]);
            }

            continue;
        }

        //if consumed item is not raw
        for(auto& item : consumption_list[j].first->getRawList()) {
            if (!item.first->getRawList().empty()) continue;

            auto production_item = std::find_if(production_list.begin(), production_list.end(),
                                                [&](const std::pair<std::shared_ptr<RawProduct>, unsigned>& element){ return element.first == item.first; });

            if (production_item != production_list.end()) {
                auto i = std::distance(production_list.begin(), production_item);

                produced_amount[i] -= item.second * consumed_amount[j];
            } else {
                auto it = std::find_if(products.begin(), products.end(), [&](const ProductIE& product){return item.first == product.getProduct();});
                it->incImport(item.second * consumed_amount[j]);
            }
        }
    }

    //analysis of produced items
    for (unsigned i = 0; i < production_list.size(); i++) {
      if (!production_list[i].first->getRawList().empty()) continue;

      auto it = std::find_if(products.begin(), products.end(), [&](const ProductIE& product){return production_list[i].first == product.getProduct();});

      if (produced_amount[i] >= 0) {
          it->incExport(produced_amount[i]);
      } else {
          it->incImport(abs(produced_amount[i]));
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

void CountryIEPolicy3::simulation(std::vector<ProductIE>& products, double random_precision) const {
    auto production_list = getProductionList();

    std::vector<int> produced_amount(production_list.size()); //i-th element corresponds to production_list[i];
    std::vector<int> consumed_amount(_consumption_volumes_list.size()); //i-th element corresponds to consumption_list[i];

    for (unsigned i = 0; i < produced_amount.size(); i++)
        produced_amount[i] = Random::randomDouble(production_list[i].second * (1 - random_precision),
                                                  production_list[i].second * (1 + random_precision));

    for (unsigned i = 0; i < consumed_amount.size(); i++)
        consumed_amount[i] = Random::randomDouble(_consumption_volumes_list[i] * (1 - random_precision),
                                                    _consumption_volumes_list[i] * (1 + random_precision));

    for (unsigned i = 0; i < production_list.size(); i++) {
        for (auto &item:production_list[i].first->getRawList()) {
            auto production_item = std::find_if(production_list.begin(), production_list.end(),
                                                [&](const std::pair<std::shared_ptr<RawProduct>, unsigned>& element){ return element.first == item.first; });

            if (production_item != production_list.end()) {
                auto index = std::distance(production_list.begin(), production_item);
                produced_amount[index] -= item.second * produced_amount[i];
            } else {
                auto it = std::find_if(products.begin(), products.end(), [&](const ProductIE& product){return item.first == product.getProduct();});
                it->incImport(item.second * produced_amount[i]);
            }
        }

        produced_amount[i] -= consumed_amount[i];
    }

    for (unsigned i = 0; i < production_list.size(); i++) {
        auto it = std::find_if(products.begin(), products.end(), [&](const ProductIE& product){return production_list[i].first == product.getProduct();});

        if (produced_amount[i] >= 0) {
            it->incExport(produced_amount[i]);
        } else {
            it->incImport(abs(produced_amount[i]));
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
