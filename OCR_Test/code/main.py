
# -*- coding:utf-8 -*-

import os
import sys
import json
import time
import base64
from tkinter import *
from PIL import Image,ImageTk,ImageDraw,ImageFont

IS_PY3 = sys.version_info.major == 3
if IS_PY3:
    from urllib.request import urlopen
    from urllib.request import Request
    from urllib.error import URLError
    from urllib.parse import urlencode
    from urllib.parse import quote_plus
else:
    import urllib2
    from urllib import quote_plus
    from urllib2 import urlopen
    from urllib2 import Request
    from urllib2 import URLError
    from urllib import urlencode

import ssl
ssl._create_default_https_context = ssl._create_unverified_context

API_KEY = 'eezZErzsGLxIow9T0Wf5lfMr'

SECRET_KEY = 'OZZm7vx7mftXlN0iDObmNmKEekF8QS24'

OCR_URL = "https://aip.baidubce.com/rest/2.0/ocr/v1/accurate"

TOKEN_URL = 'https://aip.baidubce.com/oauth/2.0/token'

global img

global tag

global pict

global mode

global winNew

tag=TRUE

#获取token
def fetch_token():
    params = {'grant_type': 'client_credentials',
              'client_id': API_KEY,
              'client_secret': SECRET_KEY}
    post_data = urlencode(params)
    if (IS_PY3):
        post_data = post_data.encode('utf-8')
    req = Request(TOKEN_URL, post_data)
    try:
        f = urlopen(req, timeout=5)
        result_str = f.read()
    except URLError as err:
        print(err)
    if (IS_PY3):
        result_str = result_str.decode()

    result = json.loads(result_str)

    if ('access_token' in result.keys() and 'scope' in result.keys()):
        if not 'brain_all_scope' in result['scope'].split(' '):
            print ('please ensure has check the  ability')
            exit()
        return result['access_token']
    else:
        print ('please overwrite the correct API_KEY and SECRET_KEY')
        exit()

#得到识别文本
def request(url, data):
    req = Request(url, data.encode('utf-8'))
    has_error = False
    try:
        f = urlopen(req)
        result_str = f.read()
        if (IS_PY3):
            result_str = result_str.decode()
        return result_str
    except  URLError as err:
        print(err)

#得到识别文本后，开始计算结果(做题模式)
def calculator_mode1(result):
    for ch in result:
        if u'\u4e00'<=ch<=u'\u9fff':
            return None
    return eval(result.strip('='))

#得到识别文本后，开始计算结果(批改模式)
def calculator_mode2(result):
    index=0
    for ch in result:
        if u'\u4e00'<=ch<=u'\u9fff':
            return None
    for ch in result:
        if ch=='=':
            break
        index=index+1
    if (index+1)==len(result):
        return '×'
    if (eval(result[:index])==int(result[index+1:])):
        return '√'
    else:
        return '×'
        
#做题模式
def run_mode1():
    global img
    global tag
    global pict
    global mode
    global winNew
    mode=1
    try:
        if tag:
            #上传图片
            pict_full_name=os.popen(r'adb shell ls /storage/emulated/0/DCIM/Camera')
            pict_name=pict_full_name.read()
            pict=pict_name.strip('.jpg\n')
            os.system(r'adb pull storage/emulated/0/DCIM/Camera/%s.jpg E:\OCR\OCR_Test\pictures'%pict)
            os.system(r'adb shell rm -rf /storage/emulated/0/DCIM/Camera/%s.jpg'%pict)
            tag=FALSE
        #子界面部件
        load=Image.open(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict)
        load=load.resize((300,300),Image.ANTIALIAS)
        img=ImageTk.PhotoImage(load)
        
        winNew=Toplevel(root)
        winNew.geometry('500x500')
        winNew.title('确认界面')
        
        lb2=Label(winNew,text='确认是以下图像吗？',fg='black',font=('华文新魏',32),width=20,height=2,relief=FLAT)
        lb2.pack()
        
        lb3=Label(winNew,image=img)
        lb3.pack()
        
        but2=Button(winNew,text='是的，开始',command=run_identify)
        but2.place(relx=0.2,rely=0.8,relheight=0.1,relwidth=0.2)
        
        but3=Button(winNew,text='不是，重新拍摄',command=run_destroy)
        but3.place(relx=0.6,rely=0.8,relheight=0.1,relwidth=0.2)
        
        but_rotate=Button(winNew,text='旋转图片',command=run_rotate)
        but_rotate.place(relx=0.4,rely=0.8,relheight=0.1,relwidth=0.2)
        
    except:
        winError=Toplevel(root)
        winError.geometry('500x500')
        winError.title('识别失败！')
        
        lb4=Message(winError,text='图像识别失败！\n可能问题:\n1、相册中无照片\n2、有多张照片',fg='black',font=('华文新魏',24),width=500,relief=FLAT)
        lb4.place(relx=0.2,y=80,relheight=0.4,width=300)
        
        but4=Button(winError,text='关闭',command=winError.destroy)
        but4.place(relx=0.35,rely=0.8,relheight=0.1,relwidth=0.2)
        
        tag=TRUE

