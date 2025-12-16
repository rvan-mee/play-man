file_path = "../testing/test-data/custom_gb_test_roms/load_immediate_16.gb"

file = open(file_path, "wb")
binary_data = bytearray([0xF0, 0x0F])  
file.write(binary_data)
file.close()
