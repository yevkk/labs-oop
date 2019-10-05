# Lab1
### Data structures
Graphs based on adjacency structure and adjacency matrix, that can store a values of any data type in nodes and edges (`int`, `double`, `std::string`, `std::vector<>`).

**Methods implemented:**
- add node
- add edge
- find node by value
- delete node by index
- delete node by value
- delete edge
- check nodes adjacency
- check graph adjacency
- distance between two nodes (based on Floyd's algorithm)
- print

### Custom class
`Dice`:
A dice with face number N = 2, 4, 6, 8, 12, 20.
Every face has own probability of being dropped.

`DiceSet`:
A set of dices.

**Methods implemented:**
- [x] count all possible sums in set (min and max)
- [x] count probabilities for all possible sums in set
- [x] count expected value of set

**Operators overloaded:**
- [ ] << for `std::ostream` and `Dice`
- [ ] << for `std::ostream` and `DiceSet`
- [ ] comparing two sets by expected value of sum