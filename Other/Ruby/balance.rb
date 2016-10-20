$hp_max1 = 60.0
$hp_cur1 = $hp_max1
$dmg1 = (22.0*0.65-4)
$count1 = 5
$prev_count1 = $count1

$hp_max2 = 180.0
$hp_cur2 = $hp_max1
$dmg2 = 22.0
$count2 = 2



def turn1
 $prev_count1 = $count1
 dmg = $dmg2*$count2
 if dmg > $hp_cur1
  overkill = $dmg2 * ($count2-1) / $hp_max1
  $count1 = $count1 - (1 + overkill.floor)
  $hp_cur1 = $hp_max1 - ((dmg - $hp_max1*overkill.floor) - $hp_cur1)
 else
  $hp_cur1 = $hp_cur1 - dmg
 end
 print "#{$count1}\n"
 print "#{$hp_cur1}\n"
end
def turn2
 dmg = $dmg1*$prev_count1
 if dmg > $hp_cur2
  overkill = $dmg1 * ($prev_count1-1) / $hp_max2
  $count2 = $count2 - (1 + overkill.floor)
  $hp_cur2 = $hp_cur2 - (dmg - $hp_max2*overkill.floor)
 else
  $hp_cur2 = $hp_cur2 - dmg
 end
 print "#{$count2}\n"
 print "#{$hp_cur2}\n"
end

#turn1();
#turn2();
