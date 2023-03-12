#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <exception>
#include <iterator>
#include <cassert>
#define DEFAULT_CAPACITY 16
#endif //_VL_VECTOR_H_
size_t cap_max (int static_cap, int cur_size, int elem_add);
template <class T, int StaticCapacity = DEFAULT_CAPACITY>
class vl_vector
{
 public:
  typedef T * iterator;
  typedef const T * const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // iterator functions
  iterator begin()
  {return (_size > StaticCapacity) ? _dynamic_memory : _static_memory;}
  iterator end()
  {return (_size>StaticCapacity)?_dynamic_memory+_size:_static_memory + _size;}
  // const_iterator functions
  const_iterator begin() const
  {return (_size > StaticCapacity) ? _dynamic_memory : _static_memory;}
  const_iterator end() const
  {return (_size>StaticCapacity)?_dynamic_memory+_size:_static_memory + _size;}
  const_iterator cbegin() const
  {return (_size > StaticCapacity) ? _dynamic_memory : _static_memory;}
  const_iterator cend() const
  {return (_size>StaticCapacity)?_dynamic_memory+_size:_static_memory + _size;}

  //reverse iterator functions
  reverse_iterator rbegin()
  {return std::reverse_iterator<iterator> (end());}
  reverse_iterator rend()
  {return std::reverse_iterator<iterator> (begin());}

  //const_reverse iterator functions
  const_reverse_iterator rbegin() const
  {return std::reverse_iterator<const_iterator> (cend());}
  const_reverse_iterator crbegin() const
  {return std::reverse_iterator<const_iterator> (cend());}
  const_reverse_iterator rend() const
  {return std::reverse_iterator<const_iterator> (cbegin());}
  const_reverse_iterator crend() const
  {return std::reverse_iterator<const_iterator> (cbegin());}

  // default constructor
  vl_vector()
  {
    _capacity = StaticCapacity;
    _size = 0;
  }

// copy constructor
/// \param other_vector the other vector to copy details from
  vl_vector(const vl_vector& other_vector)
  {
    _capacity = other_vector._capacity;
    _size = other_vector._size;
    if (other_vector._size > StaticCapacity)
      {
        _dynamic_memory = new T[other_vector._capacity];
        for (size_t i = 0; i <other_vector._size; i++)
          {
            _dynamic_memory[i] = other_vector._dynamic_memory[i];
          }
      }
    else
      {
        for (size_t i = 0; i < other_vector._size; i++)
          {
            _static_memory[i] = other_vector._static_memory[i];
          }
      }
  }
  //sequence based constructor
  /// \tparam ForwardIterator template parameter that represents an iterator
  /// \param first begin iterator
  /// \param last end iterator
  template <class ForwardIterator>
  vl_vector(ForwardIterator first, ForwardIterator last)
  {
    _size = std::distance(first, last);
    if (_size > StaticCapacity)
      {
        _capacity = cap_max(StaticCapacity, _size, 0);
        _dynamic_memory = new T[_capacity];
        for (size_t i = 0; i <_size; i++)
          {
            _dynamic_memory[i] = *first;
            first ++;
          }
      }
    else
      {
        _capacity = StaticCapacity;
        for (size_t i = 0; i <_size; i++)
          {
            _static_memory[i] = *first;
            first ++;
          }
      }
  }
  // Single-value initialize constructor
  /// \param count number of elements to initialize with a given value
  /// \param v the given value
  vl_vector(const size_t count, const T v)
  {
    _size = count;
    if (_size <= StaticCapacity)
      {
        _capacity = StaticCapacity;
        for (size_t i = 0; i <_size; i++)
          {
            _static_memory[i] = v;
          }
      }
    else
      {
        _capacity = cap_max(StaticCapacity, _size, 0);
        _dynamic_memory = new T[_capacity];
        for (size_t i = 0; i <_size; i++)
          {
            _dynamic_memory[i] = v;
          }
      }
  }
  // destructor
  ~vl_vector()
  {
    if (_size > StaticCapacity)
      {
        delete[] _dynamic_memory;
      }
  }

  // Methods
  /// \return the amount of elements in the vector
  size_t size () const {return _size;}

  /// \return the max capacity of a vector
  size_t capacity () const {return _capacity;}

  /// \return true if the vector is empty, false otherwise
  bool empty () const {return _size == 0;}

  /// gets an index and returns the value, checks index validation,
  /// cannot change the value
  /// \param index of the value
  /// \return the value in the index
  T at (unsigned long int index) const
  {
    if (_size == 0 || index > _size - 1 || index < 0)
      {
        throw std::out_of_range("out of range");
      }
    if (_size > StaticCapacity)
      {
        return _dynamic_memory[index];
      }
    else
      {
        return _static_memory[index];
      }
  }

