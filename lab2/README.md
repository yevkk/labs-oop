# Lab 2/3
This project is a supplement to module-task ("hidden" in private repository). Briefly, there is a system, which consists of components, each component has a type, feature list and some possible defects. So, in general, our system has lists of types, features and defects.
This project's main task is to create valid lists of that data and create some config file (I've used *.xml*).

Project is created in **QT Creator 4.10.2**, using **QT version 5.12**

### Interface
- Menu Bar wish tabs **File**, **Go to** and **Preview**
- 3 pages with tables (`QTableView`) for displaying data (access by **Ctrl+1**, **Ctrl+2**, **Ctrl+3** or with **Go to** menu)
- 3 pages for editing data
- XML preview page **with xml syntax highlighter** (access with **Preview** menu)

#### Stylesheet
Dark orange stylesheet for this project was taken from [here](https://discourse.techart.online/t/release-qt-dark-orange-stylesheet/2287#post14381 "here").

### Model-View
This project includes 3 models: `FeatureModel`, `TypeModel` and `DefectModel` inherited from `QAbstractTableModel`, which store lists of features, types and defects respectively.

### Custom data types (classes)
**1) Feature:**
- Required number of components (`int components_required`)
- Reward for completing during one period (`int reward`)

**2) Type:**
- Name of type (`QString name`)
- Cost of components (`int cost`)
- List of features, which can be done by components of this type (`QVector<int> features`)
- List of subcomponents types (`QVector<int> sub_components`)

**3) Defect:**
- Chance of this defect to be detected (`double detect_chance`)
- List of components' types, where this defect is possible with a time and cost of repair (`QVector<QPair<int, QPair<int, int>>> component_types`)

All classes also have `int id` property;

### Basic features
You can create new, edit and delete objects on corresponding pages

### Working with files
With **File** menu or using keyboard shortcuts you can cerate new (**Ctrl+N**), save either as existing or as new file (**Ctrl+S**), save current as another file (**Ctrl+Shift+S**), open existing file (**Ctrl+O**) and close. If current file is not saved and about to close, then a dialog window will ask about saving current changes

### XML
Files to read and write must have **.xml** extension.
A parser, used to read and write files is [RapidXML](http://rapidxml.sourceforge.net/ "RapidXML") ([manual](http://rapidxml.sourceforge.net/manual.html "manual"), [download](https://sourceforge.net/projects/rapidxml/files/latest/download "download")). If project finds invalid data in opened file it will show error dialog.

### Data correctness
While reading .xml files to models or adding new objects to models all data is checked on correctness according to next rules:
- [x] Type cannot include two same features in features list
- [x] Type cannot include feature  in features list with id, higher than feature list objects number
- [x] Type cannot include type  in subcomponents' types list with id, higher than type list objects number
- [x] Defect cannot include two same types in subcomponents' types list
- [x] If a value is supposed to be a number, there is a check if it really is a number
- [x] When an object with some Id value is deleted, it is considered in other models
- [ ] Types relations can not be recursive ('A' must have subcomponent of type 'B', and 'B' of type 'A') - *NOT IMPLEMENTED* (I've tried to use graphs and cycle serch algorithm from lab1)

Edit pages dynamic modifying should not allow you to input invalid data


