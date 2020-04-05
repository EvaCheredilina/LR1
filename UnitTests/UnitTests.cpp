#include "pch.h"
#include "CppUnitTest.h"
#include "..//LR1/Map.h"
#include "..//LR1/List.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(testInsertFunctionClearWork)
		{
			Map<int, int> map;

			map.insert(1, 1);
			map.insert(2, 2);
			map.insert(3, 3);
			map.insert(4, 4);
			map.insert(5, 5);

			assert(map.find(1) == 1);
			assert(map.find(2) == 2);
			assert(map.find(3) == 3);
			assert(map.find(4) == 4);
			assert(map.find(5) == 5);
		}

		TEST_METHOD(testFindFunctionException)
		{
			bool wasExceptionCatched = false;

			Map<int, int> map;

			try {
				map.find(1);
			}
			catch (...) {
				wasExceptionCatched = true;
			}

			assert(wasExceptionCatched == true);


			map.insert(2, 2);
			try {
				map.find(2);
				wasExceptionCatched = false;
			}
			catch (...)
			{
				wasExceptionCatched = true;
			}

			assert(wasExceptionCatched == false);


			map.insert(3, 3);
			try {
				map.find(4);
			}
			catch (...) {
				wasExceptionCatched = true;
			}

			assert(wasExceptionCatched == true);
		}

		TEST_METHOD(testClearFunction)
		{
			Map<int, int> map;
			map.insert(1, 1);
			map.insert(2, 2);
			map.insert(3, 3);
			map.insert(4, 4);

			map.clear();
			
			assert(map.get_keys().isEmpty() == true && map.get_values().isEmpty() == true);
		}

		TEST_METHOD(testRemoveFunction)
		{
			Map<int, int> map;
			map.insert(1, 1);
			map.insert(2, 2);
			map.insert(3, 3);
			map.insert(4, 4);
			map.insert(5, 5);

			map.remove(1);
			bool wasCatched = false;
			try {
				map.find(1);
			}
			catch (...)
			{
				wasCatched = true;
			}
			assert(wasCatched == true);


			map.remove(2);
			wasCatched = false;
			try {
				map.find(2);
			}
			catch (...)
			{
				wasCatched = true;
			}
			assert(wasCatched == true);


			map.remove(3);
			wasCatched = false;
			try {
				map.find(3);
			}
			catch (...)
			{
				wasCatched = true;
			}
			assert(wasCatched == true);

			map.remove(4);
			wasCatched = false;
			try {
				map.find(4);
			}
			catch (...)
			{
				wasCatched = true;
			}
			assert(wasCatched == true);

			map.remove(5);
			wasCatched = false;
			try {
				map.find(5);
			}
			catch (...)
			{
				wasCatched = true;
			}
			assert(wasCatched == true);


			assert(map.get_keys().isEmpty() == true && map.get_values().isEmpty() == true);
		}

	};
}
