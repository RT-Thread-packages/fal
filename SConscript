
from building import *

cwd     = GetCurrentDir()
src     = Glob('src/*.c')
CPPPATH = [cwd + '/inc']

group = DefineGroup('fal', src, depend = ['PKG_USING_FAL'], CPPPATH = CPPPATH)

Return('group')
