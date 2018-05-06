#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <climits>
#include <cstddef>

namespace stl 
{
	/**
	* a data container like std::vector
	* store data in a successive memory and support random access.
	*/
	template<typename T>
	class vector 
	{
	private:
		T * data;
		size_t currentLength;
		size_t maxSize;
		void doubleSpace()
		{
			T *tmp = data;
			maxSize *= 2;
			data = (T*) ::operator new[](sizeof(T) * maxSize);
			for (size_t i = 0; i < currentLength; ++i)
				new(data + i) T(tmp[i]);
			for (int i = 0; i < maxSize / 2; ++i)
				tmp[i].~T();
			::operator delete[](tmp);
		}

	public:
		/**
		* TODO
		* a type for actions of the elements of a vector, and you should write
		*   a class named const_iterator with same interfaces.
		*/
		/**
		* you can see RandomAccessIterator at CppReference for help.
		*/
		class const_iterator;
		class iterator {
			friend class vector;
		private:
			/**
			* TODO add data members
			*   just add whatever you want.
			*/
			vector *ptr = nullptr;
			int index;

		public:
			/**
			* return a new iterator which pointer n-next elements
			*   even if there are not enough elements, just return the answer.
			* as well as operator-
			*/
			iterator() = default;
			iterator(vector *_ptr, int _index)
			{
				ptr = _ptr;
				index = _index;
			}
			iterator(const iterator &other)
			{
				ptr = other.ptr;
				index = other.index;
			}
			iterator operator+(const int &n) const
			{
				//TODO
				return iterator(ptr, index + n);
			}
			iterator operator-(const int &n) const
			{
				//TODO
				return iterator(ptr, index - n);
			}
			// return th distance between two iterator,
			// if these two iterators points to different vectors, throw invaild_iterator.
			int operator-(const iterator &rhs) const
			{
				//TODO
				return index - rhs.index;
			}
			iterator operator+=(const int &n)
			{
				//TODO
				index += n;
				return *this;
			}
			iterator operator-=(const int &n)
			{
				//TODO
				index -= n;
				return *this;
			}
			/**
			* TODO iter++
			*/
			iterator operator++(int)
			{
				iterator tmp = *this;
				++index;
				return tmp;
			}
			/**
			* TODO ++iter
			*/
			iterator& operator++()
			{
				++index;
				return *this;
			}
			/**
			* TODO iter--
			*/
			iterator operator--(int)
			{
				iterator tmp = *this;
				--index;
				return tmp;
			}
			/**
			* TODO --iter
			*/
			iterator& operator--()
			{
				--index;
				return *this;
			}
			/**
			* TODO *it
			*/
			T& operator*() const
			{
				return (*ptr)[index];
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			bool operator==(const iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			bool operator==(const const_iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
		};
		/**
		* TODO
		* has same function as iterator, just for a const object.
		*/
		class const_iterator
		{
		private:
			vector * ptr = nullptr;
			int index;

		public:
			const_iterator() = default;
			const_iterator(const vector *_ptr, int _index)
			{
				ptr = (vector*)_ptr;
				index = _index;
			}
			const_iterator(const const_iterator &other)
			{
				ptr = other.ptr;
				index = other.index;
			}
			const_iterator operator+(const int &n) const
			{
				return const_iterator(ptr, index + n);
			}
			const_iterator operator-(const int &n) const
			{
				return const_iterator(ptr, index - n);
			}
			int operator-(const const_iterator &rhs) const
			{
				return index - rhs.index;
			}
			const_iterator operator+=(const int &n)
			{
				index += n;
				return *this;
			}
			const_iterator operator-=(const int &n)
			{
				index -= n;
				return *this;
			}
			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++index;
				return tmp;
			}
			const_iterator& operator++()
			{
				++index;
				return *this;
			}
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				--index;
				return tmp;
			}
			const_iterator& operator--()
			{
				--index;
				return *this;
			}
			T operator*() const
			{
				return (*ptr)[index];
			}
			bool operator==(const iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			bool operator==(const const_iterator &rhs) const
			{
				return ptr == rhs.ptr && index == rhs.index;
			}
			bool operator!=(const iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return !(ptr == rhs.ptr && index == rhs.index);
			}
		};
		/**
		* TODO Constructs
		* Atleast three: default constructor, copy constructor and a constructor for std::vector
		*/
		vector(int initSize = 100)
		{
			if (data != nullptr)
				clear();
			data = (T*) ::operator new[](sizeof(T) * initSize);
			maxSize = initSize;
			currentLength = 0;
		}
		vector(const vector &other)
		{
			if (data != nullptr)
				clear();
			data = (T*) ::operator new[](sizeof(T) * other.size());
			maxSize = currentLength = other.size();
			for (size_t i = 0; i < other.size(); ++i)
				new(data + i) T(other.data[i]);
		}

		/**
		* TODO Destructor
		*/
		~vector()
		{
			clear();
		}
		/**
		* TODO Assignment operator
		*/
		vector &operator=(const vector &other)
		{
			if (data != nullptr)
				clear();
			data = (T*) ::operator new[](sizeof(T) * other.size());
			maxSize = currentLength = other.size();
			for (size_t i = 0; i < other.size(); ++i)
				new(data + i) T(other.data[i]);
			return *this;
		}
		/**
		* assigns specified element with bounds checking
		* throw index_out_of_bound if pos is not in [0, size)
		*/
		T & at(const size_t &pos)
		{
			return data[pos];
		}
		const T & at(const size_t &pos) const
		{
			return data[pos];
		}
		/**
		* assigns specified element with bounds checking
		* throw index_out_of_bound if pos is not in [0, size)
		* !!! Pay attentions
		*   In STL this operator does not check the boundary but I want you to do.
		*/
		T & operator[](const size_t &pos)
		{
			return data[pos];
		}
		const T & operator[](const size_t &pos) const
		{
			return data[pos];
		}
		/**
		* access the first element.
		* throw container_is_empty if size == 0
		*/
		const T & front() const
		{
			return data[0];
		}
		/**
		* access the last element.
		* throw container_is_empty if size == 0
		*/
		const T & back() const
		{
			return data[currentLength - 1];
		}
		/**
		* returns an iterator to the beginning.
		*/
		iterator begin()
		{
			return iterator(this, 0);
		}
		const_iterator cbegin() const
		{
			return const_iterator(this, 0);
		}
		/**
		* returns an iterator to the end.
		*/
		iterator end()
		{
			return iterator(this, currentLength);
		}
		const_iterator cend() const
		{
			return const_iterator(this, currentLength);
		}
		/**
		* checks whether the container is empty
		*/
		bool empty() const
		{
			return currentLength == 0;
		}
		/**
		* returns the number of elements
		*/
		size_t size() const
		{
			return currentLength;
		}
		/**
		* returns the number of elements that can be held in currently allocated storage.
		*/
		size_t capacity() const
		{
			return maxSize;
		}
		/**
		* clears the contents
		*/
		void clear()
		{
			for (int i = 0;i < currentLength; ++i)
				data[i].~T();
			::operator delete[](data);
			maxSize = currentLength = 0;
		}
		/**
		* inserts value before pos
		* returns an iterator pointing to the inserted value.
		*/
		iterator insert(iterator pos, const T &value)
		{
			if (currentLength == maxSize)
				doubleSpace();
			new(data + currentLength) T(data[currentLength - 1]);
			for (size_t i = currentLength - 1; i > pos.index; --i)
				data[i] = data[i - 1];
			data[pos.index] = value;
			++currentLength;
			return pos;
		}
		/**
		* inserts value at index ind.
		* after inserting, this->at(ind) == value is true
		* returns an iterator pointing to the inserted value.
		* throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
		*/
		iterator insert(const size_t &ind, const T &value)
		{
			if (currentLength == maxSize)
				doubleSpace();
			new(data + currentLength) T(data[currentLength - 1]);
			for (size_t i = currentLength - 1; i > ind; --i)
				data[i] = data[i - 1];
			data[ind] = value;
			++currentLength;
			return iterator(this, ind);
		}
		/**
		* removes the element at pos.
		* return an iterator pointing to the following element.
		* If the iterator pos refers the last element, the end() iterator is returned.
		*/
		iterator erase(iterator pos)
		{
			for (size_t i = pos.index; i < currentLength; ++i)
				data[i] = data[i + 1];
			--currentLength;
			return pos;
		}
		/**
		* removes the element with index ind.
		* return an iterator pointing to the following element.
		* throw index_out_of_bound if ind >= size
		*/
		iterator erase(const size_t &ind)
		{
			for (size_t i = ind; i < currentLength; ++i)
				data[i] = data[i + 1];
			--currentLength;
			return iterator(this, ind);
		}
		/**
		* adds an element to the end.
		*/
		void push_back(const T &value)
		{
			if (currentLength == maxSize)
				doubleSpace();
			new(data + currentLength) T(value);
			++currentLength;
		}
		/**
		* remove the last element from the end.
		* throw container_is_empty if size() == 0
		*/
		void pop_back()
		{
			if (currentLength == 0)
				throw container_is_empty();
			--currentLength;
		}
	};


}

#endif