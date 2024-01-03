require 'utils'
require 'gen_makefile'

local g_allVariables = {
    workspaces = {},
    currentWorkspace = {},

    getCurrentWorkspace = function(self)
        return self.workspaces[self.currentWorkspace]
    end,
}

local function getOS()
    local version = package.config:sub(1, 1)

    if version == '/' then
        version = 'Unix'
    elseif version == '\\' then
        version = 'Win32'
    end

    return tostring(version)
end

local function buildDefaultConfig()
    local retTable = {}

    retTable.buildcfg = ''
    retTable.system = getOS()
    retTable.architecture = ''

    return retTable
end

local function createWorkspace()
    local retWorkspace = {}

    retWorkspace.cfg = buildDefaultConfig()

    return retWorkspace
end

function Workspace(name)
    g_allVariables.currentWorkspace = name
    g_allVariables.workspaces[name] = createWorkspace()
end

function Architecture(name)
    g_allVariables:getCurrentWorkspace().cfg.architecture = name
end

function Configurations(random)
    local currentWorkstation = g_allVariables:getCurrentWorkspace()

    currentWorkstation.cfg.configurations = random
    currentWorkstation.cfg.currentBuildCfg = '1'

    local curBuildCfg = currentWorkstation.cfg.currentBuildCfg
    currentWorkstation.cfg.buildcfg = currentWorkstation.cfg.configurations[tonumber(curBuildCfg)]
end

local function parseString(inString)
    local currentWorkstation = g_allVariables:getCurrentWorkspace()

    local function replaceMatch(inMatch)
        local value = currentWorkstation

        for key in inMatch:gmatch('[^.]+') do
            value = value[key]
            if not value then
                break
            end
        end
        return value
    end

    local pattern = '%%{([^}]-)}'
    local retString = inString:gsub(pattern, replaceMatch)
    return retString
end

function Run()
    print('=== EXECUTING CODE ===')
    
    local parsedString = parseString(outputdir)
    PrettyPrint({string_parsing = {
        toFormatString = outputdir,
        parsedString = parsedString,
    }})
    PrettyPrint({g_allVariables = g_allVariables})
    
    print('\n#####################')
    print('### PRINTING CODE ###')
    print('#####################\n')

    GenerateAll(g_allVariables)
end