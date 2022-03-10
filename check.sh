#!/bin/bash

# ----------------- General declarations and util functions ------------------ #

INPUT_DIR=_test/input/
REF_DIR=_test/ref/
OUT_DIR=_test/output/
EXEC_NAME=./tema2
VALGRIND="valgrind --leak-check=full --error-exitcode=1"
max_points=135
max_bonus=20
result_points=0
bonus_points=0

print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

test_do_fail()
{
	printf "failed  [ 0/%02d]\n" "$max_points"
	if test "x$EXIT_IF_FAIL" = "x1"; then
		exit 1
	fi
}

test_do_pass()
{
	printf "passed  [%02d/%02d]\n" "${1}" "$max_points"
	((result_points+=${1}))
}

bonus_function()
{
	printf "passed\n"
	((bonus_points+=${1}))
}

test_function()
{
	input_file="$INPUT_DIR${1}"
	output_file="$OUT_DIR${2}"
	ref_file="$REF_DIR${3}"
	points_per_test="${4}"
	points_per_test_vg="${5}"

	$EXEC_NAME $input_file $output_file 

	diff $output_file $ref_file > /dev/null
	if test $? -eq 0; then
		printf "[%s]..........................................." ${2}
		test_do_pass $points_per_test
		#valgrind test

		$VALGRIND $EXEC_NAME $input_file $output_file &> /dev/null
		if [ $? -eq 0 ]; then
			printf "[VALGRIND: PASSED]...................................." ${2}
			bonus_function $points_per_test_vg
		else
			printf "[VALGRIND: FAILED]...................................." ${2}
		fi

	else
		printf "[%s]..........................................." ${2}
		test_do_fail
	fi
}

init_function()
{
    make build
	mkdir $OUT_DIR &> /dev/null
}

clean_out_function()
{
    rm _test/output/*.*
    rmdir _test/output
}

init_function

print_header "Tema 2 SD Netflix"

#Testing

declare ex_values=(0 2 2 2 2)
declare tests_values=(0 4 4 4 4 4 4 4 4 4 5 4 4 4 4 4 4 4 4 5 5 4 4 4 4 4 4 5 5 5 5)
declare valgrind_tests_values=1

for i in {1..4}
do
	test_function "ex$i.in" "ex$i.out" "ex$i.ref" "${ex_values[$i]}" "${valgrind_tests_values}" 
	echo ""
done

for i in {1..30}
do
	test_function "test$i.in" "test$i.out" "test$i.ref" "${tests_values[$i]}" "${valgrind_tests_values}" 
	echo ""
done

#end Testing

printf "\n%49s [%02d/135]\n" "Total:" $result_points;
if [ $bonus_points -ne 34 ]; then
	((bonus_points=0))
	echo "Failed VALGRIND tests"
else
	((bonus_points=20))
fi
printf "\n%49s [%02d/20]\n" "Bonus:" $bonus_points;

clean_out_function

echo 

