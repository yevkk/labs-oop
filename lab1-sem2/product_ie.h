#ifndef PRODUCT_IE_H
#define PRODUCT_IE_H

#include "product.h"
#include "country.h"

#include <vector>
#include <memory>

class Country;

/**
 * @brief Corresponds to some RawProduct object, represent its import/export usage
 */
class ProductIE {
private:
   std::shared_ptr<RawProduct> _product;

   uint64_t _import;

   uint64_t _export;

   /**
    * @brief A list of countries that import product
    */
   std::vector<std::shared_ptr<Country>> _imp_countries;

   /**
    * @brief A list of countries that export product
    */
   std::vector<std::shared_ptr<Country>> _exp_countries;

public:
   ProductIE();

   ProductIE(std::shared_ptr<RawProduct> product);

   std::shared_ptr<RawProduct> getProduct() const;

   void setImport(int value);

   void incImport(int value);

   int getImport() const;

   void setExport(int value);

   void incExport(int value);

   int getExport() const;

   /**
    * @return A balance of product
    * @note balance of product is a difference between its export and import values
    */
   int balance() const;

   /**
    * @brief Sets all values to 0
    */
   void resetIE();

   std::vector<std::shared_ptr<Country>> getImpCountries();

   std::vector<std::shared_ptr<Country>> getExpCountries();
};

#endif // PRODUCT_IE_H
