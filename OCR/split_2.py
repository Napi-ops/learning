from matplotlib import pyplot as plt
from PIL import Image
import pytesseract
import numpy as np
import cv2

rawimg = cv2.imread(r"E:\OCR\OCR_Test\p_5.jpg")
fig = plt.figure(figsize=(10,15))
fig.add_subplot(2,3,1)
plt.title("raw image")
plt.imshow(rawimg)

fig.add_subplot(2,3,2)
plt.title("grey scale image")
# 在处理之前，我们首先应该将图像去RGB，即在它对应的灰度图像上进行处理。
# 我们可以使用opencv python库中的cvtColor函数来实现到灰度图像的转换
grayscaleimg = cv2.cvtColor(rawimg,cv2.COLOR_BGR2GRAY)
plt.imshow(grayscaleimg,cmap='gray')

# 然后，我们需要对图片进行归一化，这样可以减少最后分割出的数字中的噪声
# 这里我们采取了对每个像素减去图像总像素的平均数，并设置阈值50以下的像素归零来实现归一化
# 这样基本上背景像素就变成0了
grayscaleimg = grayscaleimg - int(np.mean(grayscaleimg))
grayscaleimg[grayscaleimg < 50] = 0
# counting non-zero value by row , axis y
# 可以得到字符高的边界
row_nz = []
for row in grayscaleimg.tolist():
    row_nz.append(len(row) - row.count(0))
fig.add_subplot(2,3,3)
plt.title("non-zero values on y (by row)")
plt.plot(row_nz)

# counting non-zero value by column, x axis
# 可以得到字符宽的边界，波形的波谷即间隔
col_nz = []
for col in grayscaleimg.T.tolist():
    col_nz.append(len(col) - col.count(0))
fig.add_subplot(2,3,4)
plt.title("non-zero values on y (by col)")
plt.plot(col_nz)

##### start split
# first find upper and lower boundary of y (row)
fig.add_subplot(2,3,5)
plt.title("y boudary deleted")
upper_y = 0
# 遇到行不为0，即有数字时，记录行数
for i,x in enumerate(col_nz):
    if x != 0:
        upper_y = i
        break
lower_y = 0
for i,x in enumerate(col_nz[::-1]):
    if x!=0:
        lower_y = len(col_nz) - i
        break
sliced_y_img = grayscaleimg[:,upper_y:lower_y]
plt.imshow(sliced_y_img)

# then we find left and right boundary of every digital (x, on column)
column_boundary_list = []
record = False
# list[:-1],slice all the list without the last one
for i,x in enumerate(row_nz[:-1]):
	# 寻找边界i
    if (row_nz[i] == 0 and row_nz[i+1] != 0) or row_nz[i] != 0 and row_nz[i+1] == 0:
        column_boundary_list.append(i+1)
img_list = []
# i是所有左边界，[i:i+2]切片得到每个字符的左右边界
xl = [ column_boundary_list[i:i+2] for i in range(0,len(column_boundary_list),2) ]
for x in xl:
    img_list.append(sliced_y_img[x[0]:x[1],:] )
    
# del invalid image
# 删去宽度不大于5像素的错误图片
img_list = [ x for x in img_list if x.shape[1] > 5 ]

# show image
fig = plt.figure()
plt.title("y boudary deleted")
f=open(r"E:\OCR\OCR_Test\output.txt","w")
for i,img in enumerate(img_list):
    fig.add_subplot(3,4,i+1)
    plt.imshow(img)
    code=pytesseract.image_to_string(img)
    code=str(code)
    f.write(code)
    plt.imsave(r"E:\OCR\OCR_Test\result\%s.jpg"%i,img)

f.close()
plt.show()