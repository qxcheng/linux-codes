#!/usr/bin/python
# -*- coding: utf-8 -*-
from tkinter import *
import tkinter.messagebox as tm
import tkinter.filedialog as tf
import os
import os.path

root = Tk()
root.title('Textbook')

#顶部菜单
menubar = Menu(root)
root.config(menu=menubar)

#菜单文件栏
def  new():
    root.title('未命名文件')
    filename = None
    textpad.delete(1.0, END)
def openfile():
    global filename
    filename = tf.askopenfilename(defaultextension='.txt')
    if filename == '':
        filename = None
    else:
        root.title('Filename:'+os.path.basename(filename))
        textpad.delete(1.0,END)
        f = open(filename, 'r')
        textpad.insert(1.0, f.read())
        f.close()
def savefile():
    global filename
    try :
        f =open(filename, 'w')
        contents = textpad.get(1.0, END)
        f.write(contents)
        f.close()
    except:
        saveas()
def saveas():
    name = tf.asksaveasfilename(initialfile='未命名.txt', defaultextension='.txt')
    global filename
    filename = name
    f = open(name, 'w')
    contents = textpad.get(1.0,END)
    f.write(contents)
    f.close()
    root.title('Filename:'+os.path.basename(name))

filemenu = Menu(menubar, tearoff=False)
filemenu.add_command(label='新建', command=new)
filemenu.add_command(label='打开', command=openfile)
filemenu.add_command(label='保存', command=savefile)
filemenu.add_command(label='另存为', command=saveas)
menubar.add_cascade(label='文件', menu=filemenu)

#菜单编辑栏
def cut():
    textpad.event_generate('<<Cut>>')
def copy():
    textpad.event_generate('<<Copy>>')
def paste():
    textpad.event_generate('<<Paste>>')
def redo():
    textpad.event_generate('<<Redo>>')
def undo():
    textpad.event_generate('<<Undo>>')
def selectAll():
    textpad.tag_add('sel', '1.0', END)
def search():
    toplevel = Toplevel(root)
    toplevel.geometry('300x30+200+250')
    Label(toplevel, text='查找').grid(row=0, column=0)
    Entry(toplevel).grid(row=0, column=1)
    Button(toplevel, text='开始').grid(row=0, column=2)
editmenu = Menu(menubar, tearoff=False)
editmenu.add_command(label='撤销', accelerator='ctrl + z',command=undo)
editmenu.add_command(label='重做', accelerator='ctrl + y',command=redo)
editmenu.add_command(label='剪切', accelerator='ctrl + x',command=cut)
editmenu.add_command(label='复制', accelerator='ctrl + c',command=copy)
editmenu.add_command(label='粘贴', accelerator='ctrl + v',command=paste)
editmenu.add_command(label='查找', accelerator='ctrl + F',command=search)
editmenu.add_command(label='全选', accelerator='ctrl + A',command=selectAll)
menubar.add_cascade(label='编辑', menu=editmenu)

#菜单关于栏
def showauthor():
    tm.showinfo('作者信息', '本软件由QXC完成')
def showcopyright():
    tm.showinfo('版权信息', '版权属于QXC')

aboutmenu = Menu(menubar,tearoff=False)
aboutmenu.add_command(label='作者', command=showauthor)
aboutmenu.add_command(label='版权', command=showcopyright)
menubar.add_cascade(label='关于', menu=aboutmenu)

#快捷键菜单栏
toolbar = Frame(root, height=10, bg='Grey')
toolbar.pack(expand=NO, fill=X)
Button(toolbar, text='新建', command=new).pack(side=LEFT, padx=5, pady=5)
Button(toolbar, text='打开', command=openfile).pack(side=LEFT, padx=5, pady=5)
Button(toolbar, text='撤销', command=undo).pack(side=RIGHT, padx=5, pady=5)
Button(toolbar, text='保存', command=savefile).pack(side=RIGHT, padx=5, pady=5)

textpad = Text(root, undo=True)
textpad.pack(expand=YES, fill=BOTH)

mainloop()


