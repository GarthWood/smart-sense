#!/bin/bash

name=`echo $1 | cut -d \. -f 1`

echo `bin/protoc -I./generator/proto -I. --nanopb_out=. "$1"`

# include replacements
cat $name.pb.h | sed -e "8d" > $name.pb.h.t1
cat $name.pb.h.t1 | sed -e "s/<pb.h>/\"pb.h\"/" > $name.pb.h.t2

# temperature box
cp $name.pb.h.t2 ./../devices/TemperatureBox/$name.pb.h
cp $name.pb.c ./../devices/TemperatureBox/$name.pb.hpp

# delete the temp files
rm $name.pb.h.t1
rm $name.pb.h.t2
