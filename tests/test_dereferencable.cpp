/*
 * Copyright 2015, 2016 Gašper Ažman
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "type_erasure/features/dereferencable.hpp"
#include "type_erasure/features/regular.hpp"
#include "type_erasure/type_erasure.hpp"

#include <memory>
#include <tuple>
#include <cassert>

struct can_const_deref {
  int operator*() const { return 1; }

  friend bool operator==(can_const_deref const&, can_const_deref const&) {
    return true;
  }
};

struct can_mutably_deref {
  int x = 0;
  int operator*() { return ++x; }

  friend bool operator==(can_mutably_deref const&, can_mutably_deref const&) {
    return true;
  }
};

void test_const_dereferencable() {
  using type_erasure::features::regular;
  using type_erasure::features::const_dereferencable;
  using type_erasure::make_any;

  auto x = make_any<regular, const_dereferencable<int>>(can_const_deref{});
  assert(*x == 1);
}

void test_mutably_dereferencable() {
  using type_erasure::features::regular;
  using type_erasure::features::mutably_dereferencable;
  using type_erasure::make_any;

  auto x = make_any<regular, mutably_dereferencable<int>>(can_mutably_deref{});
  assert(*x == 1);
  assert(*x == 2);
  assert(*x == 3);
}

int main() {
  test_const_dereferencable();
  test_mutably_dereferencable();
}