  /// gets an index and returns the value, checks index validation
  /// can change the value
  /// \param index of the value
  /// \return the value in the index
  T& at (unsigned long int index)
  {
    if (_size == 0 || index > _size - 1 || index < 0)
      {
        throw std::out_of_range("out of range");
      }
    if (_size > StaticCapacity)
      {
        return _dynamic_memory[index];
      }
    else
      {
        return _static_memory[index];
      }
  }

  /// pushes 1 element to the end of the vector
  /// \param element the element to push
  void push_back(const T element)
  {
    //calculating the new capacity that is needed after adding an element
    size_t new_cap = cap_max(StaticCapacity, (int) _size, 1);
    if (_size > StaticCapacity)
      {
        // only add the element in case the new capacity doesn't
        // reach the current capacity
        if (_size < _capacity)
          {
            _dynamic_memory[_size] = element;
            _size ++;
          }
        // making realloc if the capacity needed is higher than the current
        else
          {
            _capacity = new_cap;
            T * temp_memory;
            temp_memory = _dynamic_memory;
            _dynamic_memory = new T[new_cap];
            for (size_t i = 0; i < _size; i++)
              {
                _dynamic_memory[i] = temp_memory[i];
              }
            delete[] temp_memory;
            _dynamic_memory[_size] = element;
            _size ++;
          }
      }
    //  moving to dynamic memory
    else if(_size == StaticCapacity)
      {
        _dynamic_memory = new T[new_cap];
        _capacity = new_cap;
        for (size_t i = 0; i < _size; i++)
          {
            _dynamic_memory[i] = _static_memory[i];
          }
        _dynamic_memory[_size] = element;
        _size ++;
      }
    // static memory
    else if (_size < StaticCapacity)
      {
        _static_memory[_size] = element;
        _size ++;
      }
  }

  // pops an element from the end of the vector
  void pop_back()
  {
    if (_size == 0)
      {
        return;
      }
    if (_size - 1 == StaticCapacity)
      {
        for (size_t i = 0; i < _size - 1; i++)
          {
            _static_memory[i] = _dynamic_memory[i];
          }
        delete[] _dynamic_memory;
        _capacity = StaticCapacity;
      }
    _size --;
  }

  // clears the vector from elements
  void clear()
  {
    if (_size > StaticCapacity)
      {
        delete[] _dynamic_memory;
      }
    _size = 0;
    _capacity = StaticCapacity;
  }

  /// inserts one element to the vector in a given position
  /// \param position an iterator to the place of the inserted element to be
  /// \param element to insert to the vector
  /// \return iterator to the added element
  iterator insert(const_iterator position, T element)
  {
    auto non_const_position = (iterator) position;
    size_t distance_from_beg = (std::distance(begin(), end()) -
        std::distance(begin(), non_const_position));
    size_t new_cap = cap_max(StaticCapacity, (int) _size, 1);
    if (_size > StaticCapacity)
      {
        if (_size < _capacity)
          {
            iterator end_iter = end();
            while (end_iter >= position)
              {
                *end_iter = *(end_iter - 1);
                end_iter --;
              }
            *non_const_position = element;
            _size ++;
          }
        else
          {
            size_t distance_it = std::distance(begin(), non_const_position);
            _capacity = new_cap;
            T * temp_memory;
            temp_memory = new T[_size];
            for (size_t k = 0; k < _size; k++)
              {
                temp_memory[k] = _dynamic_memory[k];
              }
            delete[] _dynamic_memory;
            _dynamic_memory = new T[new_cap];
            for (size_t i = 0; i < _size; i++)
              {
                _dynamic_memory[i] = temp_memory[i];
              }
            delete[] temp_memory;
            iterator end_iter = end();
            int l = 0;

            for (size_t j = 0; j < distance_from_beg; j++)
              {
                *end_iter = *(end_iter - 1);
                end_iter --;
                l++;
              }
            non_const_position = (begin() + distance_it);
            *non_const_position = element;
            _size ++;
          }
      }
    else if(_size == StaticCapacity)
      {
        _dynamic_memory = new T[new_cap];
        _capacity = new_cap;
        for (size_t i = 0; i < _size; i++)
          {
            _dynamic_memory[i] = _static_memory[i];
          }
        size_t distance_it = std::distance(begin(), non_const_position);
        _size ++;
        iterator end_iter = end() - 1;
        for (size_t j = 0; j < distance_from_beg; j++)
          {
            *end_iter = *(end_iter - 1);
            end_iter --;
          }
        non_const_position = (begin() + distance_it);
        *non_const_position = element;
      }
    else if (_size < StaticCapacity)
      {
        iterator end_iter = end();
        while (end_iter >= position)
          {
            *end_iter = *(end_iter - 1);
            end_iter --;
          }
        *non_const_position = element;
        _size ++;
      }
    return non_const_position;
  }

