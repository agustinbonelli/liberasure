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

#pragma once

#include <sstream>
#include <ostream>
#include <string>

template <typename Tag>
struct atom {
  friend bool operator==(atom const&, atom const&) { return true; }
  friend bool operator<(atom const&, atom const&) { return false; }
  friend std::ostream& operator<<(std::ostream& o, atom const& x) {
    return o << typeid(x).name();
  }
};
