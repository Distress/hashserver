#!/bin/bash

port=50000
number_of_threads=4
timeout="5s"

for i in $(seq 1 $number_of_threads)
do
    timeout $timeout bash -c "cat /dev/urandom | nc localhost $port > /dev/null" &
done