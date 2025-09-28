# Exception Guarantees

## 1) No throw guarantee
Operation will not throw. C functions such as `fclose` or `std::swap(a,b)` can't throw.
All destructors of classes are implicitly no-throw according to modern C++. 
Move ctor and move assigment operator are desired to be _noexcept_ since they facilitate compiler to do make optimization(code selection in compile time).

## 2) Basic guarantee
If operation throws, no resource is leaked. Object invariants are not corrupted, but operation can be half done.  
Ex:
```cpp
std::vector::push_back(a); // either element will be added or not.
```

## 3) Strong guarantee(commit or rollback)
Can be considered as: Basic guarantee + operation will either succeed or be rolled back.  
Ex:
```cpp
void halfDoneCopy(const std::vector<A>& in, std::vector<A>& out) // if copy ctor will throw, only some elements will be copied
{
	for(const auto& a : in)
	{
		out,push_back(a);
	}
}
```
## 4) No exception safety
If operation throws exceptions bad thing might happen. Segfaults, memory corruption etc.  
Ex:
```cpp
void char* noSafety(const A& a)
{
	char* buffer = new char[100]; / if fill() will throw we have a memory leak
	a->fill(buffer);
	return buffer;
}
```
