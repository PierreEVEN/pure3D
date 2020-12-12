
#include "Examples/BasicObject.h"
#include "RType.h"


int main() {

    REFL_REGISTER_TYPE(BasicObject);

    std::cout << RType::GetType<BasicObject>()->GetName() << std::endl;

    return 0;
}