from PIL import Image
import numpy as np

# Đường dẫn đến file txt chứa dữ liệu pixel
file_path = "output.txt"
file_path1 = "output1.txt"
file_path2 = "output2.txt"
file_path3 = "output3.txt"
file_path4 = "root.txt"
# Đọc dữ liệu pixel từ file txt
with open(file_path, 'r') as file:
    pixel_values = [int(value) for value in file.read().split(',')]

# Tạo hình ảnh mới với kích thước 10x10
image_width = 88
image_height = 88
image = Image.new("L", (image_width, image_height))

# Đặt giá trị pixel cho hình ảnh
image.putdata(pixel_values)

# Hiển thị hình ảnh
image.show()


with open(file_path1, 'r') as file:
    pixel_values = [int(value) for value in file.read().split(',')]

# Tạo hình ảnh mới với kích thước 10x10
image_width = 88
image_height = 88
image1 = Image.new("L", (image_width, image_height))

# Đặt giá trị pixel cho hình ảnh
image1.putdata(pixel_values)

# Hiển thị hình ảnh
image1.show()



with open(file_path2, 'r') as file:
    pixel_values = [int(value) for value in file.read().split(',')]

# Tạo hình ảnh mới với kích thước 10x10
image_width = 88
image_height = 88
image2 = Image.new("L", (image_width, image_height))

# Đặt giá trị pixel cho hình ảnh
image2.putdata(pixel_values)

# Hiển thị hình ảnh
image2.show()


with open(file_path3, 'r') as file:
    pixel_values = [int(value) for value in file.read().split(',')]

# Tạo hình ảnh mới với kích thước 10x10
image_width = 88
image_height = 88
image3 = Image.new("L", (image_width, image_height))

# Đặt giá trị pixel cho hình ảnh
image3.putdata(pixel_values)

# Hiển thị hình ảnh
image3.show()

