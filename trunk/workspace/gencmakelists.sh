#!/bin/bash

FLIST=".metainfo\n"

for i in $1/*xml
do
    i=`basename $i`
    FLIST+="\t$i\n"
done

echo -e "INSTALL(FILES $FLIST DESTINATION share/amelia/workspace/$1)"
