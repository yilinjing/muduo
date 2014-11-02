#include <boost/swap.hpp>
#include <string>
#include <iostream>
using namespace std;

int main()
{
    string info("jingyilin");
    //char buf[1024] = {0};
    string buf;
    cout << info << endl;
    boost::swap(buf, info);
    cout << buf << endl;
    cout << info << endl;
}
