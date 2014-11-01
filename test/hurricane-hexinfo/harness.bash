set -ex

cat input.hex
cat output.gold

valgrind="$(which valgrind 2> /dev/null || true)"
if [[ "$valgrind" != "" ]]
then
    valgrind --quiet --log-file=valgrind.log \
        $PTEST_BINARY $FORMAT input.hex > output.test
    cat valgrind.log
    if [[ "$(cat valgrind.log | wc -l)" != 0 ]]
    then
        exit 1
    fi

    diff -u output.gold output.test
fi

$PTEST_BINARY $FORMAT input.hex > output.test
diff -u output.gold output.test
