#include "product.h"

RawProduct::RawProduct()
{

}

RawProduct::RawProduct(const std::string& name): _name(name)
{

}

std::string RawProduct::getName() const {
    return _name;
}

RawListVector RawProduct::getRawList() const {
   return RawListVector();
}

bool RawProduct::isUsed(std::shared_ptr<RawProduct> product) const {
   return (this == product.get()) ? true : false;
}

FinalProduct::FinalProduct(const std::string& name, const RawListVector& raw_list):
   RawProduct(name),
   _raw_list(raw_list) {

}

RawListVector FinalProduct::getRawList() const {
   return _raw_list;
}

bool FinalProduct::isUsed(std::shared_ptr<RawProduct> product) const {
   if (this == product.get()) return true;

   for (auto &e:_raw_list)
      if (e.first->isUsed(product)) return true;

   return false;
}

std::shared_ptr<RawProduct> randomProduct(std::vector<std::shared_ptr<RawProduct>> products) {
   std::shared_ptr<RawProduct> res;
   //auto name = Random::randomString(Random::randomInt(4, 10));

   if (Random::randomDouble(0, 1) > 0.5 || products.empty()) {
      static unsigned raw_created = 1;
      std::string name = "raw product " + std::to_string(raw_created++);
      res = std::make_shared<RawProduct>(RawProduct(name));
   } else {
      unsigned raw_num = Random::randomInt(1, 5);
      RawListVector raw_list;

      for(unsigned i = 0; i < raw_num && i < products.size(); i++) {
         unsigned chosen_index = Random::randomInt(0, products.size() - 1);
         auto tmp = products[chosen_index];
         products.erase(products.begin() + chosen_index);
         raw_list.emplace_back(tmp, Random::randomInt(1, 15));
      }

      static unsigned final_created = 1;
      std::string name = "product " + std::to_string(final_created++);
      res = std::make_shared<FinalProduct>(FinalProduct(name, raw_list));
   }

   return res;
}

