tempdir=`mktemp -d -t ptest-hurricane-binutils.XXXXXXXXXX`
trap "rm -rf $tempdir" EXIT
cd $tempdir
