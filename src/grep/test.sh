#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
   "ghj test.txt"
   "VAR ghj test.txt"
   "VAR ghj test.txt test.txt"
)

declare -a tests1=(
    "-e"
)

get_test()
{
    templ=$(echo $@ | sed "s/VAR/$var/")
    ./grep $templ >> test_s21_grep.log
    grep $templ >> test_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_grep.log)"
        (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_grep.log are identical" ]
    then
        (( SUCCESS++ ))
        echo "Всего тестов: "$COUNTER ", Успешно: "$SUCCESS ", Провалено "$FAIL
    else
        (( FAIL++ ))
        echo "Всего тестов: "$COUNTER ", Успешно: "$SUCCESS ", Провалено "$FAIL $templ
    fi
rm test_s21_grep.log test_grep.log
}

for var1 in n c l e i v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        get_test $i
    done
done


for i in "${tests1[@]}"
do
    var="-"
    get_test $i
done