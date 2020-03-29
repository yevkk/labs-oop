#include "catch.hpp"
#include "../product.h"
#include "../product_ie.h"
#include "../country.h"

#include <memory>
#include <vector>

TEST_CASE("sting countries' policy", "[Policy test]"){
    std::shared_ptr<RawProduct> wood = std::make_shared<RawProduct>(RawProduct("Wood"));
    std::shared_ptr<RawProduct> wool = std::make_shared<RawProduct>(RawProduct("Wool"));
    std::shared_ptr<RawProduct> iron = std::make_shared<RawProduct>(RawProduct("Iron"));
    std::shared_ptr<RawProduct> sand = std::make_shared<RawProduct>(RawProduct("Sand"));
    std::shared_ptr<RawProduct> coal = std::make_shared<RawProduct>(RawProduct("Coal"));
    std::shared_ptr<FinalProduct> chest = std::make_shared<FinalProduct>(FinalProduct("Chest", {{wood, 8}}));
    std::shared_ptr<FinalProduct> stick = std::make_shared<FinalProduct>(FinalProduct("Stick", {{wood, 2}}));
    std::shared_ptr<FinalProduct> bed = std::make_shared<FinalProduct>(FinalProduct("Bed", {{wood, 3}, {wool, 3}}));
    std::shared_ptr<FinalProduct> shovel = std::make_shared<FinalProduct>(FinalProduct("Shovel", {{iron, 1}, {stick, 2}}));
    std::shared_ptr<FinalProduct> torch = std::make_shared<FinalProduct>(FinalProduct("Torch", {{coal, 1}, {stick, 2}}));
    std::shared_ptr<FinalProduct> glass = std::make_shared<FinalProduct>(FinalProduct("Glass", {{sand, 3}}));
    std::shared_ptr<FinalProduct> string = std::make_shared<FinalProduct>(FinalProduct("String", {{wool, 1}}));
    std::shared_ptr<FinalProduct> fishing_rod = std::make_shared<FinalProduct>(FinalProduct("Fishing rod", {{string, 2}, {stick, 3}}));

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
        SECTION("simple") {
            CountryIEPolicy1 country("TestCountry 1", {{wood, 3}, {sand, 5}, {stick, 6}, {torch, 7}}, {{stick, 3}, {torch, 6}, {fishing_rod, 2}});

            country.simulation(productsIE, 0);

            REQUIRE(productsIE[0].getExport() == 3);
            REQUIRE(productsIE[2].getExport() == 0);
            REQUIRE(productsIE[3].getExport() == 5);
            REQUIRE(productsIE[5].getExport() == 0);
            REQUIRE(productsIE[6].getExport() == 6);
            REQUIRE(productsIE[8].getExport() == 0);
            REQUIRE(productsIE[9].getExport() == 7);

            REQUIRE(productsIE[6].getImport() == 3);
            REQUIRE(productsIE[8].getImport() == 0);
            REQUIRE(productsIE[9].getImport() == 6);
            REQUIRE(productsIE[10].getImport() == 0);
            REQUIRE(productsIE[12].getImport() == 2);

            REQUIRE(productsIE[0].balance() == 3);
            REQUIRE(productsIE[3].balance() == 5);
            REQUIRE(productsIE[6].balance() == 3);
            REQUIRE(productsIE[9].balance() == 1);
            REQUIRE(productsIE[12].balance() == -2);
        }

        SECTION("normal") {
            CountryIEPolicy1 country_1("TestCountry 1", {{wood, 3}, {coal, 4}, {stick, 10}, {shovel, 5}, {string, 7}}, {{wool, 10}, {bed, 12}, {glass, 5}});
            CountryIEPolicy1 country_2("TestCountry 2", {{iron, 10}, {sand, 6}, {chest, 4}}, {{wood, 9}, {iron, 4}, {chest, 3}, {bed, 10}, {shovel, 4}, {torch, 3}, {string, 10}, {fishing_rod, 7}});
            CountryIEPolicy1 country_3("TestCountry 3", {{coal, 4}, {stick, 2}, {torch, 15}, {glass, 11}}, {{sand, 15}, {coal, 12}, {chest, 8}, {torch, 13}, {string, 1}, {fishing_rod, 4}});
            CountryIEPolicy1 country_4("TestCountry 4", {{wood, 10}, {iron, 4}, {stick, 10}, {shovel, 3}, {torch, 1}, {glass, 17}, {string, 2}, {fishing_rod, 5}}, {{wool, 4}, {sand, 1}, {coal, 3}});

            country_1.simulation(productsIE, 0);
            country_2.simulation(productsIE, 0);
            country_3.simulation(productsIE, 0);
            country_4.simulation(productsIE, 0);

            REQUIRE(productsIE[0].getExport() == 13);
            REQUIRE(productsIE[1].getExport() == 0);
            REQUIRE(productsIE[2].getExport() == 14);
            REQUIRE(productsIE[3].getExport() == 6);
            REQUIRE(productsIE[4].getExport() == 8);
            REQUIRE(productsIE[5].getExport() == 4);
            REQUIRE(productsIE[6].getExport() == 22);
            REQUIRE(productsIE[7].getExport() == 0);
            REQUIRE(productsIE[8].getExport() == 8);
            REQUIRE(productsIE[9].getExport() == 16);
            REQUIRE(productsIE[10].getExport() == 28);
            REQUIRE(productsIE[11].getExport() == 9);
            REQUIRE(productsIE[12].getExport() == 5);

            REQUIRE(productsIE[0].getImport() == 9);
            REQUIRE(productsIE[1].getImport() == 14);
            REQUIRE(productsIE[2].getImport() == 4);
            REQUIRE(productsIE[3].getImport() == 16);
            REQUIRE(productsIE[4].getImport() == 15);
            REQUIRE(productsIE[5].getImport() == 11);
            REQUIRE(productsIE[6].getImport() == 0);
            REQUIRE(productsIE[7].getImport() == 22);
            REQUIRE(productsIE[8].getImport() == 4);
            REQUIRE(productsIE[9].getImport() == 16);
            REQUIRE(productsIE[10].getImport() == 5);
            REQUIRE(productsIE[11].getImport() == 11);
            REQUIRE(productsIE[12].getImport() == 11);

            REQUIRE(productsIE[0].balance() == 4);
            REQUIRE(productsIE[1].balance() == -14);
            REQUIRE(productsIE[2].balance() == 10);
            REQUIRE(productsIE[3].balance() == -10);
            REQUIRE(productsIE[4].balance() == -7);
            REQUIRE(productsIE[5].balance() == -7);
            REQUIRE(productsIE[6].balance() == 22);
            REQUIRE(productsIE[7].balance() == -22);
            REQUIRE(productsIE[8].balance() == 4);
            REQUIRE(productsIE[9].balance() == 0);
            REQUIRE(productsIE[10].balance() == 23);
            REQUIRE(productsIE[11].balance() == -2);
            REQUIRE(productsIE[12].balance() == -6 );
        }

        SECTION("complicated") {

        }
    }

    SECTION("policy type - 2") {

    }

    SECTION("policy type - 3") {

    }
}
