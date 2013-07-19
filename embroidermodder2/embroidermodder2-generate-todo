#!/bin/sh

OUTPUTFILE="TODO"
#include the line number in output
#do not search temporary, object, and moc files
GREPCMD="grep --line-number --recursive --exclude=*.*~ --exclude=*.o --exclude=moc*.cpp --exclude=*Makefile* --exclude=*TODO* --exclude=*generate-todo* --exclude=*_memleak*"
DIRECTORY="./"
TODOSTRING="TODO"
BUGSTRING="BUG"
HACKSTRING="HACK"
WARNSTRING="WARNING"
NOTESTRING="NOTE"

rm -f ${OUTPUTFILE}
echo "==================================================" >> ${OUTPUTFILE}
echo "This list was generated on:" >> ${OUTPUTFILE}
date >> ${OUTPUTFILE}
echo "==================================================" >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

echo "==================================================" >> ${OUTPUTFILE}
echo "TODO" >> ${OUTPUTFILE}
echo "==================================================" >> ${OUTPUTFILE}
${GREPCMD} ${TODOSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

echo "==================================================" >> ${OUTPUTFILE}
echo "BUG" >> ${OUTPUTFILE}
echo "==================================================" >> ${OUTPUTFILE}
${GREPCMD} ${BUGSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

echo "==================================================" >> ${OUTPUTFILE}
echo "HACK" >> ${OUTPUTFILE}
echo "==================================================" >> ${OUTPUTFILE}
${GREPCMD} ${HACKSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

echo "==================================================" >> ${OUTPUTFILE}
echo "WARNING" >> ${OUTPUTFILE}
echo "==================================================" >> ${OUTPUTFILE}
${GREPCMD} ${WARNSTRING} ${DIRECTORY} >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}

echo "==================================================" >> ${OUTPUTFILE}
echo "NOTE" >> ${OUTPUTFILE}
echo "==================================================" >> ${OUTPUTFILE}
${GREPCMD} ${NOTESTRING} ${DIRECTORY} >> ${OUTPUTFILE}
echo "" >> ${OUTPUTFILE}