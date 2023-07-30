def weird(x)
	x = x & 0x80000001
	v = x == 0
	if (x >= 0x80000000)
		a = (x-1)|0xFFFFFFFE
		b = -1
		v = (a&0xFFFFFFFF)==(b&0xFFFFFFFF)
	end
	v
end
