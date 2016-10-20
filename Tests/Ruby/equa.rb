def equa(a, b, t)
 while (a-b).abs > t
  c = 0
  if (a > b)
   c = a / 2
   a = a - c/2
   b = b + c/2
  else
   c = b / 2
   a = a + c/2
   b = b - c/2
  end
  puts "#{a}, #{b}"
 end
end

