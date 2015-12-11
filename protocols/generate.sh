#!/bin/bash

name=`echo $1 | cut -d \. -f 1`

echo `bin/protoc -I./generator/proto -I. --nanopb_out=. "$1"`

# include replacements
cat $name.pb.h | sed -e "8d" > $name.pb.h.t1
cat $name.pb.h.t1 | sed -e "s/<pb.h>/\"pb.h\"/" > $name.pb.h.t2

# box
cp $name.pb.h.t2 ./../devices/AmpBox/$name.pb.h
cp $name.pb.c ./../devices/AmpBox/$name.pb.hpp

# delete the temp files
rm $name.pb.h.t1
rm $name.pb.h.t2
