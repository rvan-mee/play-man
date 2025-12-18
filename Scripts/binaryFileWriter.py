file_path = "../testing/test-data/custom_gb_test_roms/workram_address.gb"

file = open(file_path, "wb")
binary_data = bytearray([0xC0, 0x00])  
file.write(binary_data)
file.close()
