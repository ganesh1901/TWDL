import struct
'''

li = [-7886.00]
a = struct.pack('=d', *li)


aa = bytearray(a)
t1 = []

for i in range(len(aa)):
    t1.append(aa[i])

print t1

#t1[0] = 1
# print t1

t2 = bytearray(t1)

bb = struct.unpack('d', t2)
print bb


mission_data_load_limits = [[-1, -1], [1, [-90, 90]], [1, [-179.99, 180]], [-1, -1], [1, [-179.99, 180]],
                                [1, [-179.99, 180]], [1, [-90, 90]], [2, [10, 60]], [2, [10, 300]], [-1, -1], [-1, -1]]

limit = mission_data_load_limits

'''

import re
pattern = "^(?=.)([+-]?([0-9]*)(\.[0-9]+))?)$"
rx1 = re.compile(pattern, re.VERBOSE)

