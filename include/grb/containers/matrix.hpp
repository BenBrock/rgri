
#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <memory>
#include <initializer_list>
#include "matrix_reference.hpp"
#include "matrix_iterator.hpp"
#include "backend/csr_matrix.hpp"
#include "backend/dense_matrix.hpp"
#include <grb/exceptions/exception.hpp>

#include <grb/util/matrix_hints.hpp>

namespace grb {

template <typename T,
          typename I = std::size_t,
          typename Hint = grb::sparse,
          typename Allocator = std::allocator<T>>
class matrix {
public:
  using value_type = T;
  using index_type = I;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using hint_type = Hint;

  // using backend_type = csr_matrix_impl_<value_type, index_type, allocator_type>;
  using backend_type = typename pick_backend_type<Hint>:: template type<T, I, Allocator>;

  using backend_reference = typename backend_type::reference;
  using backend_iterator = typename backend_type::iterator;

  using iterator = matrix_iterator<backend_iterator>;
  using const_iterator = const_matrix_iterator<backend_iterator>;

  using reference = matrix_reference<backend_reference>;
  using const_reference = const_matrix_reference<backend_reference>;

  using tuples_type = std::vector<std::tuple<value_type, index_type, index_type>>;

  matrix() = default;
  matrix(const matrix&) = default;
  matrix& operator=(const matrix&) = default;

  matrix(std::initializer_list<size_type> dimensions) {
    if (dimensions.size() != 2) {
      throw invalid_argument("grb::matrix::matrix(std::initializer_list<size_type>): invalid list size");
    }
    backend_matrix_ = backend_type(index_t{*dimensions.begin(), *(dimensions.begin()+1)});
  }

  matrix(index_t dimensions) : backend_matrix_(dimensions) {}

  matrix(std::string fname) : backend_matrix_(read_MatrixMarket<T, I>(fname)) {}

  index_t shape() const noexcept {
    return {backend_matrix_.m(), backend_matrix_.n()};
  }

  size_type size() const noexcept {
    return backend_matrix_.nnz();
  }

  iterator begin() {
    return iterator(backend_matrix_.begin());
  }

  iterator end() {
    return iterator(backend_matrix_.end());
  }

  const_iterator begin() const {
    return const_iterator(backend_matrix_.begin());
  }

  const_iterator end() const {
    return const_iterator(backend_matrix_.end());
  }

  iterator find(index_t index) {
    if (index[0] >= shape()[0] || index[1] >= shape()[1]) {
      return end();
    }
    return backend_matrix_.find(index);
  }

  reference operator[](index_t index) {
    auto iter = find(index);
    if (iter == end()) {
      tuples_type new_tuple = {{value_type{}, index[0], index[1]}};
      backend_matrix_.insert_tuples(new_tuple);
      iter = find(index);
    }
    return *iter;
  }

  reference at(index_t index) {
    if (index[0] >= shape()[0] || index[1] >= shape()[1]) {
      throw grb::out_of_range("grb::matrix::at(): called with out of range indices");
    }
    auto iter = find(index);
    if (iter == end()) {
      throw grb::out_of_range("grb::matrix::at(): element not present in matrix");
    } else {
      return *iter;
    }
  }

private:
  backend_type backend_matrix_;

  friend iterator;
  friend reference;
  friend const_iterator;
  friend const_reference;
};

} // end grb

#include "matrix_impl.hpp"