#!/bin/bash

echo "308351766" | sudo -S nbd-client -d /dev/nbd$1
