def romanize(x):
 y = ""
 while(x >= 1000):
  y += "M"
  x -= 500
  x -= 500
 if (x >= 500):
  y += "D"
  x -= 500
 while (x >= 100):
  x -= 100
  y += "C"
 if (x >= 90):
  x -= 90
  y += "XC"
 if (x >= 50):
  x -= 50
  y += "L"
 if (x >= 40):
  x -= 40
  y += "XL"
 while (x >= 10):
  x -= 10
  y += "X"
 if (x >= 9):
  x -= 9
  y += "IX"
 if (x >= 5):
  x -= 5
  y += "V"
 if (x >= 4):
  x -= 4
  y += "IV"
 while (x >= 1):
  x -= 1
  y += "I"
 return y

def deromanize(s):
 prv = ''
 nxt = ''
 for c in s:
  if (c == 'M')
