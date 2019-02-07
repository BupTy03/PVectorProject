#pragma once
#ifndef VECTOR_REF
#define VECTOR_REF

#include<memory>
#include<utility>
#include<stdexcept>
#include<cassert>
#include<iterator>
#include<initializer_list>
#include<limits>
#include<functional>
#include<algorithm>
#include<vector>

namespace my
{
	template<class T, class F = std::less<T>>
	struct PVectorPredicateWrapper
	{
		PVectorPredicateWrapper() {}
		PVectorPredicateWrapper(F f) : func{ f } {}
		constexpr bool operator()(T* left, T* right) const
		{
			return func(*left, *right);
		}
	private:
		F func;
	};

	using Index = int;

	template<class T, class A = std::allocator<T>>
	class PVector
	{
	public:
		using allocator_type = typename std::allocator_traits<A>::template rebind_alloc<T>;
		using ptr_allocator_type = typename std::allocator_traits<A>::template rebind_alloc<T*>;
		using size_type = Index;

		class PVectorIterator;
		class ConstPVectorIterator;

		using iterator = PVectorIterator;
		using const_iterator = ConstPVectorIterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		PVector() {}
		PVector(Index sz) : sz_{ sz }, space_{ sz }
		{
			elem_ = ptr_alloc_.allocate(sz);
			for (Index i = 0; i < sz; ++i)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i]);
			}
		}
		PVector(Index sz, const T& val) : sz_{ sz }, space_{ sz }
		{
			elem_ = ptr_alloc_.allocate(space_);
			for (Index i = 0; i < sz_; ++i)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i], val);
			}
		}

		PVector(const PVector& other) : sz_{ other.sz_ }, space_{ other.sz_ }
		{
			elem_ = ptr_alloc_.allocate(space_);
			for (Index i = 0; i < sz_; ++i)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i], *(other.elem_[i]));
			}
		}
		~PVector() { clear(); }
		PVector& operator=(const PVector& other)
		{
			if (&other == this)
			{
				return *this;
			}

			if (other.empty())
			{
				this->clear();
				return *this;
			}

			for (Index i = 0; i < sz_; ++i)
			{
				alloc_.destroy(elem_[i]);
				alloc_.deallocate(elem_[i], 1);
			}

			if (space_ >= other.sz_)
			{
				for (Index i = 0; i < other.sz_; ++i)
				{
					elem_[i] = alloc_.allocate(1);
					alloc_.construct(elem_[i], *(other.elem_[i]));
				}

				sz_ = other.sz_;
				return *this;
			}

			ptr_alloc_.deallocate(elem_, space_);
			elem_ = ptr_alloc_.allocate(other.sz_);
			sz_ = other.sz_;
			space_ = sz_;

			for (Index i = 0; i < sz_; ++i)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i], *(other.elem_[i]));
			}

			return *this;
		}

		PVector(PVector&& other) { this->operator=(std::move(other)); }
		PVector& operator=(PVector&& other)
		{
			if (&other == this)
			{
				return *this;
			}

			std::swap(this->elem_, other.elem_);
			std::swap(this->sz_, other.sz_);
			std::swap(this->space_, other.space_);

			return *this;
		}

		inline size_type size() const { return sz_; }
		inline size_type max_size() const { return std::numeric_limits<size_type>::max(); }
		inline size_type capacity() const { return space_; }
		inline T** data() { return elem_; }
		inline allocator_type get_allocator() const { return alloc_; }
		inline const T* const* data() const { return elem_; }
		inline bool empty() const { return sz_ == 0; }

		inline T& operator[](Index x)
		{
			assert(x >= 0 && x < sz_ && "index is out of range");
			return *(elem_[x]);
		}
		inline const T& operator[](Index x) const
		{
			assert(x >= 0 && x < sz_ && "index is out of range");
			return *(elem_[x]);
		}

		inline T& at(size_type x)
		{
			range_check(x);
			return *(elem_[x]);
		}
		inline const T& at(size_type x) const
		{
			range_check(x);
			return *(elem_[x]);
		}

		inline T& front()
		{
			empty_check();
			return *(elem_[0]);
		}
		inline const T& front() const
		{
			empty_check();
			return *(elem_[0]);
		}

		inline T& back()
		{
			empty_check();
			return *elem_[sz_ - 1];
		}
		inline const T& back() const
		{
			empty_check();
			return *elem_[sz_ - 1];
		}

		void reserve(size_type newalloc)
		{
			if (newalloc < 0)
				throw std::invalid_argument{ "newalloc must be positive" };

			if (newalloc <= space_) return;

			T** ptr = ptr_alloc_.allocate(newalloc);
			for (Index i = 0; i < sz_; ++i)
				ptr[i] = elem_[i];

			ptr_alloc_.deallocate(elem_, space_);
			elem_ = ptr;
			space_ = newalloc;
		}

		void resize(size_type newsize)
		{
			reserve(newsize);
			for (Index i = 0; i < newsize; ++i)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i]);
			}
			sz_ = newsize;
		}
		void resize(size_type newsize, const T& val)
		{
			reserve(newsize);
			for (Index i = 0; i < newsize; ++i)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i], val);
			}
			sz_ = newsize;
		}

		iterator insert(const_iterator p, const T& val)
		{
			Index index = p - cbegin();

			if (sz_ + 1 > space_)
				reserve((sz_ == 0) ? 8 : 2 * sz_);

			for (Index i = sz_; i > index; --i)
				elem_[i] = elem_[i - 1];

			elem_[index] = alloc_.allocate(1);
			alloc_.construct(elem_[index], val);
			++sz_;
			return begin() + index;
		}
		iterator insert(const_iterator p, T&& val)
		{
			Index index = p - cbegin();

			if (sz_ + 1 > space_)
				reserve((sz_ == 0) ? 8 : 2 * sz_);

			for (Index i = sz_; i > index; --i)
				elem_[i] = elem_[i - 1];

			elem_[index] = alloc_.allocate(1);
			alloc_.construct(elem_[index], std::move(val));
			++sz_;
			return begin() + index;
		}
		iterator insert(const_iterator p, size_type count, const T& val)
		{
			Index index = p - cbegin();

			if (count < 0)
				return begin() + index;

			if (sz_ + count > space_)
				reserve((sz_ == 0) ? count : 2 * sz_ + count);

			Index end_indx = index + count;
			for (Index i = sz_ + count; i >= end_indx; --i)
				elem_[i] = elem_[i - count];

			for (Index i = index; i < end_indx; ++i)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i], val);
			}

			sz_ += count;

			return begin() + index;
		}
		template<class InputIt>
		iterator insert(const_iterator p, InputIt first, InputIt last)
		{
			Index index = p - cbegin();

			Index count = std::distance(first, last);
			if (count < 0)
				return begin() + index;

			if (sz_ + count > space_)
				reserve((sz_ == 0) ? sz_ + count : 2 * sz_ + count);

			Index end_indx = index + count;
			for (Index i = sz_ + count; i >= end_indx; --i)
				elem_[i] = elem_[i - count];

			for (Index i = index; i < end_indx; ++i, ++first)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i], *first);
			}

			sz_ += count;
			return begin() + index;
		}
		iterator insert(const_iterator p, std::initializer_list<T> ilist)
		{
			int index = p - cbegin();

			int count = ilist.size();
			if (count < 0)
				return begin() + index;

			if (sz_ + count > space_)
				reserve((sz_ == 0) ? sz_ + count : 2 * sz_ + count);

			Index end_indx = index + count;
			for (Index i = sz_ + count; i >= end_indx; --i)
				elem_[i] = elem_[i - count];

			auto first = ilist.begin();
			for (Index i = index; i < end_indx; ++i, ++first)
			{
				elem_[i] = alloc_.allocate(1);
				alloc_.construct(elem_[i], std::move(*first));
			}

			sz_ += count;
			return begin() + index;
		}

		template<class... Args>
		iterator emplace(const_iterator p, Args&&... args)
		{
			Index index = p - cbegin();

			if (sz_ + 1 > space_)
				reserve((sz_ == 0) ? 8 : 2 * sz_);

			for (Index i = sz_; i > index; --i)
				elem_[i] = elem_[i - 1];

			elem_[index] = alloc_.allocate(1);
			alloc_.construct(elem_[index], std::forward<Args>(args)...);
			++sz_;
			return begin() + index;
		}

		iterator erase(const_iterator p)
		{
			if (p == end()) return iterator(p.p);

			Index index = p - cbegin();

			alloc_.destroy(elem_[index]);
			alloc_.deallocate(elem_[index], 1);

			for (Index i = index + 1; i < sz_; ++i)
				elem_[i - 1] = elem_[i];

			--sz_;
			return iterator(p.p);
		}
		iterator erase(const_iterator first, const_iterator last)
		{
			if (first == last) return iterator(last.p);

			Index index_first = first - cbegin();
			Index count = last - first;
			Index index_last = index_first + count;

			for (Index i = index_first; i < index_last; ++i)
			{
				alloc_.destroy(elem_[i]);
				alloc_.deallocate(elem_[i], 1);
			}

			sz_ -= count;
			for (Index i = index_first; i < sz_; ++i)
				elem_[i] = elem_[i + count];

			return begin() + index_first;
		}

		void push_back(const T& val)
		{
			if (space_ == 0) reserve(8);
			else if (sz_ + 1 > space_) reserve(2 * space_);
			elem_[sz_] = alloc_.allocate(1);
			alloc_.construct(elem_[sz_], val);
			++sz_;
		}
		void push_back(T&& val)
		{
			if (space_ == 0) reserve(8);
			else if (sz_ + 1 > space_) reserve(2 * space_);
			elem_[sz_] = alloc_.allocate(1);
			alloc_.construct(elem_[sz_], std::move(val));
			++sz_;
		}

		template<class... Args>
		void emplace_back(Args&&... args)
		{
			if (space_ == 0) reserve(8);
			else if (sz_ + 1 > space_) reserve(2 * space_);
			elem_[sz_] = alloc_.allocate(1);
			alloc_.construct(elem_[sz_], std::forward<Args>(args)...);
			++sz_;
		}

		void pop_back()
		{
			empty_check();
			T* p = elem_[sz_ - 1];
			alloc_.destroy(p);
			alloc_.deallocate(p, 1);
			--sz_;
		}

		template<class InputIt>
		void assign(InputIt first, InputIt last)
		{
			if (std::distance(first, last) > sz_)
				throw std::out_of_range{ "range size is grater then this size" };

			for (Index i = 0; first != last; ++first, ++i)
				*(elem_[i]) = *first;
		}
		void assign(Index count, const T& val)
		{
			if (count > sz_)
				throw std::out_of_range{ "count is grater then this size" };

			for (Index i = 0; i < count; ++i)
				*(elem_[i]) = val;
		}

		void shrink_to_fit()
		{
			if (sz_ == space_) return;
			T** ptr = ptr_alloc_.allocate(sz_);
			for (Index i = 0; i < sz_; ++i)
				ptr[i] = elem_[i];
			ptr_alloc_.deallocate(elem_, space_);
			elem_ = ptr;
			space_ = sz_;
		}
		void clear()
		{
			for (Index i = 0; i < sz_; ++i)
			{
				alloc_.destroy(elem_[i]);
				alloc_.deallocate(elem_[i], 1);
			}

			ptr_alloc_.deallocate(elem_, space_);
			elem_ = nullptr;
			sz_ = 0;
			space_ = 0;
		}
		void swap(PVector& vec)
		{
			std::swap(this->elem_, vec.elem_);
			std::swap(this->sz_, vec.sz_);
			std::swap(this->space_, vec.space_);
		}
		void swap_elems(size_type i, size_type j)
		{
			range_check(i);
			range_check(j);
			std::swap(elem_[i], elem_[j]);
		}

		void sort()
		{
			std::sort(elem_, elem_ + sz_, PVectorPredicateWrapper<T>());
		}
		template<class Comp>
		void sort(Comp comp)
		{
			std::sort(elem_, elem_ + sz_, PVectorPredicateWrapper<T, Comp>(comp));

		}

		void partial_sort(iterator middle)
		{
			std::partial_sort(elem_, elem_ + (middle - begin()), elem_ + sz_, PVectorPredicateWrapper<T>());
		}
		template<class Comp>
		void partial_sort(iterator middle, Comp comp)
		{
			std::partial_sort(elem_, elem_ + (middle - begin()), elem_ + sz_, PVectorPredicateWrapper<T, Comp>(comp));
		}

		void stable_sort()
		{
			std::stable_sort(elem_, elem_ + sz_, PVectorPredicateWrapper<T>());
		}
		template<class Comp>
		void stable_sort(Comp comp)
		{
			std::stable_sort(elem_, elem_ + sz_, PVectorPredicateWrapper<T, Comp>(comp));
		}

		class PVectorPtrIterator;
		inline T** ptr_begin() { return elem_; }
		inline T** ptr_end() { return elem_ + sz_; }

		inline iterator begin() { return iterator{ elem_ }; }
		inline iterator end() { return iterator{ elem_ + sz_ }; }

		inline const_iterator begin() const { return const_iterator{ elem_ }; }
		inline const_iterator end() const { return const_iterator{ elem_ + sz_ }; }

		inline const_iterator cbegin() const { return const_iterator{ elem_ }; }
		inline const_iterator cend() const { return const_iterator{ elem_ + sz_ }; }


		inline reverse_iterator rbegin() { return reverse_iterator{ end() }; }
		inline reverse_iterator rend() { return reverse_iterator{ begin() }; }

		inline const_reverse_iterator rbegin() const { return const_reverse_iterator{ cend() }; }
		inline const_reverse_iterator rend() const { return const_reverse_iterator{ cbegin() }; }

		inline const_reverse_iterator crbegin() const { return const_reverse_iterator{ cend() }; }
		inline const_reverse_iterator crend() const { return const_reverse_iterator{ cbegin() }; }

	private:
		inline void range_check(Index x)
		{
			if (x < 0 || x >= sz_) throw std::out_of_range{ "index is out of range" };
		}
		inline void empty_check()
		{
			if (sz_ == 0) throw std::out_of_range{ "container is empty" };
		}

		T** elem_{ nullptr };
		size_type sz_{};
		size_type space_{};
		allocator_type alloc_;
		ptr_allocator_type ptr_alloc_;
	};

	template<class T, class A>
	class PVector<T, A>::PVectorIterator : public std::iterator<std::random_access_iterator_tag, T>
	{
		friend class PVector<T, A>;
	private:
		PVectorIterator(T** p) : p{ p } {}
	public:
		using value_type = T;
		using difference_type = Index;
		using pointer = T * ;
		using reference = T & ;

		PVectorIterator(const PVectorIterator& it) noexcept : p{ it.p } {}

		inline bool operator!=(PVectorIterator const& other) const noexcept { return p != other.p; }
		inline bool operator==(PVectorIterator const& other) const noexcept { return p == other.p; }
		inline T& operator*() const noexcept { return *(*p); }
		inline T* operator->()const noexcept { return *p; }

		inline PVectorIterator operator++() noexcept
		{ // preincrement
			++p;
			return *this;
		}
		inline PVectorIterator operator++(int) noexcept
		{ // postincrement
			auto tmp_it = *this;
			++*this;
			return tmp_it;
		}

		inline PVectorIterator operator--() noexcept
		{ // predecrement
			--p;
			return *this;
		}
		inline PVectorIterator operator--(int) noexcept
		{ // postdecrement
			auto tmp_it = *this;
			--*this;
			return tmp_it;
		}

		inline PVectorIterator operator+(difference_type val) const noexcept { return PVectorIterator(p + val); }
		inline PVectorIterator operator+=(difference_type val) noexcept { p += val; return *this; }

		inline PVectorIterator operator-(difference_type val) const noexcept { return PVectorIterator(p - val); }
		inline PVectorIterator operator-=(difference_type val) noexcept { p -= val; return *this; }

		inline difference_type operator-(const PVectorIterator& other) const noexcept { return this->p - other.p; }

	private:
		T** p{ nullptr };
	};

	template<class T, class A>
	class PVector<T, A>::ConstPVectorIterator : public std::iterator<std::random_access_iterator_tag, T>
	{
		friend class PVector<T, A>;
		class PVector<T, A>::PVectorIterator;
	private:
		ConstPVectorIterator(T** p) noexcept : p{ p } {}
	public:
		using value_type = T;
		using difference_type = int;
		using pointer = T * ;
		using reference = T & ;

		ConstPVectorIterator(const ConstPVectorIterator& it) noexcept : p{ it.p } {}
		ConstPVectorIterator(const PVector<T, A>::PVectorIterator& it) noexcept : p{ it.p } {}

		inline bool operator!=(ConstPVectorIterator const& other) const noexcept { return p != other.p; }
		inline bool operator==(ConstPVectorIterator const& other) const noexcept { return p == other.p; }
		inline const T& operator*() const noexcept { return *(*p); }
		inline const T* operator->() const noexcept { return *p; }

		inline ConstPVectorIterator operator++() noexcept
		{ // preincrement
			++p;
			return *this;
		}
		inline ConstPVectorIterator operator++(int) noexcept
		{ // postincrement
			auto tmp_it = *this;
			++*this;
			return tmp_it;
		}

		inline ConstPVectorIterator operator--() noexcept
		{ // predecrement
			--p;
			return *this;
		}
		inline ConstPVectorIterator operator--(int) noexcept
		{ // postdecrement
			auto tmp_it = *this;
			--*this;
			return tmp_it;
		}

		inline ConstPVectorIterator operator+(difference_type val) const noexcept { return ConstPVectorIterator(p + val); }
		inline ConstPVectorIterator operator+=(difference_type val) noexcept { p += val; return *this; }

		inline ConstPVectorIterator operator-(difference_type val) const noexcept { return ConstPVectorIterator(p - val); }
		inline ConstPVectorIterator operator-=(difference_type val) noexcept { p -= val; return *this; }

		inline difference_type operator-(const ConstPVectorIterator& other) const noexcept { return this->p - other.p; }

	private:
		T** p{ nullptr };
	};
}

#endif // VECTOR_REF
