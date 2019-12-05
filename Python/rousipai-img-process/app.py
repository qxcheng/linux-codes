#!/usr/bin/env python3
# -*- coding: utf-8 -*-
__author__ = "hongtao.zhang"

from flask import Flask, render_template
from apps import register_blueprints
from flask_bootstrap import Bootstrap

flask_app = Flask(__name__)
bootstrap = Bootstrap(flask_app)


@flask_app.route('/')
def hello_world():
    return render_template("index.html", src_name='tic.gif', dst_name='tic.gif')


def init_app(app):
    register_blueprints(app)


init_app(flask_app)

if __name__ == "__main__":
    flask_app.run(host='0.0.0.0', port=5000, debug=True)
