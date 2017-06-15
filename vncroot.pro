# Create plugins directory
TEMPLATE = subdirs

src_qmlvncviewer.subdir = qmlvncviewer
src_qmlvncviewer.target = sub-src-qmlvncviewer

src_demo.subdir = demo
src_demo.target = sub-src-demo


SUBDIRS += src_qmlvncviewer src_demo



