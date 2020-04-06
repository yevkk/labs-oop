#ifndef PRODUCT_H
#define PRODUCT_H

#include "random.h"

#include <string>
#include <vector>
#include <utility>
#include <memory>

class RawProduct;

/**
 * first - a pointer to raw product; second - amount of necessary 'fisrt';
 */
using RawListVector = std::vector<std::pair<std::shared_ptr<RawProduct>, unsigned>>;

/**
 * @brief : a product that can be used for production of other products
 */
class RawProduct {
private:
   std::string _name;
public:
   RawProduct();

   RawProduct(const std::string& name);

   std::string getName() const;

   virtual RawListVector getRawList() const;

   /**
    * @param product : an object to check
    * @return true if product is used to produce this
    */
   virtual bool isUsed(std::shared_ptr<RawProduct> product) const; // returns true if product is used to produce this
};

/**
 * @brief A product that is produced from other products
 * @note can be used for production of other products too
 */
class FinalProduct : public RawProduct {
private:
    /**
    * @brief A list of pointers to products that are required for preduction of this
    */
   RawListVector _raw_list;

public:
   /**
    * @param name
    * @param raw_list A list of pointers to products that are required for preduction of this
    */
   FinalProduct(const std::string& name, const RawListVector& raw_list);

   RawListVector getRawList() const override;

   /**
    * @param product : an object to check
    * @return true if product is used to produce this
    */
   bool isUsed(std::shared_ptr<RawProduct> product) const override;
};


/**
 * @return Either smart pointer to RawProduct or to FinalProduct
 * @param products : a list of products, that can be used for production new product
 */
std::shared_ptr<RawProduct> randomProduct(std::vector<std::shared_ptr<RawProduct>> products);

#endif // PRODUCT_H
