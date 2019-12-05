import tkinter as tk
import time
import threading
from selenium import webdriver


class Application(tk.Tk):
    def __init__(self):
        super().__init__()
        self.firstSearch = True
        self.createUI()

        self.searchAll = [
            'http://www.baidu.com/s?wd=%s',
            'https://www.sogou.com/web?query=%s'
        ]

    def createUI(self):
        self.entry = tk.Entry(self)
        self.entry.pack()
        self.button = tk.Button(self, text='SearchAll', command=lambda :self.thread_it(self.search))
        self.button.pack(expand=True, side=tk.RIGHT) 
        
    def search(self):
        searchKey = self.entry.get()
        if self.firstSearch:
            self.firstSearch = False
            self.browser = webdriver.Chrome()
            self.browser.get('http://www.baidu.com/s?wd=%s' % searchKey)
            js = 'window.open("https://www.sogou.com/web?query=%s");' % searchKey
            self.browser.execute_script(js)
        else:
            i = 0
            for handle in self.browser.window_handles:
                self.browser.switch_to_window(handle)
                self.browser.get(self.searchAll[i] % searchKey)
                i += 1

    @staticmethod
    def thread_it(func, *args):
        t = threading.Thread(target=func, args=args) 
        t.setDaemon(True)   
        t.start()           
        #t.join()          
               
app = Application()
app.mainloop()