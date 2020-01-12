import tkinter as tk
import time
import threading
from selenium import webdriver

search_sites = {
    'all_sites': [
        'http://www.baidu.com/s?wd=%s',
        'https://www.sogou.com/web?query=%s'
    ],

    'video_sites': [
        'https://search.bilibili.com/all?keyword=%s',
        'https://v.qq.com/x/search/?q=%s',
        'https://so.iqiyi.com/so/q_%s',
        'https://so.youku.com/search_video/q_%s',
        'https://so.mgtv.com/so?k=%s'
    ],

    'picture_sites': [

    ],

    'article_sites:': [

    ],

    'techblog_sites': [

    ],

    'paper_sites': [

    ]
}

class Application(tk.Tk):
    def __init__(self):
        super().__init__()
        self.firstSearch = True
        self.createUI()

    def createUI(self):
        self.entry = tk.Entry(self)
        self.entry.grid(row=0, column=2, columnspan=2)

        self.radio_var = tk.StringVar()
        self.radio_var.set('all_sites')
        tk.Radiobutton(self, text='综合', variable=self.radio_var, value='all_sites').grid(row=1, column=0)
        tk.Radiobutton(self, text='视频', variable=self.radio_var, value='video_sites').grid(row=1, column=1)
        tk.Radiobutton(self, text='图片', variable=self.radio_var, value='picture_sites').grid(row=1, column=2)
        tk.Radiobutton(self, text='文章', variable=self.radio_var, value='article_sites').grid(row=1, column=3)
        tk.Radiobutton(self, text='博客', variable=self.radio_var, value='techblog_sites').grid(row=1, column=4)
        tk.Radiobutton(self, text='论文', variable=self.radio_var, value='paper_sites').grid(row=1, column=5)
        
        self.button = tk.Button(self, text='SearchAll', command=lambda :self.thread_it(self.search))
        self.button.grid(row=2, column=2, columnspan=2)
        
    def search(self):
        searchKey = self.entry.get()
        searchUrls = search_sites.get(self.radio_var.get())
        if self.firstSearch:
            self.firstSearch = False
            self.browser = webdriver.Chrome()
            assert len(searchUrls) > 1
            self.browser.get(searchUrls[0] % searchKey)
            for url in searchUrls[1:]:            
                js = 'window.open("' + url % searchKey + '");'        
                self.browser.execute_script(js)
        else:
            i = 0
            for handle in self.browser.window_handles:
                self.browser.switch_to_window(handle)
                self.browser.get(searchUrls[i] % searchKey)
                i += 1

    @staticmethod
    def thread_it(func, *args):
        t = threading.Thread(target=func, args=args) 
        t.setDaemon(True)   
        t.start()           
        #t.join()          
               
app = Application()
app.mainloop()