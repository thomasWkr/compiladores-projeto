#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Configure paths relative to the project root
MY_COMPILER="./etapa4"
TESTS_DIR="./tests"

# Color setup
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "Starting automated tests..."
echo "--------------------------------------------------"

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

for folder in $(find "$TESTS_DIR" -maxdepth 1 -mindepth 1 -type d | sort); do
    folder_name=$(basename "$folder")

    if ! [[ "$folder_name" =~ ^[0-9]+$ ]]; then
        echo "Skipping non-numeric folder: $folder_name"
        continue
    fi

    EXPECTED_CODE="$folder_name"
    echo "Processing folder: $folder_name (Expected Return Code: $EXPECTED_CODE)"

    for test_file in "$folder"/*.txt; do
        if [ ! -e "$test_file" ]; then
            echo "  No .txt files found in $folder. Skipping."
            continue 2
        fi

        TOTAL_TESTS=$((TOTAL_TESTS + 1))
        printf "  Running test: %-30s... " "$(basename "$test_file")"

        "$MY_COMPILER" < "$test_file" >/dev/null 2>&1
        ACTUAL_CODE=$?

        if [ "$ACTUAL_CODE" -eq "$EXPECTED_CODE" ]; then
            printf "${GREEN}PASS${NC} (Got: %d, Expected: %d)\n" "$ACTUAL_CODE" "$EXPECTED_CODE"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            printf "${RED}FAIL${NC} (Got: %d, Expected: %d)\n" "$ACTUAL_CODE" "$EXPECTED_CODE"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    done
    echo "--------------------------------------------------"
done

echo "Test Summary:"
echo "Total Tests: $TOTAL_TESTS"
echo "Passed:      $PASSED_TESTS"
echo "Failed:      $FAILED_TESTS"
echo "--------------------------------------------------"

if [ "$FAILED_TESTS" -gt 0 ]; then
    exit 1
else
    exit 0
fi

