#!/bin/bash

./bin/protoc --nanopb_out=. ./$1

name=`echo $1 | cut -d \. -f 1`

mv $name.pb.c $name.pb.hpp
