#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

class Person
{
public:
    Person():m_name(""), m_age(0)
    {
        cout << "Default constructor" << endl;
    }

    Person(string name, int age):
        m_name(name), m_age(age)
    {
        cout << "Calling constructor for " << m_name << ", age " << m_age << endl;
    }

    //Person(const Person& person):Person(person.m_name, person.m_age)
    //{
        //cout << "Calling copy  constructor for " << m_name << ", age " << m_age << endl;
    //}

    Person(Person&& person):Person(std::move(person.m_name), std::move(person.m_age))
    {
        cout << "Calling move constructor for " << m_name << endl;
    }

    void setName(const string& name)
    {
        m_name = name;
    }

    string getName() const
    {
        return m_name;
    }

    void setAge(const int& age)
    {
        m_age = age;
    }

    int getAge() const
    {
        return m_age;
    }

    virtual void sayHello()
    {
        cout << "Hi I'm the base class" << endl;
    }

private:
    string m_name;
    int m_age;
};

class Student : public Person
{
public:
    Student():Person()
    {

    }

    Student(string name, int age):Person(name, age)
    {
    }

    virtual void sayHello() override final
    {
        cout << "Hi I'm the student class" << endl;
    }
};

int main()
{
    struct Pair {Student student; bool isFree{true};};
    vector<Pair> vec(10);
    auto ref = &vec[0];
    cout << ref->isFree << endl;
    Student* s = &(ref->student);
    s = new(s)Student("jair", 22);
    cout << (&ref->student) << endl;
    cout << (&vec[0]) << endl;
    cout << (s) << endl;
    auto r = reinterpret_cast<const Pair*>(s);
    auto r2 = &vec[5];
    cout << "Ref: " << *&r << endl;
    cout << "&vec[0] " << &vec[0] << endl;
    cout << r - r2 << endl;
    cout << &vec[0] << endl;
    cout << &vec[5] << endl;
    cout << &vec[5] - &vec[0] << endl;
    cout << vec[0].student.getName() << endl;
    cout << vec[1].student.getName() << endl;
    return EXIT_SUCCESS;
}