#批改模式
def run_mode2():
    global img
    global tag
    global pict
    global mode
    global winNew
    mode=2
    try:
        if tag:
            #上传图片
            pict_full_name=os.popen(r'adb shell ls /storage/emulated/0/DCIM/Camera')
            pict_name=pict_full_name.read()
            pict=pict_name.strip('.jpg\n')
            os.system(r'adb pull storage/emulated/0/DCIM/Camera/%s.jpg E:\OCR\OCR_Test\pictures'%pict)
            os.system(r'adb shell rm -rf /storage/emulated/0/DCIM/Camera/%s.jpg'%pict)
            tag=FALSE
        #子界面部件
        load=Image.open(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict)
        load=load.resize((300,300),Image.ANTIALIAS)
        img=ImageTk.PhotoImage(load)
        
        winNew=Toplevel(root)
        winNew.geometry('500x500')
        winNew.title('确认界面')
        
        lb2=Label(winNew,text='确认是以下图像吗？',fg='black',font=('华文新魏',32),width=20,height=2,relief=FLAT)
        lb2.pack()
        
        lb3=Label(winNew,image=img)
        lb3.pack()
        
        but2=Button(winNew,text='是的，开始',command=run_identify)
        but2.place(relx=0.2,rely=0.8,relheight=0.1,relwidth=0.2)
        
        but3=Button(winNew,text='不是，重新拍摄',command=run_destroy)
        but3.place(relx=0.6,rely=0.8,relheight=0.1,relwidth=0.2)
        
        but_rotate=Button(winNew,text='旋转图片',command=run_rotate)
        but_rotate.place(relx=0.4,rely=0.8,relheight=0.1,relwidth=0.2)
        
    except:
        winError=Toplevel(root)
        winError.geometry('500x500')
        winError.title('识别失败！')
        
        lb4=Message(winError,text='图像识别失败！\n可能问题:\n1、相册中无照片\n2、有多张照片',fg='black',font=('华文新魏',24),width=500,relief=FLAT)
        lb4.place(relx=0.2,y=80,relheight=0.4,width=300)
        
        but4=Button(winError,text='关闭',command=winError.destroy)
        but4.place(relx=0.35,rely=0.8,relheight=0.1,relwidth=0.2)
        
        tag=TRUE        

