import requests
from bs4 import BeautifulSoup
import re
import os

def get_html(url):
    
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; Win64; x64) Apple\
                             WebKit/537.36 (KHTML, like Gecko) Chrome/51.\
                             0.2704.106 Safari/537.36'}
    html = requests.get(url,headers=headers)
    return html

def get_picpage(html):
    htmltext = html.text
    soup = BeautifulSoup(htmltext,'lxml')
    piclist = soup.find_all('img')
    pattern = re.compile(r'http.+?jpg',re.S)
    picpage = []
    for item in piclist:
        picp = re.findall(pattern,str(item))
        if picp:
            picpage.append(picp[0])
    return picpage

def save_img(picpage,num):
    filename = 'D:\\图片\\'+str(num)
    os.mkdir(filename)
    os.chdir(filename)
    for item in picpage:
        pic = get_html(item).content
        file_name = str(item.split('/')[-1])
        with open(file_name,'wb') as f:
            f.write(pic)
            f.close
    
num = 157
while num>0:
    url = 'http://www.mzitu.com/share/comment-page-'+str(num)+'#comments'
    html = get_html(url)
    picpage = get_picpage(html)
    save_img(picpage,num)
    num -= 1


