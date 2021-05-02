/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "deme.hh"

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
: mut_rate_(mut_rate)
{
  while (pop_.size() < pop_size){
    auto next = new Chromosome(cities_ptr);
    pop_.push_back(next);
  }
  std::random_device rd;
  auto seed = rd();
  generator_.seed(seed);
  // Add your implementation here
}

// Clean up as necessary
Deme::~Deme()
{
  // Add your implementation here
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
  std::vector<Chromosome*> new_pop_;
  std::uniform_real_distribution<double> dist_maker(0.0, 1.0);

  while (new_pop_.size() < pop_.size()){
    auto p1 = select_parent();
    auto p2 = select_parent();              //get 2 random parents
    auto rand1 = dist_maker(generator_);
    if (rand1 < mut_rate_){
      p1->mutate();
    }
    auto rand2 = dist_maker(generator_);
    if (rand2 < mut_rate_){
      p2->mutate();
    }                                       //mutate parents if necessary

    auto next2 = p1->recombine(p2);
    new_pop_.push_back(next2.first);
    new_pop_.push_back(next2.second);       //add the new children to the new population
  }

  pop_.clear();
  pop_ = new_pop_;

  // Add your implementation here
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  auto best_fitness = 0;
  Chromosome* best_chromosome;

  for (auto chromosome : pop_){
    if (best_fitness < chromosome->get_fitness()){
      best_fitness = chromosome->get_fitness();
      best_chromosome = chromosome;
    }
  }
  return best_chromosome;;
  // Add your implementation here
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
  auto total_fitness = 0;
  for (auto chromosome : pop_){
    total_fitness += chromosome->get_fitness();
  }
  std::uniform_real_distribution<int> dist(0, pop_.size());
  auto parital_sum = dist(generator_);                                //use generator_ to get a random int between 0 and pop_size()

  for (auto i = 0; i < pop_.size(); i++){
    parital_sum += pop_[i]->get_fitness();
    if (total_fitness < parital_sum){
      return pop_[i];
    }
  }
  return pop_[pop_.size() - 1];
  // Add your implementation here
}
