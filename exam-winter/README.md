# Winter Exam
![Santa](https://github.com/yevkk/module-oop/blob/master/exam-winter/icon.ico "Santa")
### Custom classes
##### Products
- `class RawProduct` - a raw product which is used for producing other products.
- `class FinalProduct : public RawProduct` - a product produced from some amount of raw products or other derived products.

Each product has name property, non-raw products have a vector of pairs of required products and their required amount for producing.
- `class ProductIE` is used for running simulation and storing information about import and export of some `RawProduct`.

##### Countries
There is a base abstract `class Country` with `name` and `production_list` properties (production list is represented by a vector of pairs of product and average production value).
Different type of countries have different policies on producing, importing and exporting products:
- `CountryIEPolicy1` - export: all produced raw products; import: all required consumption products;
- `CountryIEPolicy2` - tries to produce required products; export: all extra raw products; import: everything, required for production consumption products;
- `CountryIEPolicy3` - chooses some 'traditional' products for production and consumption; export: extra produced products; import: all required for production consumption products;
- `CountryIEPolicy4` - chooses 'most popular' product (according to previous year stats) export/import: same as previous;

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
For working with plots a [QCustomPlot](https://www.qcustomplot.com "QCustomPlot") library for QT is used.

### Timetracking
A timetracking file is added to repository
