import requests
import re
from requests.exceptions import RequestException
import json
from multiprocessing import Pool
import pymongo

client = pymongo.MongoClient('localhost')
db = client['maoyan']

def get_html(url):
    headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 \
    (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36'}
    try:
        response = requests.get(url, headers=headers)
        response.raise_for_status()
        return response.text
    except RequestException:
        print('Failed to connect')

def parse_html(html):
    pattern = re.compile('<dd>.*?board-index.*?>(\d+)</i>.*?title="(.*?)".*?data-src="(.*?)".*?star">'
    +'(.*?)</p>.*?releasetime">(.*?)</p>.*?integer">(.*?)</i>.*?fraction">(.*?)</i></p>', re.S)
    items = re.findall(pattern, html)
    for item in items:
        yield {
            'index': item[0],
            'title': item[1],
            'image': item[2],
            'actor': item[3].strip()[3:],
            'time': item[4].strip()[5:],
            'score': item[5]+item[6]
        }

def save_to_file(content):
    with open('movie100.txt', 'a', encoding='utf-8') as f:
        f.write(json.dumps(content, ensure_ascii=False) + '\n')
        f.close()

def main(page):
    url = 'http://maoyan.com/board/4?offset=' + str(page)
    html = get_html(url)
    items = parse_html(html)
    n = 1
    for item in items:
        #save_to_file(item)
        if db["maoyan"].insert(item):
            print("第 %d 条保存到数据库成功" % n)
            n += 1
        

if __name__ == '__main__':
    #pool = Pool()
    #pool.map(main, [i*10 for i in range(10)])
    for i in range(10):
        main(i * 10)
    
    