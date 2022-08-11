import os

from flask import Flask, render_template

from serverapp.db import database
from serverapp import auth, producer, sensor


def create_app(test_config=None):
    # create and configure the app
    app = Flask(__name__, instance_relative_config=True)
    app.config.from_mapping(
        SECRET_KEY='dev',  # override with a random value when deploying
    )
    if test_config is None:
        # load the instance config, if it exists, when not testing
        app.config.from_pyfile('config.py', silent=True)
    else:
        # load the test config if passed in
        app.config.from_mapping(test_config)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    from . import sensor
    app.register_blueprint(auth.bp)
    app.register_blueprint(producer.bp)
    app.register_blueprint(sensor.bp)

    from . import db
    db.init_app(app)

    @app.before_request
    def before_request():
        database.connect()

    @app.after_request
    def after_request(response):
        database.close()
        return response

    @app.route("/")
    def index():
        return render_template("index.html")

    return app
