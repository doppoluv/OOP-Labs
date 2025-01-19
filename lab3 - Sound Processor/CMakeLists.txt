cmake_minimum_required(VERSION 3.10.2) 
project(Lab4)

add_executable(${PROJECT_NAME} main.cpp src/CFGParser.cpp src/CMDParser.cpp src/Converters.cpp src/Factory.h src/Sound.h src/WAV.cpp)