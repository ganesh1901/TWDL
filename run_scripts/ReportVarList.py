pos_fmt = "=ffffffffffffHHhhhhhh"
lev_fmt = "=fffffffffffhhHHfff"
nav_16_fmt = "=fffffffffffff6H"
nav_17_fmt = "=15fHH"
nav_18_fmt = "=15fhH"
nav_19_fmt = "=15fHH"
nav_20_fmt = "=14fIHH"
nav_21_fmt = "=f14B14BHBBBBH4Ii2H"
nav_22_fmt = "=15f2H"
ema_fmt = "=HHIhhhhhhhhhhhhhhhhhhhh6H2H"
obc_surv_fmt = "=5H12h15H"
twdl_health_fmt = "=HHIHHHHHHHHHHH16H"


pos_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19]
pos_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0.0025, -1, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01]
pos_display = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0.0025, -1, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01]

ema_index = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30]
ema_scale = [-1, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.000190476, 0.000190476, 0.000190476, 0.000190476, 0.04394542, 0.04394542, 0.04394542, 0.04394542, 0.005493177,  0.005493177,  0.005493177,  0.005493177, -1, -1, -1, -1, -1, -1, -3, -4]
ema_display = ["%d", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%04X", "%04X", "%04X", "%04X","%04X", "%04X", "%6.4f", -4]




obc_surv_index = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
obc_surv_scale = [-2, -2, -2, -2, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.005493177, 0.000190476, 0.000190476, 0.000190476, 0.000190476, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0.01, -1, -3, -4]
obc_surv_display = [ "%4.6f", "%4.6f", "%4.6f", "%4.6f", "%4.6f", "%4.6f","%4.6f", "%4.6f", "%4.6f", "%4.6f","%4.6f", "%4.6f",  "%4.6f", "%4.6f","%4.6f", "%4.6f", "%04X", "%04X", "%04X", "%04X", "%04X", "%04X", "%04X", "%04X", "%d", "%d", "%d", "%4.2f", "%04X", "%6.4f", -4]




lev_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, .01, .01, -1, -1, -1,]
lev_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17]
lev_display = ["%6.4f", "%6.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f"]


nav_16_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, .0025, -1, -1, -1, -1, -1]
nav_16_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18]
nav_16_display = ["%6.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.4f", -1, -1, -1, -1, "%04X"]




nav_17_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_17_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16]
nav_17_display = ["%6.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%04X"]



nav_18_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0.01, -1]
nav_18_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
nav_18_display = ["%6.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.2f", "%4.2f", "%04X"]



nav_19_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16]
nav_19_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_19_display = ["%6.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%04X"]


nav_20_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
nav_20_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_20_display = ["%6.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", -1, "%04X", "%04X"]

nav_22_index = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
nav_22_scale = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
nav_22_display = ["%6.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%3.8f", "%3.8f", "%5.4f", "%5.4f", "%6.8f", "%6.8f", "%6.8f", "%6.8f", "%04X", "%04X"]


nav_21_scale = []
nav_21_index = []
nav_21_display = []
for i  in  range(42):
    nav_21_scale.append(-1)
    nav_21_index.append(i)
    nav_21_display.append(-1)
nav_21_display[0] = "%6.4f"


















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


display_list = []
display_list.append(pos_display)
display_list.append(lev_display)
display_list.append(nav_16_display)
display_list.append(nav_17_display)
display_list.append(nav_18_display)
display_list.append(nav_19_display)
display_list.append(nav_20_display)
display_list.append(nav_21_display)
display_list.append(nav_22_display)
display_list.append(ema_display)
display_list.append(obc_surv_display)


index_map = {}
index_map["GPS"] = [5, 6, 7, 8]
index_map["NAV"] = [3, 4]
index_map["INS_SURV"] = [2]
index_map["INS_POS"] = [0]
index_map["INS_LEV"] = [1]
index_map["EMA"] = [9]
index_map["OBC_SURV"] = [10]

logfile_map = {}
logfile_map["GPS"] = ["../out/nav-19.bin", "../out/nav-20.bin", "../out/nav-21.bin", "../out/nav-22.bin"]
logfile_map["NAV"] = ["../out/nav-17.bin", "../out/nav-18.bin"]
logfile_map["INS_SURV"] = ["../out/ins.bin"]
logfile_map["INS_POS"] = ["../out/ins_pos.bin"]
logfile_map["INS_LEV"] = ["../out/lev.bin"]
logfile_map["EMA"] = ["../out/ema.bin"]
logfile_map["OBC_SURV"] = ["../out/sur.bin"]



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
