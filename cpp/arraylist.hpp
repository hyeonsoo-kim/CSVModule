#ifndef __CSVMODULE_ARRAYLIST_HPP__
#define __CSVMODULE_ARRAYLIST_HPP__
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#if __cplusplus < 201103L
#define nullptr ((void*)0)
#endif

#if __cplusplus >= 201103L
#include <utility>
#endif

template <class Tp>
class ArrayList {
 public:
  ArrayList() {}                                      // Empty Constructor
  explicit ArrayList(Tp* list, const size_t length);  // constructor from array
  ArrayList(const std::vector<Tp>& vec);              // constructor from vector
  ArrayList(const Tp& c, size_t n);                   // Fill constructor
  ArrayList(const ArrayList<Tp>& al);                 // copy constructor
#if __cplusplus >= 201103L
  ArrayList(std::initializer_list<Tp> ilist);
#endif
  //~ArrayList() {}
  void append(const Tp& x);               // append element to back
  void remove(const Tp& x);               // remove element at idx
  void extend(const ArrayList<Tp>& arr);  // extend(append) array list
  void clear(void);

  size_t length(void) const;  // the number of elements in arraylist

  Tp& operator[](const size_t idx);  // indexing for read and writing element
  Tp operator[](const size_t idx) const;  // indexing for read-only element

  operator std::string();            // type casting operator to string
  operator std::vector<Tp>();        // type casting operator to vector
  operator std::vector<Tp>() const;  // type casting operator to const vector

  ArrayList<Tp>& operator=(const ArrayList<Tp>& arr);   // assign
  ArrayList<Tp>& operator+=(const ArrayList<Tp>& arr);  // extend and assign
  ArrayList<Tp> operator+(
      const ArrayList<Tp>& arr);  // extend and return exension
  typename std::vector<Tp>::iterator begin() { return c.begin(); }
  typename std::vector<Tp>::iterator end() { return c.end(); }

 private:
  std::vector<Tp> c;
  void rangeCheck(const size_t idx);  // check index in list's range
  typename std::vector<Tp>::iterator getPostionOfValue(
      const Tp& val);  // check value is in list
};

#if __cplusplus >= 202002L
#define arrlist_relational_operator(op)                \
  template <class Tp>                                  \
  constexpr bool operator op(const ArrayList<Tp>& lhs, \
                             const ArrayList<Tp>& rhs)
#else
#define arrlist_relational_operator(op) \
  template <class Tp>                   \
  bool operator op(const ArrayList<Tp>& lhs, const ArrayList<Tp>& rhs)
#endif

template <class Tp>
void ArrayList<Tp>::rangeCheck(const size_t idx) {
  if (idx >= this->c.size()) throw std::out_of_range("index is out of range");
}

template <class Tp>
typename std::vector<Tp>::iterator ArrayList<Tp>::getPostionOfValue(
    const Tp& val) {
  typename std::vector<Tp>::iterator pos =
      find(this->c.begin(), this->c.end(), val);
  if (pos == this->c.end()) {
    std::stringstream ss;
    ss << "Value " << val << " does not exist.";
    throw std::invalid_argument(ss.str());
  }
  return pos;
}

template <class Tp>
ArrayList<Tp>::ArrayList(Tp* list, const size_t length) {
  if (list == nullptr) {
    this->c.reserve(0);
  } else {
    this->c = std::vector<Tp>(list, list + length);
  }
}

template <class Tp>
ArrayList<Tp>::ArrayList(const std::vector<Tp>& vec)
    : c(std::vector<Tp>(vec.begin(), vec.end())) {}

template <class Tp>
ArrayList<Tp>::ArrayList(const Tp& c, size_t n) : c(std::vector<Tp>(c, n)) {}

template <class Tp>
ArrayList<Tp>::ArrayList(const ArrayList<Tp>& al)
    : c(std::vector<Tp>(al.c.begin(), al.c.end())) {}

