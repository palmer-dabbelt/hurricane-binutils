set -ex

objdump="$(dirname "$PTEST_BINARY")/hurricane-objdump"

if test -e input.hex
then
    cat input.hex
    $objdump input.hex --as > input.S || \
        ( gdb --batch --ex r --ex bt --args $objdump input.hex --as; exit 1 )
    cp input.hex output.gold
fi

cat input.S
cat output.gold

$PTEST_BINARY $FORMAT input.S > output.test ||
  ( cat output.gold | head -n $(cat output.test | wc -l) > output.gold.part;
      diff -u output.gold.part output.test;
      exit 1 )
diff -u output.gold output.test

valgrind="$(which valgrind 2> /dev/null || true)"
if [[ "$valgrind" != "" ]]
then
    valgrind --quiet --log-file=valgrind.log \
        $PTEST_BINARY $FORMAT input.S > output.test
    cat valgrind.log
    if [[ "$(cat valgrind.log | wc -l)" != 0 ]]
    then
        exit 1
    fi

    diff -u output.gold output.test
fi
