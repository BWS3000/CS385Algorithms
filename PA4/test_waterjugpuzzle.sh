#!/bin/bash

targetfile=waterjugpuzzle.cpp
cppfile=waterjugpuzzle.cpp
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
        # 128 + SIGBUS = 128 + 7 = 135 (rare on x86)
        135)
            echo "failed (bus error)"
            ;;
        # 128 + SIGSEGV = 128 + 11 = 139
        139)
            echo "failed (segmentation fault)"
            ;;
        # 128 + SIGTERM (sent by timeout(1)) = 128 + 15 = 143
        143)
            echo "failed (time out)"
            ;;
        *)
            computed_output=$(cat "$outputfile")
            if [ "$computed_output" = "$expected_output" ]; then
                ((num_right++))
                echo $start $end | awk '{printf "ok (%.3fs)\tvalgrind...", $2 - $1}'
                # Why 93?  Why not 93!
                valgrind --leak-check=full --error-exitcode=93 $command $args < "$inputfile" &> /dev/null
                vgstatus=$?
                if [ $vgstatus -eq 93 ]; then
                    ((memory_problems++))
                    echo "failed"
                else
                    if [ $vgstatus -eq 127 ]; then
                        echo "not found"
                    else
                        if [ $vgstatus -eq $status ]; then
                            echo "ok"
                        else
                            echo "unknown status $vgstatus"
                        fi
                    fi
                fi
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
run_test_with_args "" "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>"
run_test_with_args "1 2 3 4 5 6 7" "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>"
run_test_with_args "x 2 3 4 5 6" "Error: Invalid capacity 'x' for jug A."
run_test_with_args "1 -2 3 4 5 6" "Error: Invalid capacity '-2' for jug B."
run_test_with_args "1 2 0 4 5 6" "Error: Invalid capacity '0' for jug C."
run_test_with_args "1 2 3 y 5 6" "Error: Invalid goal 'y' for jug A."
run_test_with_args "1 2 3 4 -5 6" "Error: Invalid goal '-5' for jug B."
run_test_with_args "1 2 3 4 5 -1" "Error: Invalid goal '-1' for jug C."
run_test_with_args "3 5 8 4 0 4" "Error: Goal cannot exceed capacity of jug A."
run_test_with_args "3 5 8 0 0 9" "Error: Goal cannot exceed capacity of jug C."
run_test_with_args "3 5 8 2 1 4" "Error: Total gallons in goal state must be equal to the capacity of jug C."
run_test_with_args "6 7 10 5 5 0" "No solution."
run_test_with_args "30 45 50 25 25 0" "No solution."
run_test_with_args "5 7 10 3 3 4" "No solution."
run_test_with_args "6 7 10 0 0 10" "Initial state. (0, 0, 10)"
run_test_with_args "3 5 8 0 5 3" "Initial state. (0, 0, 8)"$'\n'"Pour 5 gallons from C to B. (0, 5, 3)"
run_test_with_args "3 5 8 0 3 5" "Initial state. (0, 0, 8)"$'\n'"Pour 3 gallons from C to A. (3, 0, 5)"$'\n'"Pour 3 gallons from A to B. (0, 3, 5)"
run_test_with_args "1 3 4 0 2 2" "Initial state. (0, 0, 4)"$'\n'"Pour 3 gallons from C to B. (0, 3, 1)"$'\n'"Pour 1 gallon from B to A. (1, 2, 1)"$'\n'"Pour 1 gallon from A to C. (0, 2, 2)"
run_test_with_args "3 5 8 0 2 6" "Initial state. (0, 0, 8)"$'\n'"Pour 5 gallons from C to B. (0, 5, 3)"$'\n'"Pour 3 gallons from B to A. (3, 2, 3)"$'\n'"Pour 3 gallons from A to C. (0, 2, 6)"
run_test_with_args "3 5 8 0 4 4" "Initial state. (0, 0, 8)"$'\n'"Pour 5 gallons from C to B. (0, 5, 3)"$'\n'"Pour 3 gallons from B to A. (3, 2, 3)"$'\n'"Pour 3 gallons from A to C. (0, 2, 6)"$'\n'"Pour 2 gallons from B to A. (2, 0, 6)"$'\n'"Pour 5 gallons from C to B. (2, 5, 1)"$'\n'"Pour 1 gallon from B to A. (3, 4, 1)"$'\n'"Pour 3 gallons from A to C. (0, 4, 4)"
run_test_with_args "8 17 20 0 10 10" "Initial state. (0, 0, 20)"$'\n'"Pour 17 gallons from C to B. (0, 17, 3)"$'\n'"Pour 8 gallons from B to A. (8, 9, 3)"$'\n'"Pour 8 gallons from A to C. (0, 9, 11)"$'\n'"Pour 8 gallons from B to A. (8, 1, 11)"$'\n'"Pour 8 gallons from A to C. (0, 1, 19)"$'\n'"Pour 1 gallon from B to A. (1, 0, 19)"$'\n'"Pour 17 gallons from C to B. (1, 17, 2)"$'\n'"Pour 7 gallons from B to A. (8, 10, 2)"$'\n'"Pour 8 gallons from A to C. (0, 10, 10)"
run_test_with_args "4 17 22 2 5 15" "No solution."
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
