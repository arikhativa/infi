#!/bin/bash

gcc -c -fPIC singleton_lib.cpp -I ../include/
gcc -shared singleton_lib.o -o libFoo.so

echo "308351766" | sudo -S rm -rf /usr/lib/libFoo.so 
echo "308351766" | sudo -S cp libFoo.so /usr/lib/
echo "308351766" | sudo -S chmod 777 /usr/lib/libFoo.so
