import glob
import os
import subprocess
import pathlib
import shutil
import json
from collections import defaultdict

import flask
from PIL import Image
from typing import *
from flask import Flask, request, render_template
from sqlalchemy import create_engine, VARCHAR, select
from sqlalchemy import Column, Integer, String, Float, DateTime

from sqlalchemy.orm import scoped_session, sessionmaker, declarative_base

visualizer_url = "http://34.85.55.117/repo/visualizer"
static_path = pathlib.Path(__file__).resolve().parent / 'static'
repo_path = pathlib.Path(__file__).resolve().parent.parent
problems_path = repo_path / "problems"
app = Flask(__name__, static_folder=str(static_path), static_url_path='')
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = 0

# global cache
problem_details = {}

engine = create_engine('mysql+pymysql://icfpc2022:icfpc2022@{host}/icfpc2022?charset=utf8'.format(**{
    'host': os.environ.get('DB_HOST', 'localhost'),
}))


@app.after_request
def add_header(response):
    if 'Expires' in response.headers:
        del response.headers['Expires']
    response.headers['Cache-Control'] = 'no-store'
    return response


def gen_thumbnail(src_path: pathlib.Path, dst_path: pathlib.Path):
    img = Image.open(src_path)
    img.resize((100, 100)).save(dst_path)


def load_problem_details(problem_files: List[str]):
    details = {}

    for prob in problem_files:
        details[prob] = {
            "name": prob
        }

    return details


@app.route('/')
def index():
    problem_files = [os.path.relpath(x, problems_path) for x in glob.glob(str(problems_path / "*"))]
    problem_files.sort(key=lambda x: int(x[:-4]))

    global problem_details
    if len(problem_details) != len(problem_files):
        problem_details = load_problem_details(problem_files)

    for png_file in problem_files:
        png_path = problems_path / png_file
        thumb_path = static_path / "thumb" / png_file
        if not thumb_path.exists():
            gen_thumbnail(png_path, thumb_path)

    problems = [
        {
            "name": x[:-4],
        } for x in problem_files
    ]

    problems_dict = {x["name"]: x for x in problems}

    result_by_api = json.load(open('../result_by_api.json', 'r'))

    solutions_rows = engine.execute("select * from solution").all()
    solutions = defaultdict(lambda: [])
    for row in solutions_rows:
        solutions[row.problem_id].append(row)

    for k in solutions.keys():
        sl = solutions[k]
        solutions[k] = sorted(sl, key=lambda x: x.cost)

    return render_template(
        'index.jinja2',
        is_search=request.args.get("search"),
        solutions=solutions,
        problems=problems,
        result_by_api=result_by_api,
        problems_dict=problems_dict
    )


@app.route('/vis/<solution>')
def get_vis(solution: str):
    problem_id, isl = engine.execute("SELECT problem_id, isl FROM solution WHERE id=%s", (solution,)).fetchone()
    return flask.redirect(visualizer_url + f"/#{problem_id};{isl}")


@app.route('/filter')
def get_filter():
    return render_template('filter.jinja2')


@app.route('/git_status')
def git_status():
    output = ""
    try:
        output += subprocess.check_output(["git", "status"], stderr=subprocess.STDOUT).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return render_template('output.jinja2', output=output)


@app.route('/fetch_problems')
def fetch_problems():
    output = ""
    try:
        output += subprocess.check_output(["node", "main.js"], cwd=(repo_path / 'portal')).decode("utf-8").strip()
        shutil.copyfile(repo_path / "portal/problems.json", static_path / "problems.json")
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return render_template('output.jinja2', output=output)


@app.route('/git_pull')
def git_pull():
    output = ""
    try:
        output += subprocess.check_output(["git", "pull"], stderr=subprocess.STDOUT).decode(
            'utf-8').strip()
    except subprocess.CalledProcessError as e:
        output += "Error:" + str(e)
    return render_template('output.jinja2', output=output)


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, threaded=True, debug=True)
