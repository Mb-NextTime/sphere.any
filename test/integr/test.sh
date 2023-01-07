#!/bin/sh
NULL=/dev/null
PASSED=0
TEST_N=0
Yellow='\033[1;33m'
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo "\n${Yellow}Integration testing:${NC}"
echo "${GREEN}[==========]${NC}"

TEST_N=$((${TEST_N} + 1))
echo "${GREEN}[ RUN      ]${NC} test ${TEST_N}"
if echo "1 i 10 i" | ./tmp/demo 2>${NULL} 1>${NULL} == "10" ; then
    echo "${GREEN}[       OK ]${NC} test ${TEST_N}"
    PASSED=$((${PASSED} + 1))
else
    echo "${RED}[  FAILED  ]${NC} test ${TEST_N}"
fi

TEST_N=$((${TEST_N} + 1))
echo "${GREEN}[ RUN      ]${NC} test ${TEST_N}"
if echo "1 f 2.1 f" | ./tmp/demo 2>${NULL} 1>${NULL} == "2.1" ; then
    echo "${GREEN}[       OK ]${NC} test ${TEST_N}"
    PASSED=$((${PASSED} + 1))
else
    echo "${RED}[  FAILED  ]${NC} test ${TEST_N}"
fi

TEST_N=$((${TEST_N} + 1))
echo "${GREEN}[ RUN      ]${NC} test ${TEST_N}"
if echo "3 i 1 f 2.0 d 3.0 d" | ./tmp/demo 2>${NULL} 1>${NULL} == "3.0" ; then
    echo "${GREEN}[       OK ]${NC} test ${TEST_N}"
    PASSED=$((${PASSED} + 1))
else
    echo "${RED}[  FAILED  ]${NC} test ${TEST_N}"
fi


TEST_N=$((${TEST_N} + 1))
echo "${GREEN}[ RUN      ]${NC} test ${TEST_N}"
if echo "2 t 10 meow t 11 @astolfo t" | ./tmp/demo 2>${NULL} 1>${NULL} == "11 @astolfo"  ; then
    echo "${GREEN}[       OK ]${NC} test ${TEST_N}"
    PASSED=$((${PASSED} + 1))
else
    echo "${RED}[  FAILED  ]${NC} test ${TEST_N}"
fi


echo "${GREEN}[==========]${NC}"
if [ ${PASSED} -ne 0 ] ; then
echo "${GREEN}[  PASSED  ]${NC} ${PASSED} tests."
fi
if [ $((${TEST_N} - ${PASSED})) -ne 0 ] ; then
echo "${RED}[  FAILED  ]${NC} $((${TEST_N} - ${PASSED})) tests."
fi