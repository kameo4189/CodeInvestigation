## Contents

- [Snippet](#snippet)
- [Analysis](#analysis)
  - [Base class](#base-class)
    - [Problems](#problems)
    - [Solutions](#solutions)
  - [Child class](#child-class)
    - [Problems](#problems-1)
    - [Solutions](#solutions-1)
  - [main method](#main-method)
    - [Problems](#problems-2)
    - [Solutions](#solutions-2)
- [Conclusion](#conclusion)

# Snippet
```C++
class Base
{
public:
    explicit Base(unsigned num) : m_arr(new int[num])
    {
    }

    virtual void foo() {}

protected:
    int *m_arr;
};

class Child : public Base
{
public:
    explicit Child(unsigned num) : Base(num), m_arr(new char[10])
    {
    }

    virtual void foo() {}

private:
    char *m_arr;
};

int main()
{
    Base b(10);
    Base *b_ptr = new Base(5);
    b_ptr = new Child(7);
    b_ptr->foo();
    delete b_ptr;
    return 0;
}
```
# Analysis
## Base class
### Problems
- Attribute *m_arr*:
  - Use raw pointer of use Old C++ for initializing *int* array in constructor.
  - Not release when object of Base class is destroyed, it can cause memory leak.
- Argument *num* in constructor has unclear meaning name.
### Solutions
- Attribute *m_arr*:
  - If using Old C++, need add destructor and code for release *m_arr*.
  ```C++
  public:
    virtual ~Base()
    {
        delete []m_arr;
    }
  ```
  - If using Modern C++:
    - If want to keep *m_arr* is pointer, change type of *m_arr* to *shared_ptr\<int[]\>* and no need to add code for release memory. But this solution is only for some certain purposes because there are some inconveniences when using pointer as array. Constructor will be changed as below.
        ```C++
        explicit Base(unsigned num) : m_arr(new int[numInt])
        {
        }
        ```
    - The common way is to change type of *m_arr* to *vector\<int\>*, this type is replaced for dynamic array with many built-in methods for array. No need to change code in constructor.
- Argument *num* in constructor: change name to *numInt*.

**→ Final solution:**
- **Change change type of *m_arr* to *vector\<int\>*.**
- **Change name of argument *num* in constructor to *numInt*.**

## Child
### Problems
- Attribute *m_arr*:
  - Use raw pointer of Old C++ for initializing *char* array in constructor.
  - Not release when object of Base class is destroyed, it can cause memory leak.
  - Have same name with *m_arr* of parent class, although *m_arr* of parent class can be accessed by using Base::m_arr, sometimes it will cause confusion.
  - Initializing by hard-code with size = 10 in constructor.
- Method foo() isn't marked as *override* to let reader and compiler know it is an override (optional).
### Solutions
- Attribute *m_arr*:
  - Type, name and release memory:
    - If using Old C++, need add destructor and code for release *m_arr*
    ```C++
    public:
      ~Child()
      {
          delete []m_arr;
      }
    ```
    - If using Modern C++:
      - If want to keep *m_arr* is pointer, change type of *m_arr* to *shared_ptr\<char[]\>* and no need to add code for release memory. But this solution is only for some certain purposes because there are some inconveniences when using pointer as char array. Constructor will be changed as below.
        ```C++
        explicit Base(unsigned num) : m_arr(new char[numChar])
        {
        }
        ```
      - The common way is to change type of *m_arr* to *string*, this type is replaced for array of char with many built-in methods for string. Constructor will be changed as below.
        ```C++
        explicit Child(unsigned num) : Base(num), m_arr(numChar, ' ')
        {
        }
        ```
    - For not confusing with *m_arr* between *Base* and *Child* class, change name of them following their type.
      - If using pointer (Old C+ or smart pointer) for *m_arr*, change name of *m_arr* of Base class to *m_arrInt* and *Child* to *m_arrChar*.
      - If using *vector* and *string* for *m_arr* of Base class and *Child* class, change name to *m_vec* and *m_str*.
  - Hard-code initialization of *m_arr*: add one more argument *numChar* in constructor with default value is 10 for *char* size, name of argument that is passed to constructor *Base* class is *numInt*. This will solve unclear meaning name of argument *num*.
  - Method *foo*: add keyword *override*.

**→ Final solution:**
- **Change change type of *m_arr* to *string* and name to *m_str***
- **Change name of *m_arr* of Base class to *m_vec***
- **In constructor, add 1 more argument *numInt* to pass for constructor of *Base* class.** 
- **Change name of argument *num* to *numChar* with default value is 10.**
- **Add keyword *override* for method *foo*.**

## main
### Problems
- Type of b_ptr is raw pointer of Old C++.
- After ```C++ Base *b_ptr = new Base(5);```, *b_ptr* is assigned to new memory but isn't released old memory, it causes memory leak.
### Solutions
- If using Old C++, need release memory before assigned *b_ptr* to new memory.
    ```C++
    Base *b_ptr = new Base(5);
    delete b_ptr;
    ```
- If using Modern C++, change type of *b_ptr* to *shared_ptr\<Base\>*.

**→ Final solution:**
- **Change change type of *b_ptr* and code for creating it to *shared_ptr\<Base\>***

# Conclusion
Final solution is as below.
I also added destructor for *Base* class with *default* keyword to ensure full class components and future maintenance.
```C++
class Base
{
public:
    explicit Base(unsigned numInt): m_vec(numInt)
    {
    }

    virtual void foo() {}

    virtual ~Base() = default;

protected:
    vector<int> m_vec;
};

class Child : public Base
{
public:
    explicit Child(unsigned numInt, unsigned numChar = 10) : Base(numInt), m_str(numChar, ' ')
    {
    }

    virtual void foo() override {}

private:
    string m_str;
};

int main()
{
    Base b(10);
    shared_ptr<Base> b_ptr = make_shared<Base>(5);
    b_ptr = make_shared<Child>(7);
    b_ptr->foo();
    return 0;
}
```
