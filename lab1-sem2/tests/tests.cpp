#include "catch.hpp"
#include "../product_ie.h"
#include "../country.h"
#include "../simulation.h"

#include <memory>
#include <vector>

void checkExport(std::vector<ProductIE> products, const std::vector<unsigned>& values) {
    for (unsigned i = 0; i < values.size(); i++) {
        REQUIRE(products[i].getExport() == values[i]);
    }
}

void checkImport(std::vector<ProductIE> products, const std::vector<unsigned>& values) {
    for (unsigned i = 0; i < values.size(); i++) {
        REQUIRE(products[i].getImport() == values[i]);
    }
}

void checkBalance(std::vector<ProductIE> products, const std::vector<int>& values) {
    for (unsigned i = 0; i < values.size(); i++) {
        REQUIRE(products[i].balance() == values[i]);
    }
}

TEST_CASE("sting countries' policy", "[Policy test]"){
    std::shared_ptr<RawProduct> wood = std::make_shared<RawProduct>(RawProduct{"Wood"});
    std::shared_ptr<RawProduct> wool = std::make_shared<RawProduct>(RawProduct{"Wool"});
    std::shared_ptr<RawProduct> iron = std::make_shared<RawProduct>(RawProduct{"Iron"});
    std::shared_ptr<RawProduct> sand = std::make_shared<RawProduct>(RawProduct{"Sand"});
    std::shared_ptr<RawProduct> coal = std::make_shared<RawProduct>(RawProduct{"Coal"});
    std::shared_ptr<FinalProduct> chest = std::make_shared<FinalProduct>(FinalProduct{"Chest", {{wood, 8}}});
    std::shared_ptr<FinalProduct> stick = std::make_shared<FinalProduct>(FinalProduct{"Stick", {{wood, 2}}});
    std::shared_ptr<FinalProduct> bed = std::make_shared<FinalProduct>(FinalProduct{"Bed", {{wood, 3}, {wool, 3}}});
    std::shared_ptr<FinalProduct> shovel = std::make_shared<FinalProduct>(FinalProduct{"Shovel", {{iron, 1}, {stick, 2}}});
    std::shared_ptr<FinalProduct> torch = std::make_shared<FinalProduct>(FinalProduct{"Torch", {{coal, 1}, {stick, 1}}});
    std::shared_ptr<FinalProduct> glass = std::make_shared<FinalProduct>(FinalProduct{"Glass", {{sand, 3}}});
    std::shared_ptr<FinalProduct> string = std::make_shared<FinalProduct>(FinalProduct{"String", {{wool, 1}}});
    std::shared_ptr<FinalProduct> fishing_rod = std::make_shared<FinalProduct>(FinalProduct{"Fishing rod", {{string, 2}, {stick, 3}}});

    std::vector<std::shared_ptr<RawProduct>> products = {wood, wool, iron, sand, coal, chest, stick, bed, shovel, torch, glass, string, fishing_rod};

    std::vector<ProductIE> productsIE;
    for (auto &product:products)
       productsIE.emplace_back(product);

    /*
     * 0 - Wood
     * 1 - Wool
     * 2 - Iron
     * 3 - Sand
     * 4 - Coal
     * 5 - Chest
     * 6 - Stick
     * 7 - Bed
     * 8 - Shovel
     * 9 - Torch
     * 10 - Glass
     * 11 - String
     * 12 - Fishing rod
     */

    SECTION("policy type - 1") {
        SECTION("single country") {
            CountryIEPolicy1 country{"TestCountry 1", {{wood, 3}, {sand, 5}, {stick, 6}, {torch, 7}}, {{stick, 3}, {torch, 6}, {fishing_rod, 2}}};

            country.simulation(productsIE, 0);

            checkExport(productsIE, {3, 0, 0, 5, 0, 0, 6, 0, 0, 7, 0, 0, 0});
            checkImport(productsIE, {0, 0, 0, 0, 0, 0, 3, 0, 0, 6, 0, 0, 2});
            checkBalance(productsIE, {3, 0, 0, 5, 0, 0, 3, 0, 0, 1, 0, 0, -2});
        }

        SECTION("a couple of countries") {
            CountryIEPolicy1 country_1{"TestCountry 1", {{wood, 3}, {coal, 4}, {stick, 10}, {shovel, 5}, {string, 7}}, {{wool, 10}, {bed, 12}, {glass, 5}}};
            CountryIEPolicy1 country_2{"TestCountry 2", {{iron, 10}, {sand, 6}, {chest, 4}}, {{wood, 9}, {iron, 4}, {chest, 3}, {bed, 10}, {shovel, 4}, {torch, 3}, {string, 10}, {fishing_rod, 7}}};
            CountryIEPolicy1 country_3{"TestCountry 3", {{coal, 4}, {stick, 2}, {torch, 15}, {glass, 11}}, {{sand, 15}, {coal, 12}, {chest, 8}, {torch, 13}, {string, 1}, {fishing_rod, 4}}};
            CountryIEPolicy1 country_4{"TestCountry 4", {{wood, 10}, {iron, 4}, {stick, 10}, {shovel, 3}, {torch, 1}, {glass, 17}, {string, 2}, {fishing_rod, 5}}, {{wool, 4}, {sand, 1}, {coal, 3}}};

            country_1.simulation(productsIE, 0);
            country_2.simulation(productsIE, 0);
            country_3.simulation(productsIE, 0);
            country_4.simulation(productsIE, 0);

            checkExport(productsIE, {13, 0, 14, 6, 8, 4, 22, 0, 8, 16, 28, 9, 5});
            checkImport(productsIE, {9, 14, 4, 16, 15, 11, 0, 22, 4, 16, 5, 11, 11});
            checkBalance(productsIE, {4, -14, 10, -10, -7, -7, 22, -22, 4, 0, 23, -2, -6});
        }

        SECTION("several years") {
            std::shared_ptr<CountryIEPolicy1> country_1 = std::make_shared<CountryIEPolicy1>(CountryIEPolicy1{"TestCountry 1", {{wool, 10}, {sand, 4}, {chest, 7}, {bed, 9}, {string, 15}}, {{wood, 10}, {wool, 4}, {stick, 11}, {glass, 8}, {torch, 2}, {fishing_rod, 2}, {string, 4}}});
            std::shared_ptr<CountryIEPolicy1> country_2 = std::make_shared<CountryIEPolicy1>(CountryIEPolicy1{"TestCountry 2", {{iron, 20}, {coal, 10}, {shovel, 4}, {glass, 3}, {torch, 17}, {string, 3}}, {{sand, 8}, {chest, 11}, {stick, 13}}});
            std::shared_ptr<CountryIEPolicy1> country_3 = std::make_shared<CountryIEPolicy1>(CountryIEPolicy1{"TestCountry 3", {}, {{wood, 21}, {stick, 8}, {shovel, 3}, {torch, 13}, {string, 9}, {fishing_rod, 4}}});
            std::shared_ptr<CountryIEPolicy1> country_4 = std::make_shared<CountryIEPolicy1>(CountryIEPolicy1{"TestCountry 4", {{wood, 40}, {fishing_rod, 10}}, {{wool, 6}, {sand, 12}, {bed, 17}, {torch, 3}, {coal, 11}, {string, 6}, {glass, 3}, {stick, 2}, {fishing_rod, 1}}});

            Simulation simulation(2000, 3, products, {country_1, country_2, country_3, country_4}, 0);

            //real production and consumption volumes are equal to average value because random_precision = 0;
            for(auto& year: simulation.getData()) {
                checkExport(year, {40, 10, 20, 4, 10, 7, 0, 9, 4, 17, 3, 18, 10});
                checkImport(year, {31, 10, 0, 20, 11, 11, 34, 17, 3, 18, 11, 19, 7});
                checkBalance(year, {9, 0, 20, -16, -1, -4, -34, -8, 1, -1, -8, -1, 3});
            }


        }
    }

    SECTION("policy type - 2") {
        SECTION("single country") {
            CountryIEPolicy2 country{"TestCountry 1", {{wood, 20}, {iron, 10}, {chest, 5}, {bed, 8}, {string, 3}, {fishing_rod, 1}}, {{sand, 5}, {stick, 15}, {shovel, 1}, {string, 1}}};

            country.simulation(productsIE, 0);

            checkExport(productsIE, {0, 0, 9, 0, 0});
            checkImport(productsIE, {10, 0, 0, 5, 0});
            checkBalance(productsIE, {-10, 0, 9, -5, 0});
        }

        SECTION("a couple of countries") {
            CountryIEPolicy2 country_1{"TestCountry 1", {{wood, 10}, {iron, 7}, {chest, 9}, {bed, 11}, {glass, 3}, {string, 5}}, {{iron, 4}, {sand, 3}, {chest, 10}, {shovel, 8}, {string ,7}}};
            CountryIEPolicy2 country_2{"TestCountry 2", {{wool, 7}, {sand, 8}, {chest, 3}, {glass, 3}}, {{iron, 4}, {bed, 2}, {glass, 4}, {fishing_rod, 1}}};
            CountryIEPolicy2 country_3{"TestCountry 3", {{wool, 6}, {coal, 3}, {torch, 18}}, {{stick, 10}, {shovel, 5}, {torch, 11}, {string, 3}}};
            CountryIEPolicy2 country_4{"TestCountry 4", {{wood, 20}, {iron, 10}, {chest, 5}, {bed, 8}, {string, 3}, {fishing_rod, 1}}, {{sand, 5}, {stick, 15}, {shovel, 1}, {string, 1}}};

            country_1.simulation(productsIE, 0);
            country_2.simulation(productsIE, 0);
            country_3.simulation(productsIE, 0);
            country_4.simulation(productsIE, 0);

            checkExport(productsIE, {2, 4, 9, 5, 3});
            checkImport(productsIE, {36, 2, 14, 8, 0});
            checkBalance(productsIE, {-34, 2, -5, -3, 3});
        }

        SECTION("several years") {
            std::shared_ptr<CountryIEPolicy2> country_1 = std::make_shared<CountryIEPolicy2>(CountryIEPolicy2{"TestCountry 1", {{wool, 7}, {sand, 8}, {chest, 3}, {glass, 3}}, {{iron, 4}, {bed, 2}, {glass, 4}, {fishing_rod, 1}}});
            std::shared_ptr<CountryIEPolicy2> country_2 = std::make_shared<CountryIEPolicy2>(CountryIEPolicy2{"TestCountry 2", {{wool, 6}, {coal, 3}, {torch, 18}}, {{stick, 10}, {shovel, 5}, {torch, 11}, {string, 3}}});
            std::shared_ptr<CountryIEPolicy2> country_3 = std::make_shared<CountryIEPolicy2>(CountryIEPolicy2{"TestCountry 3", {{wood, 20}, {iron, 10}, {chest, 5}, {bed, 8}, {string, 3}, {fishing_rod, 1}}, {{sand, 5}, {stick, 15}, {shovel, 1}, {string, 1}}});

            Simulation simulation(2000, 4, products, {country_1, country_2, country_3}, 0);

            //real production and consumption volumes are equal to average value because random_precision = 0;
            for (auto& year : simulation.getData()) {
                checkExport(year, {0, 4, 9, 5, 3});
                checkImport(year, {36, 0, 9, 5, 0});
                checkBalance(year, {-36, 4, 0, 0, 3});
            }
        }
    }

    SECTION("policy type - 3") {
        SECTION("single country") {
            CountryIEPolicy3 country{"TestCountry 1", {{wool, 3}, {sand, 10}, {shovel, 1}, {glass, 3}, {fishing_rod, 7}}, {0, 5, 4, 0, 5}};

            country.simulation(productsIE, 0);

            checkExport(productsIE, {0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 2});
            checkImport(productsIE, {0, 0, 1, 4, 0, 0, 23, 0, 3, 0, 0, 14, 0});
            checkBalance(productsIE, {0, 3, -1, -4, 0, 0, -23, 0, -3, 0, 3, -14, 2});
        }

        SECTION("a couple of countries") {
            CountryIEPolicy3 country_1{"TestCountry 1", {{wood, 7}, {iron, 3}, {chest, 5}, {bed, 4}, {torch, 0}, {string, 1}}, {0, 2, 1, 0, 3, 4}};
            CountryIEPolicy3 country_2{"TestCountry 2", {{iron, 10}, {coal, 15}, {stick, 7}, {shovel, 4}, {torch, 6}, {fishing_rod, 2}}, {5, 20, 4, 0, 0, 3}};
            CountryIEPolicy3 country_3{"TestCountry 3", {{wool, 11}, {sand, 3}, {stick, 5}, {glass, 4}}, {20, 0, 6, 7}};

            country_1.simulation(productsIE, 0);
            country_2.simulation(productsIE, 0);
            country_3.simulation(productsIE, 0);

            checkExport(productsIE, {0, 0, 2, 0, 0, 4, 0, 4, 4, 6, 0, 0, 0});
            checkImport(productsIE, {69, 22, 0, 9, 11, 0, 18, 0, 0, 3, 3, 7, 1});
            checkBalance(productsIE, {-69, -22, 2, -9, -11, 4, -18, 4, 4, 3, -3, -7, -1});
        }

        SECTION("several years") {
            std::shared_ptr<CountryIEPolicy3> country_1 = std::make_shared<CountryIEPolicy3>(CountryIEPolicy3{"TestCountry 1", {{wool, 3}, {sand, 10}, {shovel, 1}, {glass, 3}, {fishing_rod, 7}}, {0, 5, 4, 0, 5}});
            std::shared_ptr<CountryIEPolicy3> country_2 = std::make_shared<CountryIEPolicy3>(CountryIEPolicy3{"TestCountry 2", {{iron, 10}, {coal, 15}, {stick, 7}, {shovel, 4}, {torch, 6}, {fishing_rod, 2}}, {5, 20, 4, 0, 0, 3}});
            std::shared_ptr<CountryIEPolicy3> country_3 = std::make_shared<CountryIEPolicy3>(CountryIEPolicy3{"TestCountry 3", {{iron, 10}, {coal, 12}, {stick, 4}, {shovel, 3}, {string, 5}, {fishing_rod, 1}}, {0, 0, 0, 2, 2, 1}});

            Simulation simulation(2000, 4, products, {country_1, country_2, country_3}, 0);

            //real production and consumption volumes are equal to average value because random_precision = 0;
            for (auto& year : simulation.getData()) {
                checkExport(year, {0, 3, 8, 0, 12, 0, 0, 0, 5, 6, 3, 1, 2});
                checkImport(year , {22, 5, 1, 4, 11, 0, 45, 0, 3, 0, 0, 18, 1});
                checkBalance(year, {-22, -2, 7, -4, 1, 0, -45, 0, 2, 6, 3, -17, 1});
            }

        }
    }

    SECTION("all types") {

    }
}
