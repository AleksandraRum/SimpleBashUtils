#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
   "VAR txt.txt"
   "VAR txt.txt test.txt"
)

declare -a tests1=(
    "txt.txt -n"
    "-b -e -n -s -t -v txt.txt"
    "nofile.txt"
)

get_test()
{
    templ=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $templ >> test_s21_cat.log
    cat $templ >> test_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_cat.log)"
        (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_cat.log are identical" ]
    then
        (( SUCCESS++ ))
        echo "Всего тестов: "$COUNTER ", Успешно: "$SUCCESS ", Провалено "$FAIL
    else
        (( FAIL++ ))
        echo "Всего тестов: "$COUNTER ", Успешно: "$SUCCESS ", Провалено "$FAIL $templ
    fi
rm test_s21_cat.log test_cat.log
}

for var1 in b n s t v e
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        get_test $i
    done
done

for var1 in n s t v
do
    for var2 in b e n s t v
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                get_test $i
            done
        fi
    done
done
