#ifndef COUNTRY_H
#define COUNTRY_H

#include "product.h"
#include "product_ie.h"

#include <string>
#include <vector>

class ProductIE;

// first - a pointer to raw product; second - amount of average production per year;
using ProductionList = std::vector<std::pair<std::shared_ptr<RawProduct>, unsigned>>;

// first - a pointer to product; second - amount of average consumption per year;
using ConsumptionList = std::vector<std::pair<std::shared_ptr<RawProduct>, unsigned>>;

/**
 * @brief A country interface
 */
class Country {
private:
   std::string _name;

   ProductionList _production_list;

public:
   Country();

   /**
    * @param name
    * @param production_list : a list of products produced in this country
    */
   Country(const std::string& name, const ProductionList& production_list = ProductionList());

   std::string getName() const;

   ProductionList getProductionList() const;

   void setProductionList(const ProductionList& production_list);

   /**
    * @param products : a list of available products
    * @param random_precision : a precision for setting a real production/consumption values based on average
    */
   virtual void simulation(std::vector<ProductIE>& products, double random_precision) const = 0; //some actions according to export/import policy;
};

/**
 * @brief Country with policy type 1
 * @note EXPORT: all products from production list
   @note IMPORT: all products from consumption list
 */
class CountryIEPolicy1 : public Country {
private:
      ConsumptionList _consumption_list;

public:
   /**
    * @param name
    * @param production_list : a list of products produced in this country
    * @param consumption_list : a list of products consumpted by this country
    */
   CountryIEPolicy1(const std::string& name, const ProductionList& production_list = ProductionList(),
                    const ConsumptionList& consumption_list = ConsumptionList());

   ConsumptionList getConsumptionList() const;

   /**
    * @param products : a list of available products
    * @param random_precision : a precision for setting a real production/consumption values based on average
    */
   void simulation(std::vector<ProductIE>& products, double random_precision) const override;
};

/**
 * @brief Country with policy type 2
 * @note EXPORT: extra raw products from production list
   @note IMPORT: raw products for production products from consumption list and products from consumption list, that were not produced
 */
class CountryIEPolicy2 : public Country {
private:
   ConsumptionList _consumption_list;

public:
   /**
    * @param name
    * @param production_list : a list of products produced in this country
    * @param consumption_list : a list of products consumpted by this country
    */
   CountryIEPolicy2(const std::string& name, const ProductionList& production_list = ProductionList(),
                    const ConsumptionList& consumption_list = ConsumptionList());

   ConsumptionList getConsumptionList() const;

   /**
    * @param products : a list of available products
    * @param random_precision : a precision for setting a real production/consumption values based on average
    */
   void simulation(std::vector<ProductIE>& products, double random_precision) const override;
};

//;
//EXPORT:
//IMPORT: ;
/**
 * @brief Country with policy type 3
 * @note EXPORT: extra products from consumption list;
   @note IMPORT: all products required for production
   @note production list ~ consumption list (traditional products for this country)
 */
class CountryIEPolicy3 : public Country {
private:
   std::vector<unsigned> _consumption_volumes_list;

public:
   /**
    * @param name
    * @param production_list : a list of products produced in this country
    * @param consumption_volumes_list : volumes of consumption of products from production list
    */
   CountryIEPolicy3(const std::string& name, const ProductionList& production_list = ProductionList(),
                    const std::vector<unsigned>& consumption_volumes_list = std::vector<unsigned>());

   void setComsumptionVolumesList(const std::vector<unsigned>& new_list);

   /**
    * @param products : a list of available products
    * @param random_precision : a precision for setting a real production/consumption values based on average
    */
   void simulation(std::vector<ProductIE>& products, double random_precision) const override;
};

std::shared_ptr<Country> randomCountry(std::vector<std::shared_ptr<RawProduct>> products);

#endif // COUNTRY_H
