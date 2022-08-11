from serverapp.auth import login_required
from serverapp.db import Producer

from flask import abort, flash, Blueprint, render_template, render_template_string, request, redirect, url_for
import peewee

bp = Blueprint('producer', __name__, url_prefix='/producers')

@bp.route("/")
@login_required
def list_producers():
    producers = Producer.select()
    return render_template("list_producers.html", producers=producers)


@bp.route("/add", methods=('GET', 'POST'))
@login_required
def create_producer():
    error = None
    if request.method == 'POST':
        name = request.form['name']
        description = request.form['description']
        key =  request.form['key']

        p = Producer(name=name, description=description, key=key)
        try:
            p.save()
            return redirect(url_for('producer.list_producers'))
        except peewee.IntegrityError:
            flash("Producer name and key must be unique")

    return render_template('add_producer.html')
