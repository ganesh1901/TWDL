import re

def parse_bytetobit(word, bit_index):
        #word = int(word)
	print hex(word), bin(word)
        pattern = "([0-9]{1,2})(-)([0-9]{1,2}) | ([0-9]{1,2})"
        rx = re.compile(pattern, re.VERBOSE)

        x = ''
        bit = []
        a = rx.findall(bit_index)
	print 'bit_index', bit_index
        print 'rx o/p', a
        for i in range(len(a)):
            if a[i][1] != "-":
		print 'before', bin(word)
                word1 = word >> int((a[i][3]))-1
                x = str(word1 & 1)
		print 'iiii', bin(word1)
            else:
                x = ''
                for j in range(0, int(a[i][2]) - int(a[i][0]) + 1):
                    word1 = word >> (int(a[i][0]) + j)
                    x += str(word1 & 1)

            bit.append(x)
        # print bit
        return bit

if __name__ == "__main__":
	print 'op' , parse_bytetobit(0x0803, "8,9,10,11,12,13,14,15,0-7")
