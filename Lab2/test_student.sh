#!/bin/bash

targetfile=student.cpp
cppfile=student.cpp
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
run_test_with_input "John\nDoe\n3.0\n12345\nn" "Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? "$'\n'"All students:"$'\n'"John Doe, GPA: 3.00, ID: 12345"$'\n'$'\n'"Failing students: None"
run_test_with_input "John\nGrimes\n0.88\n23456\nn" "Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? "$'\n'"All students:"$'\n'"John Grimes, GPA: 0.88, ID: 23456"$'\n'$'\n'"Failing students:"$'\n'"John Grimes, GPA: 0.88, ID: 23456"
run_test_with_input "John\nDoe\n3.0\n12345\ny\nJohn\nGrimes\n0.88\n23456\nn" "Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? "$'\n'"All students:"$'\n'"John Doe, GPA: 3.00, ID: 12345"$'\n'"John Grimes, GPA: 0.88, ID: 23456"$'\n'$'\n'"Failing students:"$'\n'"John Grimes, GPA: 0.88, ID: 23456"
run_test_with_input "John\nGrimes\n0.88\n23456\nyJohn\nDoe\n1.0\n12345\nn" "Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? "$'\n'"All students:"$'\n'"John Grimes, GPA: 0.88, ID: 23456"$'\n'"John Doe, GPA: 1.00, ID: 12345"$'\n'$'\n'"Failing students:"$'\n'"John Grimes, GPA: 0.88, ID: 23456"
run_test_with_input "John\nGrimes\n0.88\n23456\nyJohn\nDoe\n0.9\n12345\nn" "Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? Enter student's first name: Enter student's last name: Enter student's GPA (0.0-4.0): Enter student's ID: Add another student to database (Y/N)? "$'\n'"All students:"$'\n'"John Grimes, GPA: 0.88, ID: 23456"$'\n'"John Doe, GPA: 0.90, ID: 12345"$'\n'$'\n'"Failing students:"$'\n'"John Grimes, GPA: 0.88, ID: 23456"$'\n'"John Doe, GPA: 0.90, ID: 12345"
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
