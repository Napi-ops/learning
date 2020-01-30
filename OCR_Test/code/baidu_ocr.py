# coding=utf-8

import sys
import json
import base64

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

OCR_URL = "https://aip.baidubce.com/rest/2.0/ocr/v1/accurate_basic"

TOKEN_URL = 'https://aip.baidubce.com/oauth/2.0/token'

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

def read_file(image_path):
    f = None
    try:
        f = open(image_path, 'rb')
        return f.read()
    except:
        print('read image file fail')
        return None
    finally:
        if f:
            f.close()

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

if __name__ == '__main__':
    token = fetch_token()
    image_url = OCR_URL + "?access_token=" + token
    for i in range(10):
        file_content = read_file(r'E:\OCR\OCR_Test\samples\%s.jpg'%i)
        result = request(image_url, urlencode({'image': base64.b64encode(file_content)}))
        result_json = json.loads(result)
        f=open(r"E:\OCR\OCR_Test\output\%s.txt"%i,"w")
        for words_result in result_json["words_result"]:
            print(words_result["words"])
            f.write(words_result["words"])
            f.write("\n")
        f.close()
