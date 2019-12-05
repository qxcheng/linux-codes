import requests
from requests.exceptions import RequestException
from bs4 import BeautifulSoup
import re
import json
import os

#获取网页内容
def get_html_text(url):
    try:
        response = requests.get(url, timeout=30)
        response.raise_for_status()
        response.encoding = response.apparent_encoding
        return response.text
    except RequestException:
        print('Fail to get the html text!')
        return None

#获取图片内容
def get_html_content(url):
    try:
        response = requests.get(url, timeout=30)
        response.raise_for_status()
        return response.content
    except:
        print('Fail to get the html content!')
        return None

#解析网页获取下一层高清图片url所需的pinid
def parse_html(html):
    pattern = re.compile('app.page\[\"board\"\] = (.*?)\;\napp._csr', re.S)
    result = re.search(pattern, html)
    if result:
        data = json.loads(result.group(1))
        if data and "pins" in data:
            pinid = [item.get("pin_id") for item in data.get("pins")]
            return pinid

#通过ajax请求模拟下拉获取所有的pinid
def get_all_pinid(url):
    fst_pinid = parse_html(get_html_text(url))
    all_pinid = []
    all_pinid.extend(fst_pinid)
    last_pinid = str(fst_pinid[-1])
    while(True):
        try:
            url = url + '?jb4z1odf&max=' + last_pinid + '&limit=20&wfl=1'
            next_pinid = parse_html(get_html_text(url))
            all_pinid.extend(next_pinid)
            last_pinid = str(next_pinid[-1])
        except:
            print("No more detail pages!")
            break
    return all_pinid

#解析网页获取图片地址所需参数keys
def parse_html_pic(url):
    html = get_html_text(url)
    pattern = re.compile('app\[\"page\"\] = (.*?)\;\napp\[\"timestamp\"\]', re.S)
    image_dict = re.search(pattern, html)
    if image_dict:
        data = json.loads(image_dict.group(1))
        if data and "pin" in data.keys():
            pins = data.get("pin")
            if pins and "file" in pins:
                files = pins.get("file")
                if files and "key" in files:
                    keys = files.get("key")
                    return keys

#保存图片
def save_to_file(filepath, filename, content):
    if not os.path.exists(filepath):
        os.mkdir(filepath)
    os.chdir(filepath)
    with open(filename, 'wb') as f:
        f.write(content)
        f.close()

#主函数
#可自行修改的参数：
#board    :爬取的起始页
#filepath :保存路径
def main():   
    board = 17762415  
    url = 'http://www.huaban.com/boards/' + str(board)
    all_pinid = get_all_pinid(url)
    n = 1
    for item in all_pinid:
        image_html = 'http://www.huaban.com/pins/'+str(item)+'/'
        keys = parse_html_pic(image_html)
        image_url = 'http://img.hb.aicdn.com/' + keys + '_fw658'
        content = get_html_content(image_url)
        filepath = "D://我的文档//图片//" + str(board)
        filename = keys[1:10] +".jfif"
        save_to_file(filepath, filename, content)
        print("正在下载第 %d 张图片..." % n)
        n += 1
       
if __name__ == '__main__':
    main()