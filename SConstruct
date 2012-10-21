# Golden Egg Productions
# SCons build file

import os

sub_dirs = ['test']
build_dir = 'build/'

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
#programmer_mcu       = 'm164'
programmer_port      = 'usb'
programmer_arg       = "-p %s -c %s -P %s -e -U flash:w:" % (programmer_mcu, programmer_type, programmer_port)
programmer_flash_cmd = "%s %s" % (programmer, programmer_arg)

# Setup environment
env = Environment(CC=cc, CPPPATH=cpppath, CFLAGS=cflags)

# Methods
###############################################################################
# Flash - flashes the specified hex file
###############################################################################
def Flash(self, target):
    cmd = programmer_flash_cmd+target
    return cmd
env.AddMethod(Flash)

###############################################################################
# ReadFuses - read the current fuse settings
###############################################################################
def ReadFuses():
    cmd = "%s -p %s -c %s -P %s -v -q" % (programmer, programmer_mcu, programmer_type, programmer_port)
    return cmd
env.AddMethod(ReadFuses)

###############################################################################
# WriteFuses - write the specified fuse settings
###############################################################################
def WriteFuses(lfuse):
    cmd = "%s -p %s -c %s -P %s -u -v -U lfuse:w:%s:m" % (programmer, programmer_mcu, programmer_type, programmer_port, lfuse)
    return cmd
env.AddMethod(WriteFuses)

# Builders
env.Append(BUILDERS = {'Elf' : Builder(action = env['CC'] + "-o $TARGET $SOURCES")})
env.Append(BUILDERS = {'Hex' : Builder(action = "avr-objcopy -O ihex -R .eeprom $SOURCES $TARGET")})

# Sconscripts
for dir in sub_dirs:
    SConscript(os.path.join(dir, 'SConscript'), exports = ['env', 'build_dir'], variant_dir=build_dir, duplicate=0)

# Clean
Clean('.', build_dir)

# Alias'
env.AlwaysBuild(env.Alias('rdfuses' , [] , ReadFuses()))
env.AlwaysBuild(env.Alias('wrfuses' , [] , WriteFuses('0b11011110')))
