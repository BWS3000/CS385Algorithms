#!/bin/bash

targetfile=quickselect.cpp
cppfile=quickselect.cpp
maxtime=2

if [ ! -f "$targetfile" ]; then
    echo "Error: file $targetfile not found"
    echo "Final score: score - penalties = 0 - 0 = 0"
    exit 1
fi

# Required by the Honor System
missing_name=0
head -n 20 "$targetfile" | egrep -i "author.*[a-zA-Z]+"
if [ $? -ne 0 ]; then
    echo "Student name missing"
    missing_name=1
fi

# Required by the Honor System
missing_pledge=0
head -n 20 "$targetfile" | egrep -i "I.*pledge.*my.*honor.*that.*I.*have.*abided.*by.*the.*Stevens.*Honor.*System"
if [ $? -ne 0 ]; then
    echo -e "Pledge missing"
    missing_pledge=1
fi

# Compiling
echo
results=$(make 2>&1)
if [ $? -ne 0 ]; then
    echo "$results"
    echo "Final score: score - penalties = 0 - 0 = 0"
    exit 1
fi

num_tests=0
num_right=0
memory_problems=0
command="./${cppfile%.*}"

run_test_with_args_and_input() {
    ((num_tests++))
    echo -n "Test $num_tests..."

    args="$1"
    input="$2"
    expected_output="$3"

    outputfile=$(mktemp)
    inputfile=$(mktemp)
    statusfile=$(mktemp)

    echo -e "$input" > "$inputfile"

    start=$(date +%s.%N)
    # Run run run, little program!
    (timeout --preserve-status "$maxtime" "$command" $args < "$inputfile" &> "$outputfile"; echo $? > "$statusfile") &> /dev/null
    end=$(date +%s.%N)
    status=$(cat "$statusfile")

    case $status in
        # $command: 128 + SIGBUS = 128 + 7 = 135 (rare on x86)
        135)
            echo "failed (bus error)"
            ;;
        # $command: 128 + SIGSEGV = 128 + 11 = 139
        139)
            echo "failed (segmentation fault)"
            ;;
        # $command: 128 + SIGTERM (sent by timeout(1)) = 128 + 15 = 143
        143)
            echo "failed (time out)"
            ;;
        *)
            computed_output=$(cat "$outputfile")
            if [ "$computed_output" = "$expected_output" ]; then
                ((num_right++))
                echo $start $end | awk '{printf "ok (%.3fs)\tvalgrind...", $2 - $1}'
                # Why 93?  Why not 93!
                (valgrind --leak-check=full --error-exitcode=93 $command $args < "$inputfile" &> /dev/null; echo $? > "$statusfile") &> /dev/null
                vgstatus=$(cat "$statusfile")
                case $vgstatus in
                    # valgrind detected an error when running $command
                    93)
                        ((memory_problems++))
                        echo "failed"
                        ;;
                    # valgrind not installed or not in $PATH
                    127)
                        echo "not found"
                        ;;
                    # valgrind: 128 + SIGBUS = 128 + 7 = 135 (rare on x86)
                    135)
                        ((memory_problems++))
                        echo "failed (bus error)"
                        ;;
                    # valgrind: 128 + SIGSEGV = 128 + 11 = 139
                    139)
                        ((memory_problems++))
                        echo "failed (segmentation fault)"
                        ;;
                    # compare with expected status from running $command without valgrind
                    $status)
                        echo "ok"
                        ;;
                    *)
                        ((memory_problems++))
                        echo "unknown status $vgstatus"
                        ;;
                esac
            else
                echo "failed"
                echo "==================== Expected ===================="
                echo "$expected_output"
                echo "==================== Received ===================="
                echo "$computed_output"
                echo "=================================================="
            fi
            ;;
    esac
    rm -f "$inputfile" "$outputfile" "$statusfile"
}

run_test_with_args() {
    run_test_with_args_and_input "$1" "" "$2"
}
run_test_with_input() {
    run_test_with_args_and_input "" "$1" "$2"
}

