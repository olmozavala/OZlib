#!/bin/sh
rm -f *.so
echo "Compiling FileManager..."
cd FileManager
make 
cd ..
echo "Compiling Timers..."
cd Timers
make 
cd ..
echo "Compiling CLManager..."
cd CLManager
make 
cd ..
echo "Compiling GLManager..."
cd GLManager
make 
cd ..
echo "Compiling ImageManager..."
cd ImageManager
make 
cd ..
echo "Compiling MatrixUtils..."
cd MatrixUtils
make 
cd ..

