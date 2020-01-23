#!/bin/bash

echo "308351766" | sudo -S valgrind --leak-check=yes --track-origins=yes ./ver.0.1.out $1 /dev/nbd$2
