from serverapp.auth import key_required
from serverapp.db import Sensor, Sample

from flask import abort, Blueprint, current_app, g, render_template, render_template_string, request

bp = Blueprint('sensor', __name__, url_prefix='/sensors')


@bp.route('/', methods=('GET',))
def list_sensors():
    sensors = Sensor.select().order_by(Sensor.name)
    return render_template('list_sensors.html', sensors=sensors)

@bp.route('/add_sample', methods=('GET',))
@key_required
def add_sample():
    sensor_name = request.args.get('name', None)
    if not sensor_name:
        abort(400)

    int_value = request.args.get('int', None)
    if int_value:
        int_value = int(int_value)
    float_value = request.args.get('float', None)
    if float_value:
        float_value = float(float_value)
    if not int_value and not float_value:
        abort(400)

    sensor, addded = Sensor.get_or_create(producer_id=g.producer,name=sensor_name)
    if addded:
        sensor.save()
    sample = Sample(sensor_id=sensor.id,
                    int_value=int_value,
                    float_value=float_value)
    sample.save()

    return render_template_string('Sample added for sensor ' + sensor_name)


@bp.route('/<int:sensor_id>/samples', methods=('GET',))
def list_samples(sensor_id):
    sensor = Sensor.get(sensor_id)
    samples = (Sample.select()
        .where(Sample.sensor_id==sensor_id)
        .order_by(Sample.timestamp.desc()))

    return render_template('list_samples.html', sensor=sensor, samples=samples)
