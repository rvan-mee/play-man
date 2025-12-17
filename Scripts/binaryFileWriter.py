file_path = "../testing/test-data/custom_gb_test_roms/load_immediate_8.gb"

file = open(file_path, "wb")
binary_data = bytearray([0x0F])  
file.write(binary_data)
file.close()
