#!/bin/sh
bin/glacier csv/Points-Controle_cam1.csv img/2009072712_cam01.png res/out1.png
bin/glacier csv/Points-Controle_cam2.csv img/2009070508_cam02.png res/out2.png
bin/glacier csv/Points-Controle_cam3.csv img/2009070408_cam03.png res/out3.png
bin/glacier csv/Points-Controle_cam4.csv img/2009070412_cam04.png res/out4.png
bin/glacier csv/Points-Controle_cam5.csv img/2009070416_cam05.png res/out5.png
bin/glacier csv/Points-Controle_cam10.csv img/2009070316_cam10.png res/out10.png

gimp -i -b '(python-despeckle RUN-NONINTERACTIVE "res/out1.png" "filtered/out1.png")' -b '(gimp-quit 0)'
gimp -i -b '(python-despeckle RUN-NONINTERACTIVE "res/out2.png" "filtered/out2.png")' -b '(gimp-quit 0)'
gimp -i -b '(python-despeckle RUN-NONINTERACTIVE "res/out3.png" "filtered/out3.png")' -b '(gimp-quit 0)'
gimp -i -b '(python-despeckle RUN-NONINTERACTIVE "res/out4.png" "filtered/out4.png")' -b '(gimp-quit 0)'
gimp -i -b '(python-despeckle RUN-NONINTERACTIVE "res/out5.png" "filtered/out5.png")' -b '(gimp-quit 0)'
gimp -i -b '(python-despeckle RUN-NONINTERACTIVE "res/out10.png" "filtered/out10.png")' -b '(gimp-quit 0)'

scripts/magic.py