  /// inserts elements by sequence to the vector in a given position
  /// \tparam ForwardIterator template parameter that represents an iterator
  /// \param position an iterator to the place of the inserted elements to be
  /// \param first iterator to the beginning of elements sequence
  /// \param last iterator to the end of elements sequence
  /// \return iterator to the first element that was added
  template <class ForwardIterator>
  iterator insert(const_iterator position,
                  ForwardIterator first, ForwardIterator last)
  {
    size_t elements_num = std::distance(first, last);
    auto non_const_position = (iterator) position;
    size_t new_cap = cap_max(StaticCapacity, (int) _size, elements_num);
    size_t distance_it = std::distance(begin(), non_const_position);
    if (_size > StaticCapacity)
      {
        if (elements_num + _size > _capacity)
          {
            _capacity = new_cap;
            T * temp_memory;
            temp_memory = new T[_size];
            for (size_t k = 0; k < _size; k++)
              {
                temp_memory[k] = _dynamic_memory[k];
              }
            delete[] _dynamic_memory;
            _dynamic_memory = new T[new_cap];
            for (size_t i = 0; i < _size; i++)
              {
                _dynamic_memory[i] = temp_memory[i];
              }
            delete[] temp_memory;
          }
        iterator end_iter = end() + elements_num;
        non_const_position = (begin() + distance_it);
        for (size_t j = 0; j < _size; j++)
          {
            *(end_iter - 1) = *(end_iter - elements_num - 1);
            end_iter --;
          }
        for (size_t i = 0; i < elements_num; i++)
          {
            _dynamic_memory[i + std::distance(begin(),
                                              non_const_position)] = *first;
            first ++;
          }
        non_const_position = (begin() + distance_it);
        _size += elements_num;
      }
    else if(_size <= StaticCapacity && (_size + elements_num) > StaticCapacity)
      {
        _dynamic_memory = new T[new_cap];
        _capacity = new_cap;
        for (size_t i = 0; i < _size; i++)
          {
            _dynamic_memory[i] = _static_memory[i];
          }
        _size += elements_num;
        iterator end_iter = end();
        non_const_position = (begin() + distance_it);
        for (size_t j = 0; j < _size - elements_num; j++)
          {
            *(end_iter - 1) = *(end_iter - elements_num - 1);
            end_iter --;
          }
        for (size_t i = 0; i < elements_num; i++)
          {
            _dynamic_memory[i + std::distance(begin(),
                                              non_const_position)] = *first;
            first ++;
          }
        non_const_position = (begin() + distance_it);
      }
    else if (elements_num + _size <= StaticCapacity)
      {
        iterator end_iter = end() + elements_num;
        while (end_iter >= non_const_position + elements_num)
          {
            *(end_iter - 1) = *(end_iter - elements_num - 1);
            end_iter --;
          }
        for (size_t i = 0; i < elements_num; i++)
          {
            _static_memory[i + std::distance(begin(),
                                             non_const_position)] = *first;
            first ++;
          }
        non_const_position = (begin() + distance_it);
        _size += elements_num;
      }
    return non_const_position;
  }

  /// erases one element from a given position
  /// \param position iterator to the element to be erased
  /// \return iterator to the right of the erased element
  iterator erase (const_iterator position)
  {
    auto non_const_position = (iterator) position;
    size_t dist = std::distance(begin(), non_const_position);
    if (_size > StaticCapacity)
      {
        iterator new_pos_iter = non_const_position;
        while (new_pos_iter <= end () - 1)
          {
            *new_pos_iter = *(new_pos_iter + 1);
            new_pos_iter++;
          }
        if (_size == StaticCapacity + 1)
          {
            _capacity = StaticCapacity;
            for (size_t i = 0; i < _size - 1; i++)
              {
                _static_memory[i] = _dynamic_memory[i];
              }
            delete[] _dynamic_memory;
          }
        _size --;
        non_const_position = begin() + dist;
      }
    else if (_size <= StaticCapacity)
      {
        iterator new_pos_iter = non_const_position;
        while (new_pos_iter <= end() - 1)
          {
            *new_pos_iter = *(new_pos_iter + 1);
            new_pos_iter ++;
          }
        _size --;
      }
    return non_const_position;
  }

