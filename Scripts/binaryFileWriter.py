file_path = "../testing/test-data/custom_gb_test_roms/jump_relative_test.gb"

file = open(file_path, "wb")
binary_data = bytearray([0x1])
for _ in range(1024): 
    file.write(binary_data)
file.write(bytearray([0xFF]))
file.close()
