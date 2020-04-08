# Lab 1
### Custom classes
#### Products
- `class RawProduct` - a raw product which is used for producing other products.
- `class FinalProduct : public RawProduct` - a product produced from some amount of raw products or other derived products.

Each product has name property, non-raw products have a vector of pairs of required products and their required amount for producing.
- `class ProductIE` is used for running simulation and storing information about import and export of some `RawProduct`.

#### Countries
###### Interfaces
- `Country` - base class inherited by all other country classes;
- `HasConsumptionList` - interface for countries that product and consumpt different sets of products;
###### Classes
There is a base abstract `class Country` with `name` and `production_list` properties (production list is represented by a vector of pairs of product and average production value).
Different type of countries have different policies on producing, importing and exporting products:
- `CountryIEPolicy1` - export: all produced raw products; import: all required consumption products;
- `CountryIEPolicy2` - tries to produce required products; export: all extra raw products; import: everything, required for production consumption products;
- `CountryIEPolicy3` - chooses some 'traditional' products for production and consumption; export: extra produced products; import: all required for production consumption products;

### Simulation
Simulation creates a sequence of `ProductIE` lists, each list represent a single year. 

### Interface (QT)
- A tree of created products (children elements are raw for creating a product)
- Product info panel - when a product in tree is selected, panel dislpays name, list of raw products, list of products, that require current product for producing and a list of countries, where current product is being produced.
- A button for creating new random product.
- Menu:
	- Reset everything
	- Add random countries (an input dialog will ask about countries number)
	- Run simulation
- Page with displaying import export and balance (export - import) data on plot (opens, when simulation is run) - *NOW SHOWS ONLY ONE PRODUCT DATA*

### Plots lib
For working with plots a [QCustomPlot](https://www.qcustomplot.com "Project website") library for QT is used.

### Unit tests
Unit tests are provided for countries' policies simulations. [Catch 2](https://github.com/catchorg/Catch2 "Project github page") is used.
To run unit tests you should change line 3 i *.pro* file as it is shown lower:
```qmake
CONFIG += testsConfig
```

### UML
There are UML diagrams created by [Visual Paradigm Community Edition](https://www.visual-paradigm.com/download/community.jsp "Website"):
- [X] Class diagram
- [X] Use-case diagram
- [X] Object diagram for prducts' relations, created in tests
- [X] Activity diagram for `CountryIEPolicy1::simulation()`
- [ ] Activity diagram for `CountryIEPolicy2::simulation()`
- [ ] Activity diagram for `CountryIEPolicy3::simulation()`

### Documentation
Key points of project are documented using [Doxygen](http://www.doxygen.nl/ "Website")-style comment blocks

---
### Change log:
- [X] Added tests and build config for running tests;
- [X] Fixed random bounds to prevent type overflow in demo;
- [X] Fixed logic in `CountryIEPolicy2::simulation()` (in previous: imported product, while they where produced);
- [X] Marked methods `const` where it's logically correct;
- [X] Added interface `HasConsumptionList` to avoid massive code duplicates in country types with different production and consumption lists;
- [X] Multiple loops replaced with stl functions;
- [X] Added documentation;
- [ ] Added UML diagrams;
	
