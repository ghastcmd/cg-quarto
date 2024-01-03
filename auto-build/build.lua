require 'config_gen'

Workspace 'MainFile'
    Architecture 'x86_64'

    Configurations
    {
        'Debug',
        'Release',
        'Dist'
    }

outputdir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'

Run()