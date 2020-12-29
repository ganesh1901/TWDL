pos_fmt = "=ffffffffffffHHhhhhhh"
lev_fmt = "=fffffffffffhhHHfff"
nav_16_fmt = "=fffffffffffffhHHHHH"
nav_17_fmt = "=fffffffffffffffHH"
nav_18_fmt = "=fffffffffffffHHff"
nav_19_fmt = "=ffffffffffffffff"
nav_20_fmt = "=ffffffffffffIIIHH"
nav_21_fmt = "=fHHHHHHHHHHHHHHHHHBBBBHIhhhhhhhh"
nav_22_fmt = "=fffffffffffffiIHH"
ema_fmt = "=HHIhhhhhhhhhhhhhhhhhhhh6HI"
obc_surv_fmt = "=HHHHHhhhhhhhhhhhhHIHHHHHHHHHHI"
twdl_health_fmt = "=HHIHHHHHHHHHHH16H"


fmt_list = []
fmt_list.append(pos_fmt)
fmt_list.append(lev_fmt)
fmt_list.append(nav_16_fmt)
fmt_list.append(nav_17_fmt)
fmt_list.append(nav_18_fmt)
fmt_list.append(nav_19_fmt)
fmt_list.append(nav_20_fmt)
fmt_list.append(nav_21_fmt)
fmt_list.append(nav_22_fmt)
fmt_list.append(ema_fmt)
fmt_list.append(obc_surv_fmt)
fmt_list.append(twdl_health_fmt)


pos_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]
lev_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17]
nav_16_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18]
nav_17_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
nav_18_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16]
nav_19_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
nav_20_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
nav_21_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                28, 29, 30, 31]
nav_22_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
ema_index = [3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 29]
obc_surv_index = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                  29]

index_list = []
index_list.append(pos_index)
index_list.append(lev_index)
index_list.append(nav_16_index)
index_list.append(nav_17_index)
index_list.append(nav_18_index)
index_list.append(nav_19_index)
index_list.append(nav_20_index)
index_list.append(nav_21_index)
index_list.append(nav_22_index)
index_list.append(ema_index)
index_list.append(obc_surv_index)

pos_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0.0025, -1, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01]
lev_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, .01, .01, -1, -1, -1, -1, -1]
nav_16_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, .01, -1, -1, -1, -1, -1]
nav_17_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_18_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_19_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_20_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_21_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1]
nav_22_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]

ema_scale = [0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177,
             0.000190476, 0.000190476, 0.000190476, 0.000190476, 0.0439453, 0.0439453, 0.0439453, 0.0439453,
             0.005493177, 0.005493177, 0.005493177, 0.005493177, -1]

obc_surv_scale = [0.000305176, 0.000305176, 0.000305176, 0.000305176, 0.005493177, 0.005493177, 0.005493177,
                  0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.00190476, 0.00190476,
                  0.00190476, 0.00190476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]

scale_list = []
scale_list.append(pos_scale)
scale_list.append(lev_scale)
scale_list.append(nav_16_scale)
scale_list.append(nav_17_scale)
scale_list.append(nav_18_scale)
scale_list.append(nav_19_scale)
scale_list.append(nav_20_scale)
scale_list.append(nav_21_scale)
scale_list.append(nav_22_scale)
scale_list.append(ema_scale)
scale_list.append(obc_surv_scale)

index_map = {}
index_map["GPS"] = [5, 6, 7, 8]
index_map["NAV"] = [3, 4]
index_map["INS_SURV"] = [2]
index_map["INS_POS"] = [0]
index_map["INS_LEV"] = [1]
index_map["EMA"] = [9]
index_map["OBC_SURV"] = [10]
index_map["TWDL_UPLINK"] = [] 
index_map["TWDL_DNLINK"] = []

logfile_map = {}
logfile_map["GPS"] = ["../out/gps-19.bin", "../out/gps-20.bin", "../out/gps-21.bin", "../out/gps-22.bin"]
logfile_map["NAV"] = ["../out/nav-17.bin", "../out/nav-18.bin"]
logfile_map["INS_SURV"] = ["../out/ins.bin"]
logfile_map["INS_POS"] = ["../out/ins_pos.bin"]
logfile_map["INS_LEV"] = ["../out/lev.bin"]
logfile_map["EMA"] = ["../out/ema.bin"]
logfile_map["OBC_SURV"] = ["../out/obc_surv"]



sheet_name_map = {}
sheet_name_map[0] = "POSITION"
sheet_name_map[1] = "LEVELLING"
sheet_name_map[2] = "INS_SURV"
sheet_name_map[3] = "NAV-17"
sheet_name_map[4] = "NAV-18"
sheet_name_map[5] = "NAV-19"
sheet_name_map[6] = "NAV-20"
sheet_name_map[7] = "NAV-21"
sheet_name_map[8] = "NAV-22"
sheet_name_map[9] = "EMA"
sheet_name_map[10] = "OBC_SURV"
