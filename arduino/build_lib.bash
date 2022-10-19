#!/bin/bash
LIB_NAME="ArchRobo2022"
mkdir ${LIB_NAME}
cp library.properties ${LIB_NAME}
cp -R examples ${LIB_NAME}
cp -R src ${LIB_NAME}
zip -r ${LIB_NAME} ${LIB_NAME} 
mv ${LIB_NAME}.zip lib
rm -R ${LIB_NAME}