  /// \tparam ForwardIterator template parameter that represents an iterator
  /// \param first iterator to the beginning of elements sequence to delete
  /// \param last iterator to the end of elements sequence to delete
  /// \return iterator to the right of the erased sequence
  template <class ForwardIterator>
  iterator erase (const ForwardIterator first, const ForwardIterator last)
  {
    auto non_first = (iterator) first;
    auto non_last = (iterator) last;
    size_t dist = std::distance(non_first, non_last);
    size_t begin_dist = std::distance(begin(), non_first);
    size_t end_dist = std::distance(begin(), end());
    if (_size > StaticCapacity && _size - dist < StaticCapacity)
      {
        for (size_t i = 0; i < end_dist - (begin_dist + dist); i++)
          {
            *non_first = *(non_first + dist);
            non_first++;
          }
        _capacity = StaticCapacity;
        for (size_t i = 0; i < _size - dist; i++)
          {
            _static_memory[i] = _dynamic_memory[i];
          }
        delete[] _dynamic_memory;
        _size = _size - dist;
      }
    else
      {
        for (size_t i = 0; i < dist; i++)
          {
            erase(non_first);
          }
      }
    iterator iter = begin() + begin_dist;
    return iter;
  }

/// \return returns a pointer to the the memory structure as a const
  const T * data () const
  {
    return begin();
  }
  /// \return returns a pointer to the the memory structure
  T * data ()
  {
    return begin();
  }

  /// checks if an element exists in a vector
  /// \param element to check if in the vector
  /// \return true if in the vector, false otherwise
  bool contains (T element) const
  {
    const_iterator iter = begin();
    for (size_t i = 0; i < size(); i++)
      {
        if (*iter == element)
          {
            return true;
          }
        iter ++;
      }
    return false;
  }

  /// [] operator that returns the value in a specific index
  /// does not check index validation, cannot change the value
  /// \param index index of a value
  /// \return the value in the index
  T operator[](int index) const
  {
    const_iterator iter = begin() + index;
    return *iter;
  }
  /// [] operator that returns the value in a specific index
  /// does not check index validation, can change the value
  /// \param index index of a value
  /// \return the value in the index
  T& operator[](int index)
  {
    iterator iter = begin() + index;
    return *iter;
  }

  /// == operator to compare between to elements from the same type
  /// \param right the right side of the ==
  /// \return true if equals, false otherwise
  bool operator==(const vl_vector& right) const
  {
    if (size() != right.size())
      {
        return false;
      }
    const_iterator left_iter = begin();
    const_iterator right_iter = right.begin();
    for (size_t i = 0; i < size(); i++)
      {
        if (*left_iter != *right_iter)
          {
            return false;
          }
        left_iter ++;
        right_iter ++;
      }
    return true;
  }

  /// != operator to compare between to elements from the same type
  /// \param right the right side of the !=
  /// \return false if equals, true otherwise
  bool operator!=(const vl_vector& right) const
  {
    if (size() != right.size())
      {
        return true;
      }
    const_iterator left_iter = begin();
    const_iterator right_iter = right.begin();
    for (size_t i = 0; i < size(); i++)
      {
        if (*left_iter != *right_iter)
          {
            return true;
          }
        left_iter ++;
        right_iter ++;
      }
    return false;
  }

  /// assignment operator to copy one vector into another vector
  /// \param other_vector the vector to copy the details from
  /// \return a reference to the vector that was updated with the other vector
  vl_vector& operator=(const vl_vector& other_vector)
  {
    if(this != &other_vector)
      {
        if (other_vector._size > StaticCapacity)
          {
            if (_size > StaticCapacity)
              {
                delete[] _dynamic_memory;
              }
            _capacity = other_vector._capacity;
            _size = other_vector._size;
            _dynamic_memory = new T[other_vector._capacity];
            for (size_t i = 0; i <other_vector._size; i++)
              {
                _dynamic_memory[i] = other_vector._dynamic_memory[i];
              }
          }
        else if (other_vector._size <= StaticCapacity &&
        _size > StaticCapacity)
          {
            delete[] _dynamic_memory;
            _capacity = other_vector._capacity;
            _size = other_vector._size;
            for (size_t i = 0; i < other_vector._size; i++)
              {
                _static_memory[i] = other_vector._static_memory[i];
              }
          }
        else if (other_vector._size <= StaticCapacity &&
        _size <= StaticCapacity)
          {
            _capacity = other_vector._capacity;
            _size = other_vector._size;
            for (size_t i = 0; i < other_vector._size; i++)
              {
                _static_memory[i] = other_vector._static_memory[i];
              }
          }
      }
    return *this;
  }

 private:
  T _static_memory[StaticCapacity];
  T * _dynamic_memory;
  size_t _size;
  size_t _capacity;
};

/// calculates the new maximum capacity of a vector
/// \param static_cap the StaticCapacity that is given by the template
/// \param cur_size the current size of the vector
/// \param elem_add numbers of elements that will be added/removed
/// \return the new capacity of the vector
size_t cap_max (int static_cap, int cur_size, int elem_add)
{
  if (cur_size + elem_add <= static_cap)
    {
      return static_cap;
    }
  return (((3 * (cur_size + elem_add)) / 2));
}
