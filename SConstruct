# Golden Egg Productions
# SCons build file

import os

sub_dirs = ['test']
build_dir = 'build'

# Flags
optimization = 's' # Optimization level: can be [0, 1, 2, 3, s]
cflags       = '-mcall-prologues -Wall -Wstrict-prototypes'
cpppath      = ['#common/']

# AVR
mcu     = 'atmega644a'
cc      = "avr-gcc -mmcu=%s -O%s " % (mcu, optimization)

# Avrdude
programmer           = 'avrdude'
programmer_type      = 'avrispmkII'
programmer_mcu       = 'm644'
programmer_port      = 'usb'
programmer_arg       = "-p %s -c %s -P %s -e -U flash:w:" % (programmer_mcu, programmer_type, programmer_port)
programmer_flash_cmd = "%s %s" % (programmer, programmer_arg)

# Setup environment
env = Environment(CC=cc, CPPPATH=cpppath, CFLAGS=cflags)

# Methods
def Flash(self, target):
    cmd = programmer_flash_cmd+target
    return cmd
env.AddMethod(Flash)

# Builders
env.Append(BUILDERS = {'Elf' : Builder(action = env['CC'] + "-o $TARGET $SOURCES")})
env.Append(BUILDERS = {'Hex' : Builder(action = "avr-objcopy -O ihex -R .eeprom $SOURCES $TARGET")})

for dir in sub_dirs:
    SConscript(os.path.join(dir, 'SConscript'), exports = ['env'], variant_dir=build_dir, duplicate=0)

Clean('.', build_dir)
