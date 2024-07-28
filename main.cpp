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