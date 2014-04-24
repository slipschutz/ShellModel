#!/bin/bash



CXX=`root-config --cxx`
CFLAGS=`root-config --cflags`
LDLIBS=`root-config --glibs`
g++ main.C -g ${LDLIBS} ${CFLAGS} -std=c++11 -o main
