/* This code snippet shows the technique that STL uses which allow algorithms being able to take not only template containers(vector, list etc.)
but also C-type arrays as range parameters(beg, end) employing IteratorTraits struct */

#include <iostream>
#include <vector>

using namespace std;

template<typename iter>
struct IteratorTraits
{
	using value_type = typename iter::value_type;
	using reference = typename iter::pointer;
	using pointer = typename iter::reference;
};

template<typename T> // Partial specialization for arrays(pointers) 
struct IteratorTraits<T*>
{
	using value_type = T;
	using pointer = T*;
	using reference = T&;
};

template<typename InIter>
void func(InIter beg, InIter end)
{
	typename IteratorTraits<InIter>::value_type v{}; // Need to use IteratorTraits<InIter>::value_type, not InIter::value_type
	cout << "value type of the container: " << typeid(v).name() << '\n';
	while (beg != end)
		cout << *beg++ << ' ';
	cout << '\n';
}

int main()
{
	int arr[5]{1, 2, 3, 4, 5};
	vector<char> ivec{'a', 'b', 'c', 'd', 'e'};

	func(arr, arr + 5);
	func(ivec.begin(), ivec.end());
}
