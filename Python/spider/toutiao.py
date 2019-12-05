import requests
from bs4 import BeautifulSoup
import json 
from urllib.parse import urlencode
from requests.exceptions import RequestException
import re
import pymongo
from multiprocessing import Pool

MONGO_URL = 'localhost'
MONGO_DB = 'toutiao'
MONGO_TABLE = 'toutiao'
KEYWORD = '街拍'
client = pymongo.MongoClient(MONGO_URL)
db = client[MONGO_DB]

def get_page_index(offset, keyword):
    data = {
        'offset':offset,
        'format':'json',
        'keyword':keyword,
        'autoload':'true',
        'count':20,
        'cur_tab':3,
        'from':'gallery'
    } 
    url = 'https://www.toutiao.com/search_content/?' + urlencode(data)
    try:
        response = requests.get(url)
        response.raise_for_status()
        return response.text
    except RequestException:
        print("Connects to index_url Error!")
        return None

def parse_page_index(html):
    dict_html = json.loads(html)
    if dict_html and 'data' in dict_html.keys():
        for item in dict_html.get('data'):
            yield item.get('article_url')

def get_page_detail(url):
    try:
        response = requests.get(url)
        response.raise_for_status()
        return response.text
    except RequestException:
        print("Connects to index_url Error!")
        return None

def parse_page_detail(html):
    soup = BeautifulSoup(html, 'lxml')
    title = soup.find('title').string
    pattern = re.compile("gallery: JSON.parse\(\"(.*?)\"\),",re.S)
    result = re.search(pattern, html)  
    if result:
        data = result.group(1).replace('\\','')
        data = json.loads(data)
        if data and 'sub_images' in data.keys():
            sub_images = data.get('sub_images')
            image_url = [item.get('url') for item in sub_images]
            last_result = {
                        'title' : title,
                        'image_url' : image_url}
            save_to_mongo(last_result)
    return last_result

def save_to_mongo(result):
    if db[MONGO_TABLE].insert(result):
        print("Save to mongodb successful!")
        return True   
    return False

def main(offset):
    response = get_page_index(offset,KEYWORD)
    for url in parse_page_index(response):
        html = get_page_detail(url)
        parse_page_detail(html)
        
if __name__ == '__main__':
    groups = [x*20 for x in range(0,5)]
    for i in groups:
        main(i)