filename = AskFile(1, "*.bin", "Output file name")
address = 0x009DD5B8
size = 0x37a0
with open(filename, "wb") as out:
    data = GetManyBytes(address, size, 1)
    out.write(data)