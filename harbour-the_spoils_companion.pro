TEMPLATE = subdirs

keepalive.file = keepalive/keepalive.pro

app.file = app.pro
app.depends = keepalive

SUBDIRS = keepalive app
