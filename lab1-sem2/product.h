#ifndef PRODUCT_H
#define PRODUCT_H

#include "random.h"

#include <string>
#include <vector>
#include <utility>
#include <memory>

class RawProduct;

// first - a pointer to raw product; second - amount of necessary 'fisrt';
using RawListVector = std::vector<std::pair<std::shared_ptr<RawProduct>, unsigned>>;

class RawProduct {
private:
   std::string _name;
public:
   RawProduct();

   RawProduct(const std::string& name);

   std::string getName();

   virtual RawListVector getRawList();

   virtual bool isUsed(std::shared_ptr<RawProduct> product); // returns true if product is used to produce this
};

class FinalProduct : public RawProduct {
private:
   RawListVector _raw_list;

public:
   FinalProduct(const std::string& name, const RawListVector& raw_list);

   RawListVector getRawList() override;

   bool isUsed(std::shared_ptr<RawProduct> product) override;
};

std::shared_ptr<RawProduct> randomProduct(std::vector<std::shared_ptr<RawProduct>> products);

#endif // PRODUCT_H
