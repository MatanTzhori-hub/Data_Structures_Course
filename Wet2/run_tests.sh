REDBG='\e[41m'
GREENBG='\e[42m'
NC='\033[0m' # No Color

printf "${YELLOW}Running tests from Whatsapp:${NC}\n"

for i in {0..99}; do
    ./a.out < ./tests/input/in$i.txt > ./tests/output/out$i.txt
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./a.out < ./tests/input/in$i.txt 2> ./valgrind_out/val$i.txt
    printf "${YELLOW}Test number $i ran: ${NC}"
    diff -q ./tests/output/out$i.txt ./tests/expected/out$i.txt
    if [[ $? == "0" ]]
    then
        printf "${GREENBG}PASSED!${NC}\n"
    else
        printf "${REDBG}FAILED${NC}\n"
    fi
done
