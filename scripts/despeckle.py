#!/usr/bin/env python
# -*- coding: utf-8 -*-

from gimpfu import pdb, main, register, PF_STRING


def despeckle(fin, fout):
    image = pdb.gimp_file_load(fin, fin)
    drawable = pdb.gimp_image_get_active_layer(image)
    pdb.plug_in_despeckle(image, drawable, 3, 1, 7, 248)
    pdb.gimp_file_save(image, drawable, fout, fout)
    pdb.gimp_image_delete(image)

args = [(PF_STRING, 'file', 'GlobPattern', '*.*'),
        (PF_STRING, 'file', 'GlobPattern', '*.*')]
register('python_despeckle', '', '', '', '', '', '', '', args, [], despeckle)

main()
