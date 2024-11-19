from PIL import Image
from PIL import ImageEnhance
import pytesseract

# 打开图片文件
img = Image.open('/home/jagger/Desktop/compiler-front/flex/photo/img.png')

# 将图片转换为RGB模式，确保图片兼容性
img = img.convert('RGB')  # 这里也可以尝试使用L（灰度模式）

# 创建颜色增强器对象
enhancer = ImageEnhance.Color(img)

# 去色处理，将图片转换为黑白图像
enhancer = enhancer.enhance(0)

# 创建亮度增强器对象
enhancer = ImageEnhance.Brightness(enhancer)

# 提高图片亮度，使图片更亮
enhancer = enhancer.enhance(2)

# 创建对比度增强器对象
enhancer = ImageEnhance.Contrast(enhancer)

# 增加图片对比度，使图片中的字符更加清晰
enhancer = enhancer.enhance(8)

# 创建锐化增强器对象
enhancer = ImageEnhance.Sharpness(enhancer)

# 大幅提高图片的锐度，以更好地识别文本
img = enhancer.enhance(20)

# 使用pytesseract库从处理后的图片中提取文本
text = pytesseract.image_to_string(img)

# 去除文本末尾的FF标识
new_text = text[:-1]

# 输出识别到的文本
#print(new_text)

# 将识别到的文本写入到指定的文件中
file = open('/home/jagger/Desktop/compiler-front/flex/test/in.txt', 'w')
file.write(new_text)
file.close()  # 关闭文件，释放资源