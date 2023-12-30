require 'build_system'

local anotherString = ''

anotherString = anotherString .. string.format('%i', 123)

print(anotherString)

Workspace 'MainFile'
    Architecture 'x86_64'

    Configurations
    {
        'Debug',
        'Release',
        'Dist'
    }

buildcfg '0'

outputdir = '%{cfg.buildcfg}-%{cfg.System}-%{cfg.Architecture}'

print(Configurations)

Run()