############################################################
run_test_with_args_and_input "lots of args" "" "Usage: ./quickselect <k>"
run_test_with_args_and_input "w" "" "Error: Invalid value 'w' for k."
run_test_with_args_and_input "0" "" "Error: Invalid value '0' for k."
run_test_with_args_and_input "-8" "" "Error: Invalid value '-8' for k."
run_test_with_args_and_input "3" "1 2 x 3" "Enter sequence of integers, each followed by a space: Error: Non-integer value 'x' received at index 2."
run_test_with_args_and_input "4" "" "Enter sequence of integers, each followed by a space: Error: Sequence of integers not received."
run_test_with_args_and_input "4" "  " "Enter sequence of integers, each followed by a space: Error: Sequence of integers not received."
run_test_with_args_and_input "4" "1 2" "Enter sequence of integers, each followed by a space: Error: Cannot find smallest element 4 with only 2 values."
run_test_with_args_and_input "5" "1" "Enter sequence of integers, each followed by a space: Error: Cannot find smallest element 5 with only 1 value."
#
run_test_with_args_and_input "1" "1" "Enter sequence of integers, each followed by a space: Smallest element 1: 1"
run_test_with_args_and_input "1" "1000" "Enter sequence of integers, each followed by a space: Smallest element 1: 1000"
run_test_with_args_and_input "2" "1 2" "Enter sequence of integers, each followed by a space: Smallest element 2: 2"
run_test_with_args_and_input "2" "23 16" "Enter sequence of integers, each followed by a space: Smallest element 2: 23"
run_test_with_args_and_input "5" "1 3 5 2 4" "Enter sequence of integers, each followed by a space: Smallest element 5: 5"
run_test_with_args_and_input "3" "10 12 16 14 11" "Enter sequence of integers, each followed by a space: Smallest element 3: 12"
run_test_with_args_and_input "1" "3 6 2 4 768 1 4 567 123 65 12 345 12 -100" "Enter sequence of integers, each followed by a space: Smallest element 1: -100"
run_test_with_args_and_input "4" "3 6 2 4 768 1 4 567 123 65 12 345 12 -100" "Enter sequence of integers, each followed by a space: Smallest element 4: 3"
run_test_with_args_and_input "9" "3 6 2 4 768 1 4 567 123 65 12 345 12 -100" "Enter sequence of integers, each followed by a space: Smallest element 9: 12"
run_test_with_args_and_input "10" "5 5 5 5 5 5 5 5 5 5 5 5" "Enter sequence of integers, each followed by a space: Smallest element 10: 5"
run_test_with_args_and_input "12" "-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1" "Enter sequence of integers, each followed by a space: Smallest element 12: -1"
run_test_with_args_and_input "10000" "$(echo {1..100000})" "Enter sequence of integers, each followed by a space: Smallest element 10000: 10000"
run_test_with_args_and_input "8765" "$(echo {100000..1})" "Enter sequence of integers, each followed by a space: Smallest element 8765: 8765"
############################################################
echo
echo "Total tests run: $num_tests"
echo "Number correct : $num_right"
score=$((100 * $num_right / $num_tests))
echo "Percent correct: $score%"
if [ $missing_name == 1 ]; then
    echo "Missing Name: -5"
fi
if [ $missing_pledge == 1 ]; then
    echo "Missing or incorrect pledge: -5"
fi

if [ $memory_problems -gt 1 ]; then
    echo "Memory problems: $memory_problems (-5 each, max of -15)"
    if [ $memory_problems -gt 3 ]; then
        memory_problems=3
    fi
fi

penalties=$((5 * $missing_name + 5 * $missing_pledge + 5 * $memory_problems))
final_score=$(($score - $penalties))
if [ $final_score -lt 0 ]; then
    final_score=0
fi
echo "Final score: score - penalties = $score - $penalties = $final_score"

make clean > /dev/null 2>&1
