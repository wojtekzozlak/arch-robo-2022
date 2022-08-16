sudo uwsgi -s /tmp/yourapplication.sock --manage-script-name --module 'serverapp:create_app()' --uid www-data --gid www-data
