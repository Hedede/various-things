def CopyShush(dest, src, n):
  for i in range(n):
    idc.PatchByte(dest+i, idc.Byte(src+i));