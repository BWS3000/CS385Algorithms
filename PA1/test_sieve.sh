#!/bin/bash

targetfile=sieve.cpp
cppfile=sieve.cpp
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
run_test_with_input "x" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: Error: Input is not an integer."
run_test_with_input "-1" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: Error: Input must be an integer >= 2."
run_test_with_input "0" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: Error: Input must be an integer >= 2."
run_test_with_input "1" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: Error: Input must be an integer >= 2."
run_test_with_input "2" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: "$'\n'"Number of primes found: 1"$'\n'"Primes up to 2:"$'\n'"2"
run_test_with_input "10" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: "$'\n'"Number of primes found: 4"$'\n'"Primes up to 10:"$'\n'"2 3 5 7"
run_test_with_input "99" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: "$'\n'"Number of primes found: 25"$'\n'"Primes up to 99:"$'\n'"2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97"
run_test_with_input "173" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: "$'\n'"Number of primes found: 40"$'\n'"Primes up to 173:"$'\n'"  2   3   5   7  11  13  17  19  23  29  31  37  41  43  47  53  59  61  67  71"$'\n'" 73  79  83  89  97 101 103 107 109 113 127 131 137 139 149 151 157 163 167 173"
run_test_with_input "250" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: "$'\n'"Number of primes found: 53"$'\n'"Primes up to 250:"$'\n'"  2   3   5   7  11  13  17  19  23  29  31  37  41  43  47  53  59  61  67  71"$'\n'" 73  79  83  89  97 101 103 107 109 113 127 131 137 139 149 151 157 163 167 173"$'\n'"179 181 191 193 197 199 211 223 227 229 233 239 241"
run_test_with_input "1000" "**************************** Sieve of Eratosthenes ****************************"$'\n'"Search for primes up to: "$'\n'"Number of primes found: 168"$'\n'"Primes up to 1000:"$'\n'"  2   3   5   7  11  13  17  19  23  29  31  37  41  43  47  53  59  61  67  71"$'\n'" 73  79  83  89  97 101 103 107 109 113 127 131 137 139 149 151 157 163 167 173"$'\n'"179 181 191 193 197 199 211 223 227 229 233 239 241 251 257 263 269 271 277 281"$'\n'"283 293 307 311 313 317 331 337 347 349 353 359 367 373 379 383 389 397 401 409"$'\n'"419 421 431 433 439 443 449 457 461 463 467 479 487 491 499 503 509 521 523 541"$'\n'"547 557 563 569 571 577 587 593 599 601 607 613 617 619 631 641 643 647 653 659"$'\n'"661 673 677 683 691 701 709 719 727 733 739 743 751 757 761 769 773 787 797 809"$'\n'"811 821 823 827 829 839 853 857 859 863 877 881 883 887 907 911 919 929 937 941"$'\n'"947 953 967 971 977 983 991 997"
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