#if __cplusplus >= 201103L
template <class Tp>
ArrayList<Tp>::ArrayList(std::initializer_list<Tp> ilist)
    : c(std::vector<Tp>(ilist)) {}
#endif

template <class Tp>
void ArrayList<Tp>::append(const Tp& x) {
  this->c.push_back(x);
}

template <class Tp>
void ArrayList<Tp>::remove(const Tp& x) {
  typename std::vector<Tp>::iterator pos = this->getPostionOfValue(x);
  this->c.erase(pos);
}

template <class Tp>
void ArrayList<Tp>::extend(const ArrayList<Tp>& arr) {
  this->c.insert(this->c.end(), arr.c.begin(), arr.c.end());
}

template <class Tp>
void ArrayList<Tp>::clear() {
  this->c.clear();
}

template <class Tp>
Tp& ArrayList<Tp>::operator[](const size_t idx) {
  this->rangeCheck(idx);
  return this->c[idx];
}

template <class Tp>
Tp ArrayList<Tp>::operator[](const size_t idx) const {
  this->rangeCheck(idx);
  return this->c[idx];
}

template <class Tp>
size_t ArrayList<Tp>::length(void) const {
  return this->c.size();
}

/*
 * Type conversion operators
 */
template <class Tp>
ArrayList<Tp>::operator std::string() {
  std::stringstream ss;
  ss << '[';
  typename std::vector<Tp>::iterator iter = this->c.begin();
  for (; iter != this->c.end(); iter++) {
    ss << *iter;
    if (iter != this->c.end() - 1) ss << ',';
  }
  ss << ']';
  return ss.str();
}

template <class Tp>
ArrayList<Tp>::operator std::vector<Tp>() {
  return this->c;
}

template <class Tp>
ArrayList<Tp>::operator std::vector<Tp>() const {
  return this->c;
}

/*
 * assign and extend operators
 */
template <class Tp>
ArrayList<Tp>& ArrayList<Tp>::operator=(const ArrayList<Tp>& arr) {
  this->c.clear();
  this->c.assign(arr.c.begin(), arr.c.end());
  return *this;
}

template <class Tp>
ArrayList<Tp>& ArrayList<Tp>::operator+=(const ArrayList<Tp>& arr) {
  this->extend(arr);
  return *this;
}

template <class Tp>
ArrayList<Tp> ArrayList<Tp>::operator+(const ArrayList<Tp>& arr) {
  std::vector<Tp> cpy(this->c);
  cpy.insert(cpy.end(), arr.c.begin(), arr.c.end());
  return ArrayList<Tp>(cpy);
}
/*
 * relational operators
 */

arrlist_relational_operator(==) {
  return static_cast<std::vector<Tp> >(lhs) ==
         static_cast<std::vector<Tp> >(rhs);
}

arrlist_relational_operator(!=) {
  return !(static_cast<std::vector<Tp> >(lhs) =
               static_cast<std::vector<Tp> >(rhs));
}

arrlist_relational_operator(<) {
  std::vector<Tp>& lhv = static_cast<std::vector<Tp> >(lhs);
  std::vector<Tp>& rhv = static_cast<std::vector<Tp> >(rhs);
  return std::lexicographical_compare(lhv.begin(), lhv.end(), rhv.begin(),
                                      rhv.end());
}

arrlist_relational_operator(>) { return !(lhs < rhs) && !(lhs == rhs); }

arrlist_relational_operator(>=) { return !(lhs < rhs); }

arrlist_relational_operator(<=) { return (lhs < rhs) || (lhs == rhs); }

#if __cplusplus >= 202002L
arrlist_relational_operator(<=>) {
  return static_cast<std::vector<Tp> >(lhs) <=>
         ststic_cast<std::vector<Tp> >(rhs);
}
#endif

// stream operators
template <class Tp>
std::ostream& operator<<(std::ostream& os, ArrayList<Tp> arrlist) {
  os << static_cast<std::string>(arrlist);
  return os;
}

template <class Tp>
std::istream& operator>>(std::istream& is, ArrayList<Tp>& arrlist) {
  return is;
}

#endif