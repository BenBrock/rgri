
#pragma once

#include <grb/util/matrix_io.hpp>
#include <grb/containers/backend/dense_matrix_iterator.hpp>
#include <grb/containers/backend/dense_matrix_reference.hpp>

namespace grb {

template <typename T,
          typename I,
          typename Allocator>
struct dense_matrix_impl_ {
  using value_type = T;
  using index_type = I;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using iterator = dense_matrix_impl_iterator<dense_matrix_impl_>;
  using reference = dense_matrix_impl_reference<dense_matrix_impl_>;

  using tuples_type = std::vector<std::tuple<value_type, index_type, index_type>>;

  dense_matrix_impl_() : m_(0), n_(0) {}

  dense_matrix_impl_(index_t shape);

  dense_matrix_impl_(const import_matrix_type_<value_type, index_type>& matrix);

  void assign_tuples(const tuples_type& tuples);

  size_type m() const {
    return m_;
  }

  size_type n() const {
    return n_;
  }

  size_type nnz() const {
    return m()*n();
  }

  iterator begin() const {
    // TODO: I should perhaps not do this.
    return iterator(*const_cast<dense_matrix_impl_*>(this), 0);
  }

  iterator end() const {
    // TODO: I should perhaps not do this.
    return iterator(*const_cast<dense_matrix_impl_*>(this), m()*n());
  }

  iterator find(index_t idx);

  void insert_tuples(tuples_type& tuples);

  size_type m_, n_;

  std::vector<value_type> values_;

  friend iterator;
  friend reference;
};

} // end grb

#include <grb/containers/backend/dense_matrix_impl.hpp>
