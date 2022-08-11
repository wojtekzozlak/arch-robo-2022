import functools
import hashlib

from flask import abort, current_app, g, request, redirect
from serverapp.db import Producer

def key_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if 'key' not in request.args:
            # handle error
            abort(403)
        key = request.args['key']
        key_hash = hashlib.sha256(key.encode('utf-8')).hexdigest()
        p = Producer.select().where(Producer.key_hash == key_hash).get_or_none()
        if not p:
            abort(403)
        g.producer = p.id

        return view(**kwargs)
    return wrapped_view
