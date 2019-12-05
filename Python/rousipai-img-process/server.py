import os
from flask import Flask, render_template, request
from werkzeug import secure_filename

app = Flask(__name__)


@app.route('/')
def hello_world():
    return render_template("index.html", src_name='tic.gif', dst_name='tic.gif')


@app.route('/upload', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        f = request.files['file']
        print(f.filename)
        basepath = os.path.dirname(__file__)  # 当前文件所在路径
        upload_path = os.path.join(basepath, 'static/images', secure_filename(f.filename))  # 注意：没有的文件夹一定要先创建，不然会提示没有该路径
        f.save(upload_path)
        global FILENAME
        FILENAME = f.filename
        return render_template("upload.html", src_name=FILENAME)
    else:
        return "No results!"


@app.route('/process', methods=['GET', 'POST'])
def img_process():
    global FILENAME
    if request.method == 'POST':
        return render_template("process.html", src_name=FILENAME, dst_name=FILENAME)
    else:
        return "No results!"


@app.route('/logo', methods=['GET', 'POST'])
def img_logo():
    global FILENAME
    if request.method == 'POST':
        return render_template("logo.html", src_name=FILENAME, dst_name=FILENAME)
    else:
        return "No results!"


@app.route('/color', methods=['GET', 'POST'])
def img_color():
    global FILENAME
    if request.method == 'POST':
        return render_template("color.html", src_name=FILENAME, dst_name=FILENAME)
    else:
        return "No results!"


if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5000, debug=True)
