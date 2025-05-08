# Advanced Programming - HW4
<p  align="center"> <b>Homework 4 - Spring 2022 Semester <br> Deadline: Tuesday Ordibehesht 13st - 11:59 pm</b> </p>

## Outline

In this homework we are going to implement our own smart pointers. Specifically we want to implement our custom `SharedPtr` and `UniquePtr` classes with almost all functionality of std::shared_ptr and std::unique_ptr.

We want to implement 2 class templates called `UniquePtr` and `SharedPtr` with the functions described in the following sections.

**note.** You are only allowed  to alter `unique_ptr.hpp/h`, `shared_ptr.hpp/h` and only the debug section of `main.cpp`. 

</br>

# UniquePtr Class
Define a template class named `UniquePtr` and add the following functions to the class.

This class should use a member variable called `T* _p` (T is a template argument) to store a given pointer.

- **Constructor**
Implement a constructor for your class so the below code would work. Your constructor should be able to store the given dynamic pointer properly inside the class using `_p` variable.

	```cpp
	UniquePtr<int> ptr{new int{10}};
	```

- **make_unique** (outside the class)
The prefered way to construct a  std::unique_ptr is to use a function called `std::make_unique`. implement a similar function and make this code work:

	```cpp
	UniquePtr<int> ptr{make_unique<int>(10)};
	```

- **Default Constructor**
Implement a default constructor for your class so the below code works and assign `nullptr` to `_p`.

	```cpp
	UniquePtr<int> ptr;
	```
- **Destructor**
As you know when dealing a with dynamic pointer in a class, implementing destructor is a neccessaty so implement a proper destructor and delete your dynamic pointer (*hint:* assign `nullptr` after deletion).

	```cpp
	~UniquePtr()
	```
- **Copy Constructor**
As you already know you cannot copy a UniquePtr, make arrangements so the following code would cause a compile error.

	```cpp
	UniquePtr<int> ptr1{new int{10}};
	UniquePtr<int> ptr2{ptr1};
	```

- **Operator=**
Exactly like the previous section we should not be able to write the following code as well. Make the copiler to produce an error for this code.

	```cpp
	UniquePtr<int> ptr1{new int{10}};
	UniquePtr<int> ptr2{new int{11}};
	ptr2 = ptr1;
	```
- **get**
The get() function should return the raw pointer stored in the class.

	```cpp
	UniquePtr<int> ptr{new int{10}};
	std::cout << ptr.get() << std::endl; // output: raw pointer stored in the class
	```
- **Operator***
Smart pointers should be able to be dereferenced exactly like raw pointers. make this code work:

	```cpp
	UniquePtr<int> ptr{new int{10}};
	std::cout << *ptr << std::endl; // output: 10
	```
- **Operator->**
Smart pointers can use the arrow operator like normal pointers. make this code work as well:

	```cpp
	UniquePtr<std::string> ptr{new std::string{"hello"}};
	std::cout << ptr->length() << std::endl; // output: 5
	```

- **reset**
The reset() function will delete the ponter and assign `nullptr` to it:

	```cpp
	void reset();
	```

- **reset**
The reset() function can have a input and make a new pointer with it after deleting the old pointer:

	```cpp
	UniquePtr<std::string> ptr{new std::string{"hello"}};
    ptr.reset(new std::string{"nice"});
	std::cout << *ptr << std::endl; // output: nice
	```
- **release**
The release() function returns the stored pointer in the class (like get) with two differences: The UniquePtr class won't store  the pointer anymore and also deleting the pointer should **not** be done by UniquePtr class after calling release().

	```cpp
	UniquePtr<double> ptr{new double{1.567}};
    double *tmp{ptr.release()};
	std::cout << *tmp << std::endl; // output: 1.567
    delete tmp; // manual deletion
	```


</br>

# SharedPtr Class
Define a template class named `SharedPtr` and add the following functions to the class.

This class should use a member variable called `T* _p` (T is a template argument) to store a given pointer.


- **Constructor**
Implement a constructor for your class so the below code would work. Your constructor should be able to store the given dynamic pointer properly inside the class using `_p` variable.

	```cpp
	UniquePtr<int> ptr{new int{10}};
	```

- **make_shared** (outside the class)
The prefered way to construct a  std::shared_ptr is to use a function called `std::make_shared`. implement a similar function and make the code below work.

	```cpp
	SharedPtr<int> ptr{make_shared<int>(10)};
	```

- **Default Constructor**
Implement a default constructor for your class so the below code works and assign nullptr to `_p`.

	```cpp
	SharedPtr<int> ptr;
	```
- **Destructor**
As you know when dealing with dynamic pointers inside a class implementing destructor is a neccessaty so implement a proper destructor and delete your dynamic pointers (do not forget to assign `nullptr` after deletion).

	```cpp
	~SharedPtr()
	```

- **Copy Constructor**
As you already know A key difference between `SharedPtr` and `UniquePtr` classes is that we can use copy constrctor and make a copy of `SharedPtr`s. so the code below should run smoothly.

	```cpp
	SharedPtr<int> ptr1{new int{10}};
	SharedPtr<int> ptr2{ptr1};
	```

- **Operator=**
Exactly like the previous section we can have operator= for `SharedPtr`s. again the code below should run without any errors.

	```cpp
	SharedPtr<int> ptr1{new int{10}};
	SharedPtr<int> ptr2{new int{11}};
	ptr2 = ptr1;
	```

