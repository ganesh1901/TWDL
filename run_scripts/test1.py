#li="1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK|1-OK,0-Not OK"

#print len(li.split('|'))
tt="1|2|3|4-7|8-9|10-11|12-13|14|15"
name = "SENSOR_DATA_VAL|SENSOR_TEMP|MISSION_D|INS_ACT_M|ALIGN_M|ATTITUDE_AVL|OP_MODE|HYBRIDIZATION|INS_RDY"

for i in range(len(name.split('|'))):
	print i, name.split("|")[i]


li = "1-Data Not valid,0-Data Valid|1-NOT OK,0-OK|1-Data required,0-Data not required|0001-Standby,0010-Position test,0111-Mission data load,0110-Mission Data dump,0100-Alignment,1000-Navigation|01-static leveling,10-Two"
ti = (li.split('|'))
for i in range(len(ti)):
	print i, ti[i]


