#include <iostream>
#include <grb/grb.hpp>
#include <iomanip>
#include <algorithm>

int main(int argc, char** argv) {
  grb::csr_matrix_impl_<float, int> matrix("data/chesapeake.mtx");

  grb::index_t shape = matrix.shape();

  printf("Loaded a %lu x %lu matrix with %lu nonzeros.\n",
         shape[0], shape[1], matrix.size());

  printf("Assigning each matrix element to 12.\n");
  for (auto elem : matrix) {
    elem = 12;
  }

  float total_sum = 0;
  for (auto elem : matrix) {
    float value = elem;
    grb::index_t idx = elem;
    total_sum += value;
  }

  printf("Total sum was %f, average value %f\n", total_sum, total_sum / matrix.size());

  std::vector<float> vec(matrix.size(), 2);
  std::copy(vec.begin(), vec.end(), matrix.begin());

  total_sum = 0;
  for (auto elem : matrix) {
    float value = elem;
    grb::index_t idx = elem;
    total_sum += value;
  }

  printf("After std::copy, total sum was %f, average value %f\n", total_sum, total_sum / matrix.size());

  size_t n_elements = 0;
  for (auto elem : matrix) {
    n_elements++;
    elem = matrix.size() - n_elements;
  }

  printf("Before sorting:\n");
  print_matrix(matrix);

  auto a = *matrix.begin();
  auto b = *(matrix.begin() + 1);

  std::sort(matrix.begin(), matrix.end(), [](auto a, auto b) {
                                            float a_v = a;
                                            float b_v = b;
                                            return a_v < b_v;
                                          });

  printf("After sorting:\n");
  print_matrix(matrix);

  auto iter = matrix.find({38, 22});

  if (iter != matrix.end()) {
    printf("Element 39, 23 exists! Setting to 12.\n");
    *iter = 12;
  } else {
    printf("element 39, 23 does not exist.\n");
  }

  print_matrix(matrix);

  return 0;
}