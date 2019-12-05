#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = "hongtao.zhang"

from apps.routers import create_buleprint_v1


def register_blueprints(app):
    app.register_blueprint(create_buleprint_v1())
