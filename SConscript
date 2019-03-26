
from building import *
import rtconfig

cwd     = GetCurrentDir()
src     = Glob('src/*.c')
CPPPATH = [cwd + '/inc']
LOCAL_CCFLAGS = ''

if GetDepend(['FAL_USING_SFUD_PORT']):
    src += Glob('samples/porting/fal_flash_sfud_port.c')

if rtconfig.CROSS_TOOL == 'gcc':
    LOCAL_CCFLAGS += ' -std=c99'
elif rtconfig.CROSS_TOOL == 'keil':
    LOCAL_CCFLAGS += ' --c99'

group = DefineGroup('fal', src, depend = ['PKG_USING_FAL'], CPPPATH = CPPPATH, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
