cmake_minimum_required(VERSION 2.8)

project(SubscribeDCEL2Data)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)
#add_definitions(-std=c++11 -m64 -Wall -Wextra -Wshadow -Wnon-virtual-dtor)
add_definitions(-std=c++11 -m64 -Wall)

SET(CMAKE_BUILD_TYPE "Release")

#boost include/lib path 
set(BOOSTPAHT /home/chaozhen/bin_cz)
set(BOOSTPAHT_2 /usr/local)
include_directories(${BOOSTPAHT_2}/include)
link_directories(${BOOSTPAHT_2}/lib)

include_directories(../Include)

add_executable(SubscribeDCEL2Data SubscribeDCEL2Data.cpp
	       CommonFunction.h
	       QuoteCircularBuffer.h
	       DCEL2DataRespond.h
	       DCEL2DataRespond.cpp
	       LoadYamlFileConfig.h
	       LoadYamlFileConfig.cpp
	       TickDataContainer.h
	       TickDataContainer.cpp
               WriteTickDataFile.h
	       WriteTickDataFile.cpp
	       )

#dce l2
target_link_libraries(SubscribeDCEL2Data ${SubscribeDCEL2Data_SOURCE_DIR}/Lib/liblevel2Api.so)

#yaml
target_link_libraries(SubscribeDCEL2Data ${SubscribeDCEL2Data_SOURCE_DIR}/Lib/libyaml-cpp.a)

#boost
target_link_libraries(SubscribeDCEL2Data boost_thread)
target_link_libraries(SubscribeDCEL2Data boost_filesystem)
target_link_libraries(SubscribeDCEL2Data boost_system)

target_link_libraries(SubscribeDCEL2Data -lpthread -lm)

