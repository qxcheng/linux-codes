#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = "hongtao.zhang"

from flask import Blueprint
from flask_restful import Api
from apps.routers.routers import AddLogo, GetColor, ResizeImage


def register_views(app):
    api = Api(app)
    api.add_resource(AddLogo, '/logo', endpoint="logo")
    api.add_resource(ResizeImage, '/process', endpoint='process')
    api.add_resource(GetColor, '/color', endpoint='color')


def create_buleprint_v1():
    bp_v1 = Blueprint('v1', __name__)
    register_views(bp_v1)
    return bp_v1
