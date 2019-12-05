#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = "hongtao.zhang"

import os
from pypinyin import lazy_pinyin
from flask import request
from PIL import Image


class BaseInfo(object):
    """基础属性信息类"""

    def __init__(self):
        path = r'./static/images'
        self.file_save_path = path
        self.save_file_path_src, self.filename_src, self.upload_file_name_src = self.upload_file_use(
            request.files['file'])
        
        self.file_path_logo = None
        self.file_path_obj = request.form.get('file_path_obj')
        self.width_obj = request.form.get('width_obj')
        self.height_obj = request.form.get('height_obj')
        self.file_path_src = Image.open(self.save_file_path_src)
        self.width_red = request.form.get('width_red')
        self.height_red = request.form.get('height_red')
        self.min_margin = request.form.get('min_margin')

    @staticmethod
    def allowed_file(file_name):
        if file_name.lower().endswith(('png', 'jpg')):
            return True
        else:
            return False

    def upload_file_use(self, file):
        """上传函数"""
        try:
            from werkzeug.utils import secure_filename
            if file and self.allowed_file(file.filename):
                upload_file_name = file.filename.rsplit('.', 1)[0]
                name = file.filename.split('.')[0]
                ext = file.filename.split('.')[1]
                filename = '_'.join(lazy_pinyin(name)) + '.' + ext
                filename = secure_filename(filename).encode(
                    'utf-8', 'ignore').decode('utf-8')
                save_file_path = os.path.join(self.file_save_path, filename)
                file.save(save_file_path)
                return save_file_path, filename, upload_file_name
            else:
                return "No Fit", False, False
        except Exception as e:
            raise ValueError(e)
