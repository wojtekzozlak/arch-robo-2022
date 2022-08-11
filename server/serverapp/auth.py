import functools
import hashlib

from flask import abort, flash, Blueprint, current_app, g, request, redirect, render_template, url_for, session
from serverapp.db import Producer

bp = Blueprint('auth', __name__, url_prefix='/auth')

@bp.route('/login', methods=('GET', 'POST'))
def login():
    error = None
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        # TODO: Move to some config. Remember to override before deploying.
        if username == 'admin' and password == 'admin':
            session.clear()
            session['user_id'] = username
            return redirect('/')
        else:
            error = 'Incorrect data'

    flash(error)
    return render_template('login.html')


@bp.route('/logout', methods=('GET',))
def logout():
    session.clear()
    return redirect(url_for('index'))


def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if 'user_id' not in session:
            return redirect(url_for('auth.login'))
        return view(**kwargs)

    return wrapped_view


def key_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if 'key' not in request.args:
            # handle error
            abort(403)
        key = request.args['key']
        p = Producer.select().where(Producer.key == key).get_or_none()
        if not p:
            abort(403)
        g.producer = p.id

        return view(**kwargs)
    return wrapped_view
