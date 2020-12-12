
file(GLOB SOURCE Tests/*.cpp)
file(GLOB includes ./*/*.cpp)

create_executable(${SOURCE})
