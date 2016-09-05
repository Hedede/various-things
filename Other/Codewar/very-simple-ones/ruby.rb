def weirdcase string
  string.split(' ').map do |s|
    s.each_char.with_index.map{ |c, i| (i % 2 == 0) ? c.upcase : c.downcase }.join()
  end.join(' ')
end
