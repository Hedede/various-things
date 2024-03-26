#!/usr/bin/env ruby

for a in (1..1000)
	for b in (1..a)
		c = 1000 - (a + b)

		if (a*a + b*b == c*c)
			puts a*b*c
			exit
		end
	end
end
