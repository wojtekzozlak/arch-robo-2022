import click

from flask import current_app, g
from peewee import *
from playhouse.pool import SqliteDatabase

database = SqliteDatabase('server.db')

class Producer(Model):
    id = AutoField(primary_key=True)
    name = CharField(unique=True)
    description = CharField()
    key_hash = CharField()

    class Meta:
        database = database

class Sensor(Model):
    producer = ForeignKeyField(Producer, backref='sensors')

    id = AutoField(primary_key=True)
    name = CharField(unique=True)

    class Meta:
        database = database


class Sample(Model):
    sensor = ForeignKeyField(Sensor, backref='samples')

    timestamp = TimestampField()
    int_value = IntegerField(null=True)
    float_value = DoubleField(null=True)

    class Meta:
        database = database

def init_db():
    with database:
        database.create_tables([Producer, Sensor, Sample])

@click.command('init-db')
def init_db_command():
    init_db()
    click.echo('Initialized the database.')


def init_app(app):
    app.cli.add_command(init_db_command)
