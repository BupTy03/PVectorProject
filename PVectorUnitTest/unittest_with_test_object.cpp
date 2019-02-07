#include "stdafx.h"
#include "CppUnitTest.h"
#include"../PVectorProject/PVector.hpp"
#include"../PVectorProject/TestObject.hpp"

#include<iostream>
#include<vector>
#include<stdexcept>
#include<numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestPVector
{
	TEST_CLASS(UnitTestWithTestObject)
	{
	public:

		TEST_METHOD(ConstructorAndDestructorTest)
		{
			my::PVector<my::TestObject> vec;
		}

		TEST_METHOD(ConstructorWithSizeTest)
		{
			my::PVector<my::TestObject> vec(100);
		}

		TEST_METHOD(ConstructorWithValueTest)
		{
			my::PVector<my::TestObject> vec(100, my::TestObject(-5));
			for (int i = 0; i < vec.size(); ++i)
				Assert::AreEqual((vec.at(i)).getValue(), -5);
		}

		TEST_METHOD(CopyConstructorTest)
		{
			constexpr int sz = 100;
			my::PVector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
				(vec.at(i)).setValue(i);

			Assert::AreEqual(vec.size(), sz);
			Assert::AreEqual(vec.capacity(), sz);

			my::PVector<my::TestObject> vec2(vec);
			Assert::AreEqual(vec2.size(), sz);
			Assert::AreEqual(vec2.capacity(), sz);

			for (int i = 0; i < vec.size(); ++i)
				Assert::AreEqual((vec.at(i)).getValue(), (vec2.at(i)).getValue());
		}

		TEST_METHOD(AssignmentOperatorTest)
		{
			constexpr int sz = 100;
			my::PVector<my::TestObject> vec(sz);
			for (int i = 0; i < vec.size(); ++i)
				(vec.at(i)).setValue(i);

			Assert::AreEqual(vec.size(), sz);
			Assert::AreEqual(vec.capacity(), sz);

			my::PVector<my::TestObject> vec2;
			Assert::AreEqual(vec2.size(), 0);
			Assert::AreEqual(vec2.capacity(), 0);

			vec = vec2;
			Assert::AreEqual(vec.size(), 0);
			Assert::AreEqual(vec.capacity(), 0);

			constexpr int newsize = 55;
			my::PVector<my::TestObject> vec3(newsize, 1);
			for (int i = 0; i < vec.size(); ++i)
				(vec.at(i)).setValue(i);

			vec = vec3;
			Assert::AreEqual(vec.size(), newsize);
			Assert::AreEqual(vec.capacity(), newsize);

			vec2 = vec;
			Assert::AreEqual(vec2.size(), newsize);
			Assert::AreEqual(vec2.capacity(), newsize);

			for (int i = 0; i < newsize; ++i)
			{
				Assert::AreEqual((vec.at(i)).getValue(), (vec2.at(i)).getValue());
				Assert::AreEqual((vec2.at(i)).getValue(), (vec3.at(i)).getValue());
			}
		}

		TEST_METHOD(RBForTest)
		{
			my::PVector<my::TestObject> vec(10);
			int counter{ 0 };
			for (auto& obj : vec)
			{
				obj.setValue(counter);
				counter++;
			}

			counter = 0;
			for (const auto& obj : vec)
			{
				Assert::AreEqual(obj.getValue(), counter);
				counter++;
			}
		}

		TEST_METHOD(AtTest)
		{
			my::PVector<my::TestObject> vec(3);
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
			my::PVector<my::TestObject> vec(10);
			for (int i = 0; i < vec.size(); ++i)
				(vec.at(i)).setValue(i);

			Assert::AreEqual((vec.at(0)).getValue(), (vec.front()).getValue());
		}

		TEST_METHOD(BackTest)
		{
			my::PVector<my::TestObject> vec(10);
			for (int i = 0; i < vec.size(); ++i)
				(vec.at(i)).setValue(i);

			Assert::AreEqual((vec.at(vec.size() - 1)).getValue(), (vec.back()).getValue());
		}

		TEST_METHOD(ReserveTest)
		{
			constexpr int sz = 10;
			my::PVector<my::TestObject> vec(sz, my::TestObject(1));
			vec.reserve(sz * 2);
			Assert::AreEqual(vec.capacity(), sz * 2);
		}

		TEST_METHOD(ResizeTest)
		{
			constexpr int sz = 10;
			my::PVector<my::TestObject> vec(sz, my::TestObject(1));
			const int cap = vec.capacity();

			constexpr int newsz = sz / 2;
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

			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			auto pit = pvec.insert(std::cbegin(pvec), my::TestObject(-2));
			auto it = vec.insert(std::cbegin(vec), my::TestObject(-2));

			Assert::AreEqual((*pit).getValue(), (*it).getValue());

			auto pit2 = pvec.insert(std::cend(pvec), my::TestObject(-5));
			auto it2 = vec.insert(std::cend(vec), my::TestObject(-5));

			Assert::AreEqual((*pit2).getValue(), (*it2).getValue());

			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, my::TestObject(-1));
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, my::TestObject(-1));

			Assert::AreEqual((*pit3).getValue(), (*it3).getValue());

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(InsertValCountTest)
		{
			constexpr int sz = 50;

			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			auto pit = pvec.insert(std::cbegin(pvec), 3, my::TestObject(-2));
			auto it = vec.insert(std::cbegin(vec), 3, my::TestObject(-2));

			Assert::AreEqual((*pit).getValue(), (*it).getValue());

			auto pit2 = pvec.insert(std::cend(pvec), 4, my::TestObject(-5));
			auto it2 = vec.insert(std::cend(vec), 4, my::TestObject(-5));

			Assert::AreEqual((*pit2).getValue(), (*it2).getValue());

			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, 10, my::TestObject(-1));
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, 10, my::TestObject(-1));

			Assert::AreEqual((*pit3).getValue(), (*it3).getValue());

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(InsertRangeTest)
		{
			constexpr int sz = 50;

			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			std::vector<my::TestObject> insvec = { my::TestObject(-1), my::TestObject(-2), my::TestObject(-3) };
			auto pit = pvec.insert(std::cbegin(pvec), std::cbegin(insvec), std::cend(insvec));
			auto it = vec.insert(std::cbegin(vec), std::cbegin(insvec), std::cend(insvec));

			Assert::AreEqual((*pit).getValue(), (*it).getValue());

			std::vector<my::TestObject> insvec2 = { my::TestObject(-7), my::TestObject(-8), my::TestObject(-9), my::TestObject(-1) };
			auto pit2 = pvec.insert(std::cend(pvec), std::cbegin(insvec2), std::cend(insvec2));
			auto it2 = vec.insert(std::cend(vec), std::cbegin(insvec2), std::cend(insvec2));

			Assert::AreEqual((*pit2).getValue(), (*it2).getValue());

			std::vector<my::TestObject> insvec3 = { my::TestObject(-1), my::TestObject(-12), my::TestObject(-23), my::TestObject(-15), my::TestObject(-5) };
			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, std::cbegin(insvec3), std::cend(insvec3));
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, std::cbegin(insvec3), std::cend(insvec3));

			Assert::AreEqual((*pit3).getValue(), (*it3).getValue());

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(InsertInitListTest)
		{
			constexpr int sz = 50;

			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			auto pit = pvec.insert(std::cbegin(pvec), { -1,-2,-3 });
			auto it = vec.insert(std::cbegin(vec), { -1,-2,-3 });

			Assert::AreEqual((*pit).getValue(), (*it).getValue());

			auto pit2 = pvec.insert(std::cend(pvec), { -7, -8, -9, -1 });
			auto it2 = vec.insert(std::cend(vec), { -7, -8, -9, -1 });

			Assert::AreEqual((*pit2).getValue(), (*it2).getValue());

			auto pit3 = pvec.insert(std::cbegin(pvec) + sz / 2, { -1, -12, -23, -15, -5 });
			auto it3 = vec.insert(std::cbegin(vec) + sz / 2, { -1, -12, -23, -15, -5 });

			Assert::AreEqual((*pit3).getValue(), (*it3).getValue());

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(EmplaceTest)
		{
			constexpr int sz = 10;

			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			auto pit = pvec.emplace(std::cbegin(pvec), -2);
			auto it = vec.emplace(std::cbegin(vec), -2);

			Assert::AreEqual((*pit).getValue(), (*it).getValue());

			auto pit2 = pvec.emplace(std::cend(pvec), -5);
			auto it2 = vec.emplace(std::cend(vec), -5);

			Assert::AreEqual((*pit2).getValue(), (*it2).getValue());

			auto pit3 = pvec.emplace(std::cbegin(pvec) + sz / 2, -1);
			auto it3 = vec.emplace(std::cbegin(vec) + sz / 2, -1);

			Assert::AreEqual((*pit3).getValue(), (*it3).getValue());

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(EraseValTest)
		{
			constexpr int sz = 50;

			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			auto pit = pvec.erase(std::cbegin(pvec));
			auto it = vec.erase(std::cbegin(vec));

			Assert::AreEqual((*pit).getValue(), (*it).getValue());

			auto pit2 = pvec.erase(std::cend(pvec));

			Assert::IsTrue(pit2 == std::end(pvec));

			auto pit3 = pvec.erase(std::cbegin(pvec) + sz / 2);
			auto it3 = vec.erase(std::cbegin(vec) + sz / 2);

			Assert::AreEqual((*pit3).getValue(), (*it3).getValue());

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(EraseRangeTest)
		{
			constexpr int sz = 50;

			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			auto pit = pvec.erase(std::cbegin(pvec) + std::size(pvec) / 4, std::cend(pvec) - std::size(pvec) / 4);
			auto it = vec.erase(std::cbegin(vec) + std::size(vec) / 4, std::cend(vec) - std::size(vec) / 4);

			Assert::AreEqual((*pit).getValue(), (*it).getValue());
			Assert::IsTrue(std::size(pvec) == std::size(vec));

			auto pit3 = pvec.erase(std::cbegin(pvec) + std::size(pvec) / 2, std::cend(pvec));
			auto it3 = vec.erase(std::cbegin(vec) + std::size(vec) / 2, std::cend(vec));

			Assert::IsTrue(pit3 == std::end(pvec), L"pit3 != std::end(pvec)");
			Assert::IsTrue(it3 == std::end(vec));


			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(PushBackTest)
		{
			my::PVector<my::TestObject> pvec(8);
			std::vector<my::TestObject> vec(8);

			constexpr int count = 20;
			for (int i = 0; i < count; ++i)
			{
				pvec.push_back(i);
				vec.push_back(i);
			}

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(EmplaceBackTest)
		{
			my::PVector<my::TestObject> pvec(10, -1);
			std::vector<my::TestObject> vec(10, -1);

			constexpr int count = 20;
			for (int i = 0; i < count; ++i)
			{
				pvec.emplace_back(i);
				vec.emplace_back(i);
			}

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(PopBackTest)
		{
			constexpr int sz = 50;
			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			for (int i = 0; i < sz / 2; ++i)
			{
				pvec.pop_back();
				vec.pop_back();
			}

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(AssignValTest)
		{
			constexpr int sz = 50;
			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < sz; ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			pvec.assign(sz / 4, my::TestObject(-1));
			vec.assign(sz / 4, my::TestObject(-1));

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
			}
		}

		TEST_METHOD(AssignRangeTest)
		{
			std::vector<my::TestObject> control =
			{ my::TestObject(-3), my::TestObject(4), my::TestObject(-2), my::TestObject(-1),
				my::TestObject(99), my::TestObject(-35), my::TestObject(98) };

			my::PVector<my::TestObject> pvec(std::size(control));
			std::vector<my::TestObject> vec(std::size(control));

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			pvec.assign(std::begin(control), std::end(control));
			vec.assign(std::begin(control), std::end(control));

			for (int i = 0; i < vec.size(); ++i)
			{
				Assert::AreEqual((pvec.at(i)).getValue(), (vec.at(i)).getValue());
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
			my::PVector<my::TestObject> pvec(10, my::TestObject(1));
			pvec.clear();
			Assert::AreEqual(pvec.size(), 0);
			Assert::AreEqual(pvec.capacity(), 0);
		}

		TEST_METHOD(SwapTest)
		{
			constexpr int sz = 30;
			my::PVector<my::TestObject> control1(sz);
			my::PVector<my::TestObject> control2(sz);

			for (int i = 0; i < sz; ++i)
			{
				(control1.at(i)).setValue(i);
				(control2.at(i)).setValue(i);
			}

			my::PVector<my::TestObject> pvec1 = control1;
			my::PVector<my::TestObject> pvec2 = control2;

			pvec1.swap(pvec2);

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual((control1.at(i)).getValue(), (pvec2.at(i)).getValue());
				Assert::AreEqual((control2.at(i)).getValue(), (pvec1.at(i)).getValue());
			}
		}

		TEST_METHOD(SortsTest)
		{
			constexpr int sz = 50;
			my::PVector<my::TestObject> pvec(sz);
			std::vector<my::TestObject> vec(sz);

			for (int i = 0; i < vec.size(); ++i)
			{
				(pvec.at(i)).setValue(i);
				(vec.at(i)).setValue(i);
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			// sort() test
			pvec.sort();
			std::sort(std::begin(vec), std::end(vec));

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual((vec.at(i)).getValue(), (pvec.at(i)).getValue());
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			pvec.sort(std::greater<my::TestObject>());
			std::sort(std::begin(vec), std::end(vec), std::greater<my::TestObject>());

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual((vec.at(i)).getValue(), (pvec.at(i)).getValue());
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			// partial_sort() test
			pvec.partial_sort(std::begin(pvec) + std::size(pvec) / 2);
			std::partial_sort(std::begin(vec), std::begin(vec) + std::size(vec) / 2, std::end(vec));

			for (int i = 0; i < sz / 2; ++i)
			{
				Assert::AreEqual((vec.at(i)).getValue(), (pvec.at(i)).getValue());
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			pvec.partial_sort(std::begin(pvec) + std::size(pvec) / 4, std::greater<my::TestObject>());
			std::partial_sort(std::begin(vec), std::begin(vec) + std::size(vec) / 4, std::end(vec), std::greater<my::TestObject>());

			for (int i = 0; i < sz / 4; ++i)
			{
				Assert::AreEqual((vec.at(i)).getValue(), (pvec.at(i)).getValue());
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			// stable_sort() test
			pvec.stable_sort();
			std::stable_sort(std::begin(vec), std::end(vec));

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual((vec.at(i)).getValue(), (pvec.at(i)).getValue());
			}

			std::random_shuffle(std::begin(pvec), std::end(pvec));
			std::random_shuffle(std::begin(vec), std::end(vec));

			pvec.stable_sort(std::greater<my::TestObject>());
			std::stable_sort(std::begin(vec), std::end(vec), std::greater<my::TestObject>());

			for (int i = 0; i < sz; ++i)
			{
				Assert::AreEqual((vec.at(i)).getValue(), (pvec.at(i)).getValue());
			}

			Assert::IsTrue(std::is_sorted(pvec.ptr_begin(), pvec.ptr_end(), my::PVectorPredicateWrapper<my::TestObject, std::greater<my::TestObject>>()));
		}
	};
}