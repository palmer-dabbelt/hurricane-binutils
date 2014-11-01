tempdir=`mktemp -d -t ptest-hurricane-hexinfo.XXXXXXXXXX`
trap "rm -rf $tempdir" EXIT
cd $tempdir
