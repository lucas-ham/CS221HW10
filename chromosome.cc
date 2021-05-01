/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>

#include "chromosome.hh"

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand()),
    longest_(get_longest_distance() * (cities_ptr->size() -1))
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  srand (time(NULL));

  auto first = rand() % cities_ptr_->size(); //May change later
  auto second = rand() % cities_ptr_->size();
  std::swap(order_[first],order_[second]);
  // Add your implementation here

  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());

  srand(time(NULL));
  auto start = rand() % cities_ptr_->size();
  auto end = rand() % cities_ptr_->size();                    //create a random swtich interval
  std::pair <Chromosome*, Chromosome*> outpair;                   //initialize the pair

  outpair.first = create_crossover_child(this, other, start, end);
  outpair.second = create_crossover_child(other, this, start, end);  //create chromosomes using create_crossover_child

  return outpair;


  // Add your implementation here
}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
      }
      assert(j < p2->order_.size());
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
  auto path_distance = cities_ptr_->total_path_distance(order_);
  return longest_ - path_distance;
  // Add your implementation here
}

double Chromosome::get_longest_distance() {
  auto cities_size = cities_ptr_->size();
  auto longest = 0;
  for (int i = 0; i < cities_size; i++){
    for (int j = i + 1; j < cities_size; j++){
      const auto dx = cities_ptr_->get_coords_by_index(i).first - cities_ptr_->get_coords_by_index(j).first;
      const auto dy = cities_ptr_->get_coords_by_index(i).second - cities_ptr_->get_coords_by_index(j).second;
      auto current = std::hypot(dx, dy);
      if (longest < current){
        longest = current;
      }
    }
  }
  return longest;
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple
bool
Chromosome::is_valid() const
{
  std::vector<bool> here(cities_ptr_->size(), 0);
  for (auto index : order_ ){
    if (here[index]){
      break;
    }
    here[index] = 1;
  }
  for (bool value: here){
    if (!value){
      return false;
    }
  }
  return true;
  // Add your implementation here
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
  for (int i = begin; i < end; i++){
    if (order_[i] == value){
      return true;
    }
  }
  return false;
  // Add your implementation here
}
