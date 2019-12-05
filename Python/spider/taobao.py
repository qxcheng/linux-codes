from selenium import webdriver

from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

from selenium.common.exceptions import TimeoutException

import re
from pyquery import PyQuery as pq
import pymongo

SERVICE_ARGS = ['--load-images=false', '--disk-cache=true']
browser = webdriver.PhantomJS(service_args=SERVICE_ARGS)
browser.set_window_size(1400, 900)
wait = WebDriverWait(browser, 10)

client = pymongo.MongoClient('localhost')
db = client['taobao']

def search():
    try:
        browser.get('https://www.taobao.com')
        input = wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, "#q")))
        submit = wait.until(EC.element_to_be_clickable((By.CSS_SELECTOR, "#J_TSearchForm > div.search-button > button")))
        input.send_keys("美食")
        submit.click()
        total = wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, "#mainsrp-pager > div > div > div > div.total")))
        total = int(re.compile('(\d+)').search(total.text).group(1))
        get_products()
        return total
    except TimeoutException:
        search()

def next_page(page_num):
    try:
        input = wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, '#mainsrp-pager > div > div > div > div.form > input')))
        submit = wait.until(EC.element_to_be_clickable((By.CSS_SELECTOR, '#mainsrp-pager > div > div > div > div.form > span.btn.J_Submit')))
        input.clear()
        input.send_keys(page_num)
        submit.click()
        wait.until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, '#mainsrp-pager > div > div > div > ul > li.item.active > span'), str(page_num)))
        get_products()
    except TimeoutException:
        next_page(page_num)

def get_products():
    wait.until(EC.presence_of_element_located((By.CSS_SELECTOR, '#mainsrp-itemlist .items .item')))
    html = browser.page_source
    doc = pq(html)
    items = doc('#mainsrp-itemlist .items .item').items()
    for item in items:
        product = {
            'title' : item.find('.title').text(),
            'image' : item.find('.pic .img').attr('src'),
            'price' : item.find('.price').text(),
            'deal' : item.find('.deal-cnt').text()[:-2],
            'shop' : item.find('.shop').text(),
            'location' : item.find('.location').text()
        }
        save_to_mongo(product)

def save_to_mongo(item):
    try:
        if db['taobao'].insert(item):
            print("保存到数据库成功：\n", item)
    except:
        print("保存失败：", item)

def main():
    try:
        total = search()
        for i in range(2, total+1):
            next_page(i)
    except:
        print("请求出现错误！")
    finally:
        browser.close()

if __name__ == '__main__':
    main()
    
        