#include <iostream>
#include "Map.h"
int main()
{
	Map<int, int> map;
	map.insert(1, 1);
	map.insert(2, 2);
	map.insert(3, 3);
	map.insert(4, 4);
	map.insert(5, 5);
	map.insert(6, 6);
	map.insert(7, 7);
	map.insert(8, 8);
	map.insert(9, 9);


	map.print();
}