- **use_count**
In `SharedPtr`s we should have the ability to count the number of instances pointing to a same place. to do this you have to define another member variabel for your `SharedPtr` class and keep track of this number.

	**note.** you may have to make some adjusments in the previous functions (such as constructor and ...) to do this.

	```cpp
	SharedPtr<int> ptr1{make_shared<int>(10)};
	std::cout << ptr1.use_count() << std::endl; // output: 1
    SharedPtr<int> ptr2{ptr1};
	std::cout << ptr1.use_count() << std::endl; // output: 2
	std::cout << ptr2.use_count() << std::endl; // output: 2
	```

- **get**
The get() function should return the raw pointer stored in the class.

	```cpp
	SharedPtr<int> ptr{new int{10}};
	std::cout << ptr.get() << std::endl; // output: raw pointer of the class
	```
- **Operator***
Smart pointers should be able to be dereferenced exactly like raw pointers. make this code work:

	```cpp
	SharedPtr<int> ptr{new int{10}};
	std::cout << *ptr << std::endl; // output: 10
	```
- **Operator->**
Smart pointers can use the arrow operator like raw pointers. make this code work:

	```cpp
	SharedPtr<std::string> ptr{new std::string{"hello"}};
	std::cout << ptr->length() << std::endl; // output: 5
	```

- **reset**
The reset() function will delete the pointer and assigns `nullptr` to it:

	```cpp
	void reset();
	```

- **reset**
The reset() function can have a input and make a new pointer with it after deleting the old pointer:

	```cpp
	SharedPtr<std::string> ptr{new std::string{"hello"}};
    ptr.reset(new std::string{"nice"});
	std::cout << *ptr << std::endl; // output: nice
	```

</br> 
 
# Challenge
- If you reached this section congratulations, there is only one part left. Make arrangements so you can use your custom smart pointers in an *if condition*, the condition should return *false* if your smart pointer contains a `nullptr` and otherwise it should return *true*.


	```cpp
	UniquePtr<double> ptr{new double{1.567}};
    if(ptr) // => true
        // something
	ptr.reset();
	if(ptr) // => false
        // some other thing
	```
	Make this arrangement for both `UniquePtr` and `SharedPtr` classes.
</br>

# Finally
As mentioned before, do not alter other files already populated except otherwise indicated. In case you want to test your code you may use the `debug` section of the `main.cpp`.

```cpp
if (true) // make false to run unit tests  
{ 
	// debug section 
}  
else  
{  
	::testing::InitGoogleTest(&argc, argv);  
	std::cout << "RUNNING TESTS ..." << std::endl;  
	int ret{RUN_ALL_TESTS()};  
	if (!ret)  
		std::cout << "<<<SUCCESS>>>" << std::endl;  
	else  
	  std::cout << "FAILED" << std::endl;  
}  
return 0;
```
<br/>
<p  align="center"> <b>GOOD LUCK</b> </p>

```
RUNNING TESTS ...
[==========] Running 21 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 21 tests from HW4Test
[ RUN      ] HW4Test.TEST1
[       OK ] HW4Test.TEST1 (0 ms)
[ RUN      ] HW4Test.TEST2
[       OK ] HW4Test.TEST2 (0 ms)
[ RUN      ] HW4Test.TEST3
[       OK ] HW4Test.TEST3 (0 ms)
[ RUN      ] HW4Test.TEST4
[       OK ] HW4Test.TEST4 (0 ms)
[ RUN      ] HW4Test.TEST5
[       OK ] HW4Test.TEST5 (0 ms)
[ RUN      ] HW4Test.TEST6
[       OK ] HW4Test.TEST6 (0 ms)
[ RUN      ] HW4Test.TEST7
[       OK ] HW4Test.TEST7 (0 ms)
[ RUN      ] HW4Test.TEST8
[       OK ] HW4Test.TEST8 (0 ms)
[ RUN      ] HW4Test.TEST9
[       OK ] HW4Test.TEST9 (0 ms)
[ RUN      ] HW4Test.TEST10
[       OK ] HW4Test.TEST10 (0 ms)
[ RUN      ] HW4Test.TEST11
[       OK ] HW4Test.TEST11 (0 ms)
[ RUN      ] HW4Test.TEST12
[       OK ] HW4Test.TEST12 (0 ms)
[ RUN      ] HW4Test.TEST13
[       OK ] HW4Test.TEST13 (0 ms)
[ RUN      ] HW4Test.TEST14
[       OK ] HW4Test.TEST14 (0 ms)
[ RUN      ] HW4Test.TEST15
[       OK ] HW4Test.TEST15 (0 ms)
[ RUN      ] HW4Test.TEST16
[       OK ] HW4Test.TEST16 (0 ms)
[ RUN      ] HW4Test.TEST17
[       OK ] HW4Test.TEST17 (0 ms)
[ RUN      ] HW4Test.TEST18
[       OK ] HW4Test.TEST18 (0 ms)
[ RUN      ] HW4Test.TEST19
[       OK ] HW4Test.TEST19 (0 ms)
[ RUN      ] HW4Test.TEST20
[       OK ] HW4Test.TEST20 (0 ms)
[ RUN      ] HW4Test.TEST21
[       OK ] HW4Test.TEST21 (0 ms)
[----------] 21 tests from HW4Test (12 ms total)

[----------] Global test environment tear-down
[==========] 21 tests from 1 test suite ran. (13 ms total)
[  PASSED  ] 21 tests.
<<<SUCCESS>>>
```