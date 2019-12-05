import requests
from pyquery import PyQuery as pq
import os

def get_html_text(url):
    try:
        response = requests.get(url, timeout=30)
        response.raise_for_status()
        response.encoding = response.apparent_encoding
        return response.text
    except:
        print('Fail to get the html text!')
        return None

def parse_html(html):
    doc = pq(html)
    items = doc(".d_post_content_main .p_content").items()    
    for item in items:
        r1 = item.find('.d_post_content').text()
        save_to_file('D://tieba','novels.md',r1+'\n')
        r2 = item.find('.BDE_Image')
        if r2:
            r2 = r2.attr('src')
            pic = '![](' + r2 + ')'
            save_to_file('D://tieba','novels.md',pic+'\n\n')
        
def save_to_file(filepath, filename, content):
    if not os.path.exists(filepath):
        os.mkdir(filepath)
    os.chdir(filepath)
    with open(filename, 'a+') as f:
        f.write(content)
        f.close() 

def main():
    for num in range(1,3):
        url = 'https://tieba.baidu.com/p/5379723568?see_lz=1&pn=' + str(num)
        html = get_html_text(url)
        parse_html(html)

if __name__ == '__main__':
    main()