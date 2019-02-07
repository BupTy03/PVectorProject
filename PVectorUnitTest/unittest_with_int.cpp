#include "stdafx.h"
#include "CppUnitTest.h"
#include"../PVectorProject/PVector.hpp"

#include<vector>
#include<stdexcept>
#include<numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestPVector
{
	TEST_CLASS(UnitTestWithInt)
	{
	public:

		TEST_METHOD(ConstructorAndDestructorTest)
		{
			my::PVector<int> vec;
		}

		TEST_METHOD(ConstructorWithSizeTest)
		{
			my::PVector<int> vec(100);
		}

		TEST_METHOD(ConstructorWithValueTest)
		{
			my::PVector<int> vec(100, -5);
			for (int i = 0; i < vec.size(); ++i)
				Assert::AreEqual(vec[i], -5);
		}

		TEST_METHOD(CopyConstructorTest)
		{
			constexpr int sz = 100;
			my::PVector<int> vec(sz);

			std::iota(std::begin(vec), std::end(vec), 1);

			Assert::AreEqual(vec.size(), sz);
			Assert::AreEqual(vec.capacity(), sz);

			my::PVector<int> vec2(vec);
			Assert::AreEqual(vec2.size(), sz);
			Assert::AreEqual(vec2.capacity(), sz);

			for (int i = 0; i < vec.size(); ++i)
				Assert::AreEqual(vec[i], vec2[i]);
		}

		TEST_METHOD(AssignmentOperatorTest)
		{
			constexpr int sz = 100;
			my::PVector<int> vec(sz);
			std::iota(std::begin(vec), std::end(vec), 1);

			Assert::AreEqual(vec.size(), sz);
			Assert::AreEqual(vec.capacity(), sz);

			my::PVector<int> vec2;
			Assert::AreEqual(vec2.size(), 0);
			Assert::AreEqual(vec2.capacity(), 0);

			vec = vec2;
			Assert::AreEqual(vec.size(), 0);
			Assert::AreEqual(vec.capacity(), 0);

			constexpr int newsize = 55;
			my::PVector<int> vec3(newsize, 1);
			std::iota(std::begin(vec3), std::end(vec3), -10);

			vec = vec3;
			Assert::AreEqual(vec.size(), newsize);
			Assert::AreEqual(vec.capacity(), newsize);

			vec2 = vec;
			Assert::AreEqual(vec2.size(), newsize);
			Assert::AreEqual(vec2.capacity(), newsize);

			for (int i = 0; i < newsize; ++i)
			{
				Assert::AreEqual(vec[i], vec2[i]);
				Assert::AreEqual(vec2[i], vec3[i]);
			}
		}

		TEST_METHOD(RBForTest)
		{
			my::PVector<int> vec(10);
			std::iota(std::begin(vec), std::end(vec), 1);
			int counter{ 0 };
			for (int i : vec)
			{
				Assert::AreEqual(i, ++counter);
			}
		}

		TEST_METHOD(AtTest)
		{
			my::PVector<int> vec(3);
			try
			{
				vec.at(3);
			}
			catch (const std::out_of_range& ex)
			{
				return;
			}

			Assert::Fail(L"Exception wasn't generated std::out_of_range expected");
		}

		TEST_METHOD(FrontTest)
		{
			my::PVector<int> vec(10);
			std::iota(std::begin(vec), std::end(vec), 1);
			Assert::AreEqual(vec.at(0), vec.front());
		}

		TEST_METHOD(BackTest)
		{
			my::PVector<int> vec(10);
			std::iota(std::begin(vec), std::end(vec), 1);
			Assert::AreEqual(vec.at(vec.size() - 1), vec.back());
		}

		TEST_METHOD(ReserveTest)
		{
			my::PVector<int> vec(10, 1);
			vec.reserve(20);
			Assert::AreEqual(vec.capacity(), 20);
		}

		TEST_METHOD(ResizeTest)
		{
			constexpr int sz = 10;
			my::PVector<int> vec(sz, 1);
			const int cap = vec.capacity();

			constexpr int newsz = 5;
			vec.resize(newsz);
			Assert::AreEqual(newsz, vec.size());
			Assert::AreEqual(cap, vec.capacity());

			vec.resize(sz);
			Assert::AreEqual(sz, vec.size());
			Assert::AreEqual(cap, vec.capacity());

			constexpr int newsz2 = sz * 2;
			vec.resize(newsz2, -3);
			Assert::AreEqual(newsz2, vec.size());
			Assert::AreNotEqual(cap, vec.capacity());
		}

		TEST_METHOD(InsertValTest)
		{
			constexpr int sz = 10;

			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			auto pit = pvec.insert(std::cbegin(pvec), -2);
			auto it = vec.insert(std::cbegin(vec), -2);

			Assert::AreEqual(*pit, *it);

			auto pit2 = pvec.insert(std::cend(pvec), -5);
			auto it2 = vec.insert(std::cend(vec), -5);

			Assert::AreEqual(*pit2, *it2);

			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, -1);
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, -1);

			Assert::AreEqual(*pit3, *it3);

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(pvec.at(i), vec.at(i));
			}
		}

		TEST_METHOD(InsertValCountTest)
		{
			constexpr int sz = 50;

			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			auto pit = pvec.insert(std::cbegin(pvec), 3, -2);
			auto it = vec.insert(std::cbegin(vec), 3, -2);

			Assert::AreEqual(*pit, *it);

			auto pit2 = pvec.insert(std::cend(pvec), 4, -5);
			auto it2 = vec.insert(std::cend(vec), 4, -5);

			Assert::AreEqual(*pit2, *it2);

			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, 10, -1);
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, 10, -1);

			Assert::AreEqual(*pit3, *it3);

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(pvec.at(i), vec.at(i));
			}
		}

		TEST_METHOD(InsertRangeTest)
		{
			constexpr int sz = 50;

			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			std::vector<int> insvec = { -1,-2,-3 };
			auto pit = pvec.insert(std::cbegin(pvec), std::cbegin(insvec), std::cend(insvec));
			auto it = vec.insert(std::cbegin(vec), std::cbegin(insvec), std::cend(insvec));

			Assert::AreEqual(*pit, *it);

			std::vector<int> insvec2 = { -7, -8, -9, -1 };
			auto pit2 = pvec.insert(std::cend(pvec), std::cbegin(insvec2), std::cend(insvec2));
			auto it2 = vec.insert(std::cend(vec), std::cbegin(insvec2), std::cend(insvec2));

			Assert::AreEqual(*pit2, *it2);

			std::vector<int> insvec3 = { -1, -12, -23, -15, -5 };
			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, std::cbegin(insvec3), std::cend(insvec3));
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, std::cbegin(insvec3), std::cend(insvec3));

			Assert::AreEqual(*pit3, *it3);

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(pvec.at(i), vec.at(i));
			}
		}

		TEST_METHOD(InsertInitListTest)
		{
			constexpr int sz = 50;

			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			auto pit = pvec.insert(std::cbegin(pvec), { -1,-2,-3 });
			auto it = vec.insert(std::cbegin(vec), { -1,-2,-3 });

			Assert::AreEqual(*pit, *it);

			auto pit2 = pvec.insert(std::cend(pvec), { -7, -8, -9, -1 });
			auto it2 = vec.insert(std::cend(vec), { -7, -8, -9, -1 });

			Assert::AreEqual(*pit2, *it2);

			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, { -1, -12, -23, -15, -5 });
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, { -1, -12, -23, -15, -5 });

			Assert::AreEqual(*pit3, *it3);

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(pvec.at(i), vec.at(i));
			}
		}

		TEST_METHOD(EmplaceTest)
		{
			constexpr int sz = 10;

			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			auto pit = pvec.emplace(std::cbegin(pvec), -2);
			auto it = vec.emplace(std::cbegin(vec), -2);

			Assert::AreEqual(*pit, *it);

			auto pit2 = pvec.emplace(std::cend(pvec), -5);
			auto it2 = vec.emplace(std::cend(vec), -5);

			Assert::AreEqual(*pit2, *it2);

			auto pit3 = pvec.emplace(std::cbegin(pvec) + sz / 2, -1);
			auto it3 = vec.emplace(std::cbegin(vec) + sz / 2, -1);

			Assert::AreEqual(*pit3, *it3);

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(pvec.at(i), vec.at(i));
			}
		}

		TEST_METHOD(EraseValTest)
		{
			constexpr int sz = 50;

			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			auto pit = pvec.erase(std::cbegin(pvec));
			auto it = vec.erase(std::cbegin(vec));

			Assert::AreEqual(*pit, *it);

			auto pit2 = pvec.erase(std::cend(pvec));

			Assert::IsTrue(pit2 == std::end(pvec));

			auto pit3 = pvec.erase(std::cbegin(pvec) + sz / 2);
			auto it3 = vec.erase(std::cbegin(vec) + sz / 2);

			Assert::AreEqual(*pit3, *it3);

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(pvec.at(i), vec.at(i));
			}
		}

		TEST_METHOD(EraseRangeTest)
		{
			constexpr int sz = 50;

			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			auto pit = pvec.erase(std::cbegin(pvec) + std::size(pvec) / 4, std::cend(pvec) - std::size(pvec) / 4);
			auto it = vec.erase(std::cbegin(vec) + std::size(vec) / 4, std::cend(vec) - std::size(vec) / 4);

			Assert::AreEqual(*pit, *it);
			Assert::IsTrue(std::size(pvec) == std::size(vec));

			auto pit3 = pvec.erase(std::cbegin(pvec) + std::size(pvec) / 2, std::cend(pvec));
			auto it3 = vec.erase(std::cbegin(vec) + std::size(vec) / 2, std::cend(vec));

			Assert::IsTrue(pit3 == std::end(pvec), L"pit3 != std::end(pvec)");
			Assert::IsTrue(it3 == std::end(vec));


			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(pvec.at(i), vec.at(i));
			}
		}

		TEST_METHOD(PushBackTest)
		{
			my::PVector<int> pvec(8);
			std::vector<int> vec(8);

			constexpr int count = 20;
			for (int i = 0; i < count; ++i)
			{
				pvec.push_back(i);
				vec.push_back(i);
			}

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}
		}

		TEST_METHOD(EmplaceBackTest)
		{
			my::PVector<int> pvec(10, -1);
			std::vector<int> vec(10, -1);

			constexpr int count = 20;
			for (int i = 0; i < count; ++i)
			{
				pvec.emplace_back(i);
				vec.emplace_back(i);
			}

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}
		}

		TEST_METHOD(PopBackTest)
		{
			constexpr int sz = 50;
			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			for (int i = 0; i < sz / 2; ++i)
			{
				pvec.pop_back();
				vec.pop_back();
			}

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}
		}

		TEST_METHOD(AssignValTest)
		{
			constexpr int sz = 50;
			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			pvec.assign(sz / 4, -1);
			vec.assign(sz / 4, -1);

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}
		}

		TEST_METHOD(AssignRangeTest)
		{
			const std::vector<int> control = { -3, 4, -2, -1, 99, -35, 98 };

			my::PVector<int> pvec(std::size(control));
			std::vector<int> vec(std::size(control));

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			pvec.assign(std::cbegin(control), std::cend(control));
			vec.assign(std::cbegin(control), std::cend(control));

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}
		}

		TEST_METHOD(ShrinkToFitTest)
		{
			my::PVector<int> pvec;

			constexpr int sz = 25;
			for (int i = 0; i < sz; ++i)
			{
				pvec.push_back(i);
			}

			Assert::IsTrue(pvec.capacity() >= pvec.size());

			pvec.shrink_to_fit();

			Assert::AreEqual(pvec.capacity(), pvec.size());
		}

		TEST_METHOD(ClearTest)
		{
			my::PVector<int> pvec(10, 1);
			pvec.clear();
			Assert::AreEqual(pvec.size(), 0);
			Assert::AreEqual(pvec.capacity(), 0);
		}

		TEST_METHOD(SwapTest)
		{
			constexpr int sz = 30;
			my::PVector<int> control1(sz);
			my::PVector<int> control2(sz);

			std::iota(std::begin(control1), std::end(control1), 1);
			std::iota(std::begin(control2), std::end(control2), 1);

			my::PVector<int> pvec1 = control1;
			my::PVector<int> pvec2 = control2;

			pvec1.swap(pvec2);

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual(control1.at(i), pvec2.at(i));
				Assert::AreEqual(control2.at(i), pvec1.at(i));

			}
		}

		TEST_METHOD(SortsTest)
		{
			constexpr int sz = 50;
			my::PVector<int> pvec(sz);
			std::vector<int> vec(sz);

			std::iota(std::begin(pvec), std::end(pvec), 1);
			std::iota(std::begin(vec), std::end(vec), 1);

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			// sort() test
			pvec.sort();
			std::sort(std::begin(vec), std::end(vec));

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			pvec.sort(std::greater<int>());
			std::sort(std::begin(vec), std::end(vec), std::greater<int>());

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			// partial_sort() test
			pvec.partial_sort(std::begin(pvec) + std::size(pvec) / 2);
			std::partial_sort(std::begin(vec), std::begin(vec) + std::size(vec) / 2, std::end(vec));

			for (int i = 0; i < sz / 2; ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			pvec.partial_sort(std::begin(pvec) + std::size(pvec) / 4, std::greater<int>());
			std::partial_sort(std::begin(vec), std::begin(vec) + std::size(vec) / 4, std::end(vec), std::greater<int>());

			for (int i = 0; i < sz / 4; ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			// stable_sort() test
			pvec.stable_sort();
			std::stable_sort(std::begin(vec), std::end(vec));

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			pvec.stable_sort(std::greater<int>());
			std::stable_sort(std::begin(vec), std::end(vec), std::greater<int>());

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual(vec.at(i), pvec.at(i));
			}

			Assert::IsTrue(std::is_sorted(pvec.ptr_begin(), pvec.ptr_end(), my::PVectorPredicateWrapper<int, std::greater<int>>()));
		}
	};
}