
using namespace std;


#include "Examples/BasicObject.h"
#include "Type.h"



int main() {

    Type::RegisterType("test", 16);

    //Type::GetType<BasicObject>();


    return 0;
}