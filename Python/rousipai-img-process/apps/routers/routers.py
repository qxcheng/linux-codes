#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = "hongtao.zhang"

import os
from flask import request, render_template, redirect, url_for, make_response
from flask_restful import Resource
from apps.common import baseinfo
from image_functions import add_logo
from image_functions import fit_in
from image_functions import analyze_pic

"""接口类"""


class AddLogo(Resource, baseinfo.BaseInfo):
    """添加LOGO"""

    def __init__(self):
        baseinfo.BaseInfo.__init__(self)
        save_file_path_logo, filename_logo, upload_file_name_logo = self.upload_file_use(
            request.files[
                'logofile'])
        self.file_path_logo = save_file_path_logo

    def post(self):
        result_get = add_logo.add_logo(self.file_path_src, self.file_path_logo)
        if result_get:
            save_path_logo = os.path.join(r'.\static\images', self.upload_file_name_src+'-logo.png')
            result_get.save(save_path_logo)
        return make_response(
            render_template("logo.html", src_name=self.filename_src, dst_name=self.upload_file_name_src + '-logo.png'))

    def get(self):
        result_get = add_logo.add_logo(self.file_path_src, self.file_path_logo)
        if result_get:
            save_path_logo = os.path.join(r'.\static\images', 'result.png')
            result_get.save(save_path_logo)
        return make_response(render_template("logo.html", src_name=self.filename_src, dst_name='result.png'))


class GetColor(Resource, baseinfo.BaseInfo):
    """主色分析"""
    def __init__(self):
        baseinfo.BaseInfo.__init__(self)
        
    def post(self):
        print(self.file_path_src, 'self.file_path_src')
        result_get = analyze_pic.analyze_pic(self.file_path_src)
        if result_get:
            save_path_color = os.path.join(r'.\static\images', self.upload_file_name_src+'-color.png')
            result_get.save(save_path_color)
        return make_response(render_template("color.html", src_name=self.filename_src,
                                             dst_name=self.upload_file_name_src + '-color.png'))

    def get(self):
        result_get = analyze_pic.analyze_pic(self.file_path_src)
        if result_get:
            save_path_color = os.path.join(r'.\static\images', self.upload_file_name_src+'-color.png')
            result_get.save(save_path_color)
        return make_response(render_template("color.html", src_name=self.filename_src,
                                             dst_name=self.upload_file_name_src + '-color.png'))


class ResizeImage(Resource, baseinfo.BaseInfo):
    """获取主体并缩放图片"""

    def __init__(self):
        baseinfo.BaseInfo.__init__(self)
        self.width_obj = int(self.width_obj)
        self.height_obj = int(self.height_obj)
        self.min_margin = int(self.min_margin)

    def post(self):
        result_get = fit_in.fit_in(self.file_path_src, self.min_margin, (self.width_obj, self.height_obj))
        if result_get:
            save_path_resize = os.path.join(r'.\static\images', self.upload_file_name_src + '-resize.png')
            result_get.save(save_path_resize)
        return make_response(render_template("process.html", src_name=self.filename_src,
                                             dst_name=self.upload_file_name_src + '-resize.png'))

    def get(self):
        result_get = fit_in.fit_in(self.file_path_src, self.min_margin, (self.width_obj, self.height_obj))
        if result_get:
            save_path_resize = os.path.join(r'.\static\images', self.upload_file_name_src + '-resize.png')
            result_get.save(save_path_resize)
        return make_response(render_template("process.html", src_name=self.filename_src,
                                             dst_name=self.upload_file_name_src + '-resize.png'))
