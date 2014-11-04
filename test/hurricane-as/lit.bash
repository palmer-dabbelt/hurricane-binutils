#include "tempdir.bash"

FORMAT="--hex"

cat >>input.hex <<"EOF"
VERSION 5
INS = 0
OUTS = 0
REGS = 0
MEMS = 0
INITS = 0
DIMS (1, 1)
TILE @ (1, 0) NUM_LO_INSTS = 1
0x1000f800
EOF

#include "harness.bash"
