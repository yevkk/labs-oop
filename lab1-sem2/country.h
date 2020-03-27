#ifndef COUNTRY_H
#define COUNTRY_H

#include "product.h"
#include "product_ie.h"

#include <string>
#include <vector>

constexpr double RANDOM_PRECISION = 0.3;

class ProductIE;

// first - a pointer to raw product; second - amount of average production per year;
using ProductionList = std::vector<std::pair<std::shared_ptr<RawProduct>, unsigned>>;

// first - a pointer to product; second - amount of average consumption per year;
using ConsumptionList = std::vector<std::pair<std::shared_ptr<RawProduct>, unsigned>>;

class Country {
private:
   std::string _name;

   ProductionList _production_list;

public:
   Country();

   Country(const std::string& name, const ProductionList& production_list = ProductionList());

   std::string getName();

   ProductionList getProductionList();

   void setProductionList(const ProductionList& production_list);

   virtual void simulation(std::vector<ProductIE>& products, double random_precision = RANDOM_PRECISION) = 0; //some actions according to export/import policy;
};

//EXPORT: all products from production list;
//IMPORT: all products from consumption list;
class CountryIEPolicy1 : public Country {
private:
      ConsumptionList _consumption_list;

public:
   CountryIEPolicy1(const std::string& name, const ProductionList& production_list = ProductionList(),
                    const ConsumptionList& consumption_list = ConsumptionList());

   ConsumptionList getConsumptionList();

   void simulation(std::vector<ProductIE>& products, double random_precision = RANDOM_PRECISION) override;
};

//EXPORT: extra products from production list;
//IMPORT: raw products for production products from consumption list;
class CountryIEPolicy2 : public Country {
private:
   ConsumptionList _consumption_list;

public:
   CountryIEPolicy2(const std::string& name, const ProductionList& production_list = ProductionList(),
                    const ConsumptionList& consumption_list = ConsumptionList());

   ConsumptionList getConsumptionList();

   void simulation(std::vector<ProductIE>& products, double random_precision = RANDOM_PRECISION) override;
};

//production list ~ consumption list (traditional products for this country);
//EXPORT: all products required for production;
//IMPORT: extra products from consumption list;
class CountryIEPolicy3 : public Country {
private:
   std::vector<unsigned> _consumption_volumes_list;

public:
   CountryIEPolicy3(const std::string& name, const ProductionList& production_list = ProductionList(),
                    const std::vector<unsigned>& consumption_volumes_list = std::vector<unsigned>());

   void setComsumptionVolumesList(const std::vector<unsigned>& new_list);

   void simulation(std::vector<ProductIE>& products, double random_precision = RANDOM_PRECISION) override;
};

//prodcution list ~ consumption list (last year most popular products);
//EXPORT: all products required for production;
//IMPORT: extra products from consumption list;
class CountryIEPolicy4 : public CountryIEPolicy3 {
public:
   CountryIEPolicy4(const std::string& name, const ProductionList& production_list = ProductionList(),
                    const std::vector<unsigned>& consumption_volumes_list = std::vector<unsigned>());

   void simulation(std::vector<ProductIE>& products, std::vector<ProductIE> products_previous);
};

std::shared_ptr<Country> randomCountry(std::vector<std::shared_ptr<RawProduct>> products);

#endif // COUNTRY_H
