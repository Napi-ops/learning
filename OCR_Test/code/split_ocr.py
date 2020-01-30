import pytesseract
from PIL import Image
image = Image.open(r"E:\OCR\OCR_Test\p_1.jpg")
f=open(r"E:\OCR\OCR_Test\output.txt","w")
code = pytesseract.image_to_string(image)
code=str(code)
f.write(code)
print(code)
f.close()