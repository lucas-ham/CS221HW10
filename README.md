### Traveling Salesperson Genetic Algorithm

#### Harrison Nicholls and Lucas Hamilton

##### Chromosome Class
- One protected function and data member were added to this class to help with calculate fitness
  - the function is called get_longest_distance() and computes the longest path between two cities through a search
  - this function is then called once and the result is multiplied by cities_ptr.size() - 1 to create a path longer than any other path
    - this is then saved as a protected data member called longest_
  - longest_ is used to compute the fitness of a given chromosome
    - a given chromosome has fitness longest_ - length, where the length it describes is length
    - this assures that shorter paths will have a higher fitness and longer paths a shorter get_fitness


##### Deme Class
- The constructor uses the chromosome constructor to make a population of random chromosomes
  - it also seeds the generator so it is consistent throughout
- get_best is a simple loop through all of the chromosomes to find the most fit using the chromosome fitness calculation
- select_parent works on the roulette wheel weighting system
  - a random value is selected, between 0 and the total fitness of all the Chromosomes
  - then the fitness of the chromosomes are added to this value until the entire sum exceeds the fitness of all the Chromosomes
    - depending on the random value picked, this will change which chromosome pushes the value over
    - this ensures there is randomness since any chromosome can be selected, but the chromosomes with higher fitness will be picked more often
  - the chromosome to exceed the total fitness is the parent selected
- compute_next_generation has an extra vector created where the children are added to while the next generation is being computed
  - they cannot be added to pop_ until the entire generation is completed to avoid the risk of a child and it's parents being in the same generation
  - the new generation replaces the old at the end of the function
