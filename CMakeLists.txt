cmake_minimum_required(VERSION 3.5)

project(Corners-cpp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR})

file(GLOB HEADERS "${INCLUDE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp")
file(GLOB SOURCES "${SRC_DIR}/*.c" "${SRC_DIR}/*.cpp")

set(PROJECT_SOURCES ${SOURCES} ${HEADERS})

find_package(
    SFML 2.5 
    COMPONENTS 	
        system window graphics audio REQUIRED
)

add_executable(Corners-cpp ${PROJECT_SOURCES})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

target_link_libraries(Corners-cpp sfml-system sfml-graphics sfml-audio sfml-window)

install(TARGETS Corners-cpp RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_SOURCE_DIR}/bin)
