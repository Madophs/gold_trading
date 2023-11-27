#include <ios>
#include <iostream>
#include <vector>
#include <iomanip>

#include <bits/stl_vector.h>

using namespace std;

int main()
{
    vector<int*> vec(2);
    cout << "vec[0] "<< &vec[0] << endl;
    cout << "vec[1] "<< &vec[1] << endl;
    vec[0] = new int(2);
    vec[1] = new int(1);
    auto p = &vec[0];
    cout << "vec[0] "<< &vec[0] << endl;
    cout << "vec[1] "<< &vec[1] << endl;
    cout << "**p "<< p << endl;
    for (int i = 2; i < 20; ++i) {
        vec.push_back(new int(i));
    }
    vec.pop_back();
    cout << "vec[0] "<< &vec[0] << endl;
    cout << "vec[1] "<< &vec[1] << endl;
    cout << "**p "<< p << endl;
    return 0;
}
