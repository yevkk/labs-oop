#ifndef PRODUCT_IE_H
#define PRODUCT_IE_H

#include "product.h"
#include "country.h"

#include <vector>
#include <memory>

class Country;

class ProductIE {
private:
   std::shared_ptr<RawProduct> _product;

   uint64_t _import;

   uint64_t _export;

   std::vector<std::shared_ptr<Country>> _importing_countries_list;

   std::vector<std::shared_ptr<Country>> _exporting_countries_list;

public:
   ProductIE();

   ProductIE(std::shared_ptr<RawProduct> product);

   std::shared_ptr<RawProduct> getProduct();

   void setImport(int value);

   void incImport(int value);

   int getImport();

   void setExport(int value);

   void incExport(int value);

   int getExport();

   int balance();

   void resetIE();

   std::vector<std::shared_ptr<Country>> getImportingCountriesList();

   std::vector<std::shared_ptr<Country>> getExportingCountriesList();
};

#endif // PRODUCT_IE_H
