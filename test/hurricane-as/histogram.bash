#include "tempdir.bash"

FORMAT="--hex"

cat >>input.hex <<"EOF"
VERSION 5
INS = 4
Histogram::io_data_in_3/32 @ (1, 3)[0]
Histogram::io_data_in_2/32 @ (1, 2)[0]
Histogram::io_data_in_1/32 @ (1, 1)[0]
Histogram::io_data_in_0/32 @ (1, 0)[0]
OUTS = 4
Histogram::io_count_out_3/32 @ (4, 3)[0]
Histogram::io_count_out_2/32 @ (4, 2)[0]
Histogram::io_count_out_1/32 @ (4, 1)[0]
Histogram::io_count_out_0/32 @ (4, 0)[0]
REGS = 0
MEMS = 16
Histogram:Row_3:Tile_3::bins @ (4, 3)[992 : 1023]
Histogram:Row_3:Tile_2::bins @ (3, 3)[992 : 1023]
Histogram:Row_3:Tile_1::bins @ (2, 3)[992 : 1023]
Histogram:Row_3:Tile_0::bins @ (1, 3)[992 : 1023]
Histogram:Row_2:Tile_3::bins @ (4, 2)[992 : 1023]
Histogram:Row_2:Tile_2::bins @ (3, 2)[992 : 1023]
Histogram:Row_2:Tile_1::bins @ (2, 2)[992 : 1023]
Histogram:Row_2:Tile_0::bins @ (1, 2)[992 : 1023]
Histogram:Row_1:Tile_3::bins @ (4, 1)[992 : 1023]
Histogram:Row_1:Tile_2::bins @ (3, 1)[992 : 1023]
Histogram:Row_1:Tile_1::bins @ (2, 1)[992 : 1023]
Histogram:Row_1:Tile_0::bins @ (1, 1)[992 : 1023]
Histogram:Row_0:Tile_3::bins @ (4, 0)[992 : 1023]
Histogram:Row_0:Tile_2::bins @ (3, 0)[992 : 1023]
Histogram:Row_0:Tile_1::bins @ (2, 0)[992 : 1023]
Histogram:Row_0:Tile_0::bins @ (1, 0)[992 : 1023]
INITS = 0
DIMS (4, 4)
TILE @ (1, 0) NUM_LO_INSTS = 28
0x1000f800 Histogram:Row_0:Tile_0::bins
0x10408000 Histogram:Row_0:Tile_0::bin__lit_hi__1
0x10800000 Histogram:Row_0:Tile_0::bin__lit_lo__1
0x88421400 Histogram:Row_0:Tile_0::bin__lit_cat__1
0xd0800000
0x90c50000 Histogram:Row_0::io_data_in
0x90c70000 Histogram:Row_0:Tile_0::io_data_in
0xb0460800 Histogram:Row_0:Tile_0::bin
0x90c70000 Histogram:Row_0:Tile_0::io_data_out
0x97c70004
0x90ff0010
0xb1021800 Histogram:Row_0:Tile_0::tile
0x51090000 T141
0xb1421800 T135
0x91bf0010
0x918d0000 Histogram:Row_0:Tile_0::io_count_in
0xb0ca1980 T136
0x70c70000 T137
0x98421800 Histogram:Row_0:Tile_0::local_bin
0x70c30140 T142
0x90c60000 T143_add
0x70430140 T138
0x90020000 T139_add
0xc0010800 T139
0x60480180 T156
0x90430000 Histogram:Row_0:Tile_0::io_count_out
0x97c30004
0x90010800 T140
0xc8001900 T143
TILE @ (2, 0) NUM_LO_INSTS = 27
0x1000f800 Histogram:Row_0:Tile_1::bins
0x907f0000
0x90430000 Histogram:Row_0:Tile_1::io_data_in
0x90830000 Histogram:Row_0:Tile_1::io_data_out
0x97c50004
0x10808000 Histogram:Row_0:Tile_1::bin__lit_hi__1
0x10c00000 Histogram:Row_0:Tile_1::bin__lit_lo__1
0x88841c00 Histogram:Row_0:Tile_1::bin__lit_cat__1
0xb0421000 Histogram:Row_0:Tile_1::bin
0x90bf0010
0xb0c21000 Histogram:Row_0:Tile_1::tile
0x50c70800 T132
0xb1021000 T126
0xb0881180 T127
0x70850000 T128
0x98421000 Histogram:Row_0:Tile_1::local_bin
0x90bf0000
0x90850000 Histogram:Row_0:Tile_1::io_count_in
0x71030140 T133
0x91080000 T134_add
0x70430140 T129
0x90020000 T130_add
0xc0010800 T130
0x60460080 T157
0x90430000 Histogram:Row_0:Tile_1::io_count_out
0x97c30004
0x90010800 T131
0xc80020c0 T134
TILE @ (3, 0) NUM_LO_INSTS = 27
0x1000f800 Histogram:Row_0:Tile_2::bins
0x10408000 Histogram:Row_0:Tile_2::bin__lit_hi__1
0x90bf0000
0x90850000 Histogram:Row_0:Tile_2::io_data_in
0x90c50000 Histogram:Row_0:Tile_2::io_data_out
0x97c70004
0x10c00000 Histogram:Row_0:Tile_2::bin__lit_lo__1
0x88421c00 Histogram:Row_0:Tile_2::bin__lit_cat__1
0xb0440800 Histogram:Row_0:Tile_2::bin
0x90bf0010
0xb0c21000 Histogram:Row_0:Tile_2::tile
0x50c71000 T123
0xb1021000 T117
0xb0881180 T118
0x70850000 T119
0x98421000 Histogram:Row_0:Tile_2::local_bin
0x70830140 T124
0x90840000 T125_add
0x70430140 T120
0x913f0000
0x91090000 Histogram:Row_0:Tile_2::io_count_in
0x90020000 T121_add
0xc0010800 T121
0x60460100 T158
0x90430000 Histogram:Row_0:Tile_2::io_count_out
0x97c30004
0x90010800 T122
0xc80010c0 T125
TILE @ (4, 0) NUM_LO_INSTS = 26
0x1000f800 Histogram:Row_0:Tile_3::bins
0x10408000 Histogram:Row_0:Tile_3::bin__lit_hi__1
0x10800000 Histogram:Row_0:Tile_3::bin__lit_lo__1
0x90ff0000
0x90c70000 Histogram:Row_0:Tile_3::io_data_in
0x88421400 Histogram:Row_0:Tile_3::bin__lit_cat__1
0x90bf0010
0xb0460800 Histogram:Row_0:Tile_3::bin
0xb0c21000 Histogram:Row_0:Tile_3::tile
0x50c71800 T114
0xb1021000 T108
0xb0881180 T109
0x913f0000
0x91090000 Histogram:Row_0:Tile_3::io_count_in
0x70850000 T110
0x98421000 Histogram:Row_0:Tile_3::local_bin
0x70830140 T115
0x90840000 T116_add
0x70430140 T111
0x90020000 T112_add
0xc0010800 T112
0x60460100 T159
0x90430000 Histogram:Row_0:Tile_3::io_count_out
0x90430000 Histogram:Row_0::io_count_out
0xd8020000 Histogram::io_count_out_0
0x90010800 T113
0xc80010c0 T116
TILE @ (1, 1) NUM_LO_INSTS = 30
0x1000f800 Histogram:Row_1:Tile_0::bins
0x10408000 Histogram:Row_1:Tile_0::bin__lit_hi__1
0x10800000 Histogram:Row_1:Tile_0::bin__lit_lo__1
0x88421400 Histogram:Row_1:Tile_0::bin__lit_cat__1
0xd0800000
0x90c50000 Histogram:Row_1::io_data_in
0x90c70000 Histogram:Row_1:Tile_0::io_data_in
0xb0460800 Histogram:Row_1:Tile_0::bin
0x90c70000 Histogram:Row_1:Tile_0::io_data_out
0x97c70004
0x90ff0010
0x97c70008
0xb1021800 Histogram:Row_1:Tile_0::tile
0x51090000 T105
0x917f0010
0x97cb0008
0x914b0000 Histogram:Row_1:Tile_0::io_count_in
0xb1821800 T99
0xb0cc1980 T100
0x70c70000 T101
0x98421800 Histogram:Row_1:Tile_0::local_bin
0x70c30140 T106
0x90c60000 T107_add
0x70430140 T102
0x90020000 T103_add
0xc0010800 T103
0x60480140 T152
0x90430000 Histogram:Row_1:Tile_0::io_count_out
0x97c30004
0x90010800 T104
0xc8001900 T107
TILE @ (2, 1) NUM_LO_INSTS = 28
0x1000f800 Histogram:Row_1:Tile_1::bins
0x907f0000
0x90430000 Histogram:Row_1:Tile_1::io_data_in
0x90830000 Histogram:Row_1:Tile_1::io_data_out
0x97c50004
0x10808000 Histogram:Row_1:Tile_1::bin__lit_hi__1
0x10c00000 Histogram:Row_1:Tile_1::bin__lit_lo__1
0x88841c00 Histogram:Row_1:Tile_1::bin__lit_cat__1
0xb0421000 Histogram:Row_1:Tile_1::bin
0x90bf0010
0x97c50008
0xb0c21000 Histogram:Row_1:Tile_1::tile
0x50c70800 T96
0xb1021000 T90
0xb0881180 T91
0x70850000 T92
0x98421000 Histogram:Row_1:Tile_1::local_bin
0x70830140 T97
0x90840000 T98_add
0x913f0000
0x91090000 Histogram:Row_1:Tile_1::io_count_in
0x70430140 T93
0x90020000 T94_add
0xc0010800 T94
0x60460100 T153
0x90430000 Histogram:Row_1:Tile_1::io_count_out
0x97c30004
0x90010800 T95
0xc80010c0 T98
TILE @ (3, 1) NUM_LO_INSTS = 28
0x1000f800 Histogram:Row_1:Tile_2::bins
0x10408000 Histogram:Row_1:Tile_2::bin__lit_hi__1
0x90bf0000
0x90850000 Histogram:Row_1:Tile_2::io_data_in
0x90c50000 Histogram:Row_1:Tile_2::io_data_out
0x97c70004
0x10c00000 Histogram:Row_1:Tile_2::bin__lit_lo__1
0x88421c00 Histogram:Row_1:Tile_2::bin__lit_cat__1
0x90ff0010
0x97c70008
0xb0440800 Histogram:Row_1:Tile_2::bin
0xb0821800 Histogram:Row_1:Tile_2::tile
0x50851000 T87
0xb1021800 T81
0xb0c81980 T82
0x70c70000 T83
0x98421800 Histogram:Row_1:Tile_2::local_bin
0x70c30140 T88
0x913f0000
0x91090000 Histogram:Row_1:Tile_2::io_count_in
0x90c60000 T89_add
0x70430140 T84
0x90020000 T85_add
0xc0010800 T85
0x60440100 T154
0x90430000 Histogram:Row_1:Tile_2::io_count_out
0x97c30004
0x90010800 T86
0xc8001880 T89
TILE @ (4, 1) NUM_LO_INSTS = 27
0x1000f800 Histogram:Row_1:Tile_3::bins
0x10408000 Histogram:Row_1:Tile_3::bin__lit_hi__1
0x10800000 Histogram:Row_1:Tile_3::bin__lit_lo__1
0x90ff0000
0x913f0010
0x97c90008
0x90c70000 Histogram:Row_1:Tile_3::io_data_in
0x88421400 Histogram:Row_1:Tile_3::bin__lit_cat__1
0xb0460800 Histogram:Row_1:Tile_3::bin
0xb0822000 Histogram:Row_1:Tile_3::tile
0x50851800 T78
0xb0c22000 T72
0x917f0000
0x914b0000 Histogram:Row_1:Tile_3::io_count_in
0xb0c62180 T73
0x70c70000 T74
0x98421800 Histogram:Row_1:Tile_3::local_bin
0x70c30140 T79
0x90c60000 T80_add
0x70430140 T75
0x90020000 T76_add
0xc0010800 T76
0x60440140 T155
0x90430000 Histogram:Row_1:Tile_3::io_count_out
0x90430000 Histogram:Row_1::io_count_out
0xd8020000 Histogram::io_count_out_1
0x90010800 T77
0xc8001880 T80
TILE @ (1, 2) NUM_LO_INSTS = 30
0x1000f800 Histogram:Row_2:Tile_0::bins
0x10408000 Histogram:Row_2:Tile_0::bin__lit_hi__1
0x10800000 Histogram:Row_2:Tile_0::bin__lit_lo__1
0x88421400 Histogram:Row_2:Tile_0::bin__lit_cat__1
0xd0800000
0x90c50000 Histogram:Row_2::io_data_in
0x90c70000 Histogram:Row_2:Tile_0::io_data_in
0xb0460800 Histogram:Row_2:Tile_0::bin
0x90c70000 Histogram:Row_2:Tile_0::io_data_out
0x97c70004
0x90ff0010
0x97c70008
0xb1021800 Histogram:Row_2:Tile_0::tile
0x51090000 T69
0x917f0010
0x97cb0008
0x914b0000 Histogram:Row_2:Tile_0::io_count_in
0xb1821800 T63
0xb0cc1980 T64
0x70c70000 T65
0x98421800 Histogram:Row_2:Tile_0::local_bin
0x70c30140 T70
0x90c60000 T71_add
0x70430140 T66
0x90020000 T67_add
0xc0010800 T67
0x60480140 T148
0x90430000 Histogram:Row_2:Tile_0::io_count_out
0x97c30004
0x90010800 T68
0xc8001900 T71
TILE @ (2, 2) NUM_LO_INSTS = 28
0x1000f800 Histogram:Row_2:Tile_1::bins
0x907f0000
0x90430000 Histogram:Row_2:Tile_1::io_data_in
0x90830000 Histogram:Row_2:Tile_1::io_data_out
0x97c50004
0x10808000 Histogram:Row_2:Tile_1::bin__lit_hi__1
0x10c00000 Histogram:Row_2:Tile_1::bin__lit_lo__1
0x88841c00 Histogram:Row_2:Tile_1::bin__lit_cat__1
0xb0421000 Histogram:Row_2:Tile_1::bin
0x90bf0010
0x97c50008
0xb0c21000 Histogram:Row_2:Tile_1::tile
0x50c70800 T60
0xb1021000 T54
0xb0881180 T55
0x70850000 T56
0x98421000 Histogram:Row_2:Tile_1::local_bin
0x70830140 T61
0x90840000 T62_add
0x70430140 T57
0x913f0000
0x91090000 Histogram:Row_2:Tile_1::io_count_in
0x90020000 T58_add
0xc0010800 T58
0x60460100 T149
0x90430000 Histogram:Row_2:Tile_1::io_count_out
0x97c30004
0x90010800 T59
0xc80010c0 T62
TILE @ (3, 2) NUM_LO_INSTS = 28
0x1000f800 Histogram:Row_2:Tile_2::bins
0x10408000 Histogram:Row_2:Tile_2::bin__lit_hi__1
0x90bf0000
0x90850000 Histogram:Row_2:Tile_2::io_data_in
0x90c50000 Histogram:Row_2:Tile_2::io_data_out
0x97c70004
0x90ff0010
0x97c70008
0x11000000 Histogram:Row_2:Tile_2::bin__lit_lo__1
0x88422400 Histogram:Row_2:Tile_2::bin__lit_cat__1
0xb0440800 Histogram:Row_2:Tile_2::bin
0xb0821800 Histogram:Row_2:Tile_2::tile
0x50851000 T51
0xb1021800 T45
0xb0c81980 T46
0x70c70000 T47
0x913f0000
0x91090000 Histogram:Row_2:Tile_2::io_count_in
0x98421800 Histogram:Row_2:Tile_2::local_bin
0x70c30140 T52
0x90c60000 T53_add
0x70430140 T48
0x90020000 T49_add
0xc0010800 T49
0x60440100 T150
0x90430000 Histogram:Row_2:Tile_2::io_count_out
0x97c30004
0x90010800 T50
0xc8001880 T53
TILE @ (4, 2) NUM_LO_INSTS = 27
0x1000f800 Histogram:Row_2:Tile_3::bins
0x10408000 Histogram:Row_2:Tile_3::bin__lit_hi__1
0x90bf0010
0x97c50008
0x90ff0000
0x90c70000 Histogram:Row_2:Tile_3::io_data_in
0x11000000 Histogram:Row_2:Tile_3::bin__lit_lo__1
0x88422400 Histogram:Row_2:Tile_3::bin__lit_cat__1
0xb0460800 Histogram:Row_2:Tile_3::bin
0xb0c21000 Histogram:Row_2:Tile_3::tile
0x50c71800 T42
0xb1021000 T36
0x917f0000
0x914b0000 Histogram:Row_2:Tile_3::io_count_in
0xb0881180 T37
0x70850000 T38
0x98421000 Histogram:Row_2:Tile_3::local_bin
0x70830140 T43
0x90840000 T44_add
0x70430140 T39
0x90020000 T40_add
0xc0010800 T40
0x60460140 T151
0x90430000 Histogram:Row_2:Tile_3::io_count_out
0x90430000 Histogram:Row_2::io_count_out
0xd8020000 Histogram::io_count_out_2
0x90010800 T41
0xc80010c0 T44
TILE @ (1, 3) NUM_LO_INSTS = 30
0x1000f800 Histogram:Row_3:Tile_0::bins
0x10408000 Histogram:Row_3:Tile_0::bin__lit_hi__1
0x10800000 Histogram:Row_3:Tile_0::bin__lit_lo__1
0x88421400 Histogram:Row_3:Tile_0::bin__lit_cat__1
0xd0800000
0x90c50000 Histogram:Row_3::io_data_in
0x90c70000 Histogram:Row_3:Tile_0::io_data_in
0xb0460800 Histogram:Row_3:Tile_0::bin
0x90c70000 Histogram:Row_3:Tile_0::io_data_out
0x97c70004
0x90ff0020
0x97c70008
0xb1021800 Histogram:Row_3:Tile_0::tile
0x51090000 T33
0x917f0020
0x97cb0008
0x914b0000 Histogram:Row_3:Tile_0::io_count_in
0xb1821800 T27
0xb0cc1980 T28
0x70c70000 T29
0x98421800 Histogram:Row_3:Tile_0::local_bin
0x70c30140 T34
0x90c60000 T35_add
0x70430140 T30
0x90020000 T31_add
0xc0010800 T31
0x60480140 T144
0x90430000 Histogram:Row_3:Tile_0::io_count_out
0x97c30004
0x90010800 T32
0xc8001900 T35
TILE @ (2, 3) NUM_LO_INSTS = 29
0x1000f800 Histogram:Row_3:Tile_1::bins
0x907f0000
0x90430000 Histogram:Row_3:Tile_1::io_data_in
0x90830000 Histogram:Row_3:Tile_1::io_data_out
0x97c50004
0x10808000 Histogram:Row_3:Tile_1::bin__lit_hi__1
0x10c00000 Histogram:Row_3:Tile_1::bin__lit_lo__1
0x88841c00 Histogram:Row_3:Tile_1::bin__lit_cat__1
0xb0421000 Histogram:Row_3:Tile_1::bin
0x90bf0020
0x97c50001
0x97c50008
0xb0c21000 Histogram:Row_3:Tile_1::tile
0x50c70800 T24
0xb1021021 T18
0xb0881180 T19
0x70850000 T20
0x98421000 Histogram:Row_3:Tile_1::local_bin
0x70830140 T25
0x90840000 T26_add
0x70430140 T21
0x913f0000
0x91090000 Histogram:Row_3:Tile_1::io_count_in
0x90020000 T22_add
0xc0010800 T22
0x60460100 T145
0x90430000 Histogram:Row_3:Tile_1::io_count_out
0x97c30004
0x90010800 T23
0xc80010c0 T26
TILE @ (3, 3) NUM_LO_INSTS = 30
0x1000f800 Histogram:Row_3:Tile_2::bins
0x10408000 Histogram:Row_3:Tile_2::bin__lit_hi__1
0x90bf0000
0x90ff0020
0x97c70001
0x97c70008
0x90850000 Histogram:Row_3:Tile_2::io_data_in
0x91050000 Histogram:Row_3:Tile_2::io_data_out
0x97c90004
0x00000021
0x11000000 Histogram:Row_3:Tile_2::bin__lit_lo__1
0x88422400 Histogram:Row_3:Tile_2::bin__lit_cat__1
0xb0440800 Histogram:Row_3:Tile_2::bin
0xb0821800 Histogram:Row_3:Tile_2::tile
0x50851000 T15
0xb1021800 T9
0x917f0000
0x914b0000 Histogram:Row_3:Tile_2::io_count_in
0xb0c81980 T10
0x70c70000 T11
0x98421800 Histogram:Row_3:Tile_2::local_bin
0x70c30140 T16
0x90c60000 T17_add
0x70430140 T12
0x90020000 T13_add
0xc0010800 T13
0x60440140 T146
0x90430000 Histogram:Row_3:Tile_2::io_count_out
0x97c30004
0x90010800 T14
0xc8001880 T17
TILE @ (4, 3) NUM_LO_INSTS = 29
0x1000f800 Histogram:Row_3:Tile_3::bins
0x10400800 T0__lit__1
0x97c30001
0x97c30008
0x10808000 Histogram:Row_3:Tile_3::bin__lit_hi__1
0x10c00000 Histogram:Row_3:Tile_3::bin__lit_lo__1
0x88841c00 Histogram:Row_3:Tile_3::bin__lit_cat__1
0x97c50001
0x90ff0000
0x90c70000 Histogram:Row_3:Tile_3::io_data_in
0xb0c61000 Histogram:Row_3:Tile_3::bin
0xb1060800 Histogram:Row_3:Tile_3::tile
0x51091800 T6
0xb1460800 T0
0x91bf0000
0x918d0000 Histogram:Row_3:Tile_3::io_count_in
0xb14a0980 T1
0x714b0000 T2
0x98c62800 Histogram:Row_3:Tile_3::local_bin
0x71470140 T7
0x914a0000 T8_add
0x70c70140 T3
0x90060000 T4_add
0xc0010800 T4
0x60c80180 T147
0x90c70000 Histogram:Row_3:Tile_3::io_count_out
0x90c70000 Histogram:Row_3::io_count_out
0xd8060000 Histogram::io_count_out_3
0x90010800 T5
0xc8002900 T8
EOF

#include "harness.bash"
