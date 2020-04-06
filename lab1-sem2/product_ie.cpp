#include "product_ie.h"

ProductIE::ProductIE() :
   _import(0),
   _export(0) {

}

ProductIE::ProductIE(std::shared_ptr<RawProduct> product) :
   _product(product),
   _import(0),
   _export(0) {

}

std::shared_ptr<RawProduct> ProductIE::getProduct() {
   return _product;
}

void ProductIE::setImport(int value) {
   _import = value;
}

void ProductIE::incImport(int value) {
   _import += value;
}

int ProductIE::getImport() {
   return _import;
}

void ProductIE::setExport(int value) {
   _export = value;
}

void ProductIE::incExport(int value) {
   _export += value;
}

int ProductIE::getExport() {
   return _export;
}

int ProductIE::balance() {
   return _export - _import;
}

void ProductIE::resetIE() {
   _export = _import = 0;
}

std::vector<std::shared_ptr<Country> > ProductIE::getImpCountries() {
   return _imp_countries;
}

std::vector<std::shared_ptr<Country> > ProductIE::getExpCountries() {
   return _exp_countries;
}