#确认图片无误后开始识别
def run_identify():
    global mode
    global pict
    global winNew
    try:
        #获取token
        token = fetch_token()
        image_url = OCR_URL + "?access_token=" + token
        
        #调用API返回识别结果
        f=open(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict,'rb')     
        file_content = f.read()
        f.close()
        result = request(image_url, urlencode({'image': base64.b64encode(file_content)}))
        result_json = json.loads(result)
        
        if mode==1:
            #将识别结果保存在文本中(做题模式)
            #保存答案
            f=open(r"E:\OCR\OCR_Test\output\output.txt","w")
            for words_result in result_json["words_result"]:
                f.write(words_result["words"])
                if calculator_mode1(words_result["words"])!=None:
                    f.write(str(calculator_mode1(words_result["words"])))
                f.write("\n")
            f.close()
            #保存答案位置
            f=open(r"E:\OCR\OCR_Test\output\location.txt","w")
            f.write("x,y,height:\n")
            for words_result in result_json["words_result"]:
                f.write(str(words_result["location"]["width"]+words_result["location"]["left"]))
                f.write(",")
                f.write(str(words_result["location"]["top"]))
                f.write(",")
                f.write(str(words_result["location"]["height"]))
                f.write("\n")
            f.close()
            #得到含有答案的图片
            image=Image.open(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict)
            draw=ImageDraw.Draw(image)
            font=ImageFont.truetype(r'C:\Users\123\Downloads\font.ttf',200)
            fillcolor="#000000"
            width,height=image.size
            for words_result in  result_json["words_result"]:
                if calculator_mode1(words_result["words"])!=None:
                    draw.text((words_result["location"]["width"]+words_result["location"]["left"],words_result["location"]["top"]),str(calculator_mode1(words_result["words"])),fill=fillcolor,font=font)
            image.save(r'E:\OCR\OCR_Test\output\result_img.jpg')
            newImage=Image.new('RGB',image.size,(255,255,255))
            newDraw=ImageDraw.Draw(newImage)
            for words_result in  result_json["words_result"]:
                if calculator_mode1(words_result["words"])!=None:
                    newDraw.text((words_result["location"]["width"]+words_result["location"]["left"],words_result["location"]["top"]),str(calculator_mode1(words_result["words"])),fill=fillcolor,font=font)
            newImage.save(r'E:\OCR\OCR_Test\output\result_img_blank.jpg')
               
        elif mode==2:
            #将识别结果保存在文本中(批改模式)
            #保存答案
            f=open(r"E:\OCR\OCR_Test\output\output.txt","w")
            for words_result in result_json["words_result"]:
                f.write(words_result["words"])
                if calculator_mode2(words_result["words"])!=None:
                    f.write(calculator_mode2(words_result["words"]))
                f.write("\n")
            f.close()
            #保存答案位置
            f=open(r"E:\OCR\OCR_Test\output\location.txt","w")
            f.write("x,y,height:\n")
            for words_result in result_json["words_result"]:
                f.write(str(words_result["location"]["width"]+words_result["location"]["left"]))
                f.write(",")
                f.write(str(words_result["location"]["top"]))
                f.write(",")
                f.write(str(words_result["location"]["height"]))
                f.write("\n")
            f.close()
            #得到含有答案的图片
            image=Image.open(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict)
            draw=ImageDraw.Draw(image)
            font=ImageFont.truetype(r'C:\Windows\Fonts\Arial.ttf',300)
            fillcolor="#000000"
            width,height=image.size
            for words_result in  result_json["words_result"]:
                if calculator_mode2(words_result["words"])!=None:
                    draw.text((words_result["location"]["width"]+words_result["location"]["left"],words_result["location"]["top"]),str(calculator_mode2(words_result["words"])),fill=fillcolor,font=font)
            image.save(r'E:\OCR\OCR_Test\output\result_img.jpg')
            newImage=Image.new('RGB',image.size,(255,255,255))
            newDraw=ImageDraw.Draw(newImage)
            for words_result in  result_json["words_result"]:
                if calculator_mode2(words_result["words"])!=None:
                    newDraw.text((words_result["location"]["width"]+words_result["location"]["left"],words_result["location"]["top"]),str(calculator_mode2(words_result["words"])),fill=fillcolor,font=font)
            newImage.save(r'E:\OCR\OCR_Test\output\result_img_blank.jpg')
           
    except:
        winError=Toplevel(root)
        winError.geometry('500x500')
        winError.title('识别失败！')
        
        lb4=Message(winError,text='图像识别失败！\n请重新拍摄！',fg='black',font=('华文新魏',24),width=500,relief=FLAT)
        lb4.place(relx=0.2,y=80,relheight=0.4,width=300)
        
        but4=Button(winError,text='关闭',command=winError.destroy)
        but4.place(relx=0.35,rely=0.8,relheight=0.1,relwidth=0.2)

    finally:
        winNew.destroy()
        tag=TRUE

#旋转图片功能
def run_rotate():
    global winNew
    global mode
    global pict
    winNew.destroy()
    load=Image.open(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict)
    load=load.rotate(90,expand=True)
    load.save(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict)
    if mode==1:
        run_mode1()
    elif mode==2:
        run_mode2()

def run_destroy():
    global tag
    global winNew
    tag=TRUE
    winNew.destroy()
    os.remove(r'E:\OCR\OCR_Test\pictures\%s.jpg'%pict)

if __name__ == '__main__':
    #图像主界面
    root = Tk()
    root.title('开始界面')
    root.geometry('500x500')

    lb=Label(root,text='注意事项',fg='black',font=('华文新魏',32),width=20,height=2,relief=FLAT)
    lb.pack()

    mes=Message(root,text='1、拍照前确保当前设\n备相册中无照片\n2、拍摄照片后再选择\n模式\n3、尽量使文字水平,\n光线良好',fg='black',font=('华文新魏',24),width=500,relief=FLAT)
    mes.place(relx=0.2,y=80,relheight=0.4,width=300)

    but_mode1=Button(root,text='做题模式',command=run_mode1)
    but_mode1.place(relx=0.2,rely=0.8,relheight=0.1,relwidth=0.2)
    
    but_mode2=Button(root,text='批改模式',command=run_mode2)
    but_mode2.place(relx=0.6,rely=0.8,relheight=0.1,relwidth=0.2)

    root.mainloop()