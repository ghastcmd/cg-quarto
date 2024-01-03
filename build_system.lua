local g_allVariables = {
    workspaces = {},
    currentWorkspace = {},

    getCurrentWorkspace = function(self)
        return self.workspaces[self.currentWorkspace]
    end,

    getCurrentConfiguration = function(self)
        return self:getCurrentWorkstation().Configurations[1]
    end,
}

local function prettyPrintList(inList)
    for index, data in ipairs(inList) do
        io.write(index)
        print('    ', data)
    end
end

local function aux_prettyPrintTable(retString, inTable, spaces)
    local formatting = string.rep(' ', spaces)
    for key, value in pairs(inTable) do
        if type(value) ~= 'string' then
            retString = retString .. string.format('%s%s {', formatting, key)
        else
            retString = retString .. string.format('%s%s = \"', formatting, key)
        end

        if type(value) == 'table' then
            if next(value) ~= nil then
                retString = retString .. '\n'
                retString = aux_prettyPrintTable(retString, value, spaces + 4)
                retString = retString .. formatting .. '}\n'
            else
                retString = aux_prettyPrintTable(retString, value, spaces + 4)
                retString = retString .. '}\n'
            end
        elseif type(value) == 'string' then
            -- retString = retString .. '####STRING####'
            retString = retString .. string.format('%s\"\n', value)
        else
            retString = retString .. '}\n'
        end
    end
    return retString
end

local function prettyPrintTable(inTable)
    return aux_prettyPrintTable('', inTable, 0)
end

local function buildDefaultConfig()
    local retTable = {}

    retTable.buildcfg = ''
    retTable.system = 'Win32'
    retTable.architecture = ''

    return retTable
end

local function createWorkspace()
    local retWorkspace = {}
    
    retWorkspace.cfg = buildDefaultConfig()

    for key, value in pairs(retWorkspace.cfg) do
        print(key, value)
    end

    print('######### DEBUG PRINT WORKSPACE ##########')
    print(prettyPrintTable(retWorkspace))
    print('##########################################')

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

    print('Configurations:')
    prettyPrintList(random)
end

local function parseString(inString)
    local index = 0

    -- local currentWorkstation = g_allVariables.workstations[g_allVariables.currentWorkstation]
    local currentWorkstation = g_allVariables:getCurrentWorkspace()

    local function replaceMatch(inMatch)
        print('>>>', index, inMatch)
        index = index + 1
        -- print('#########')
        local value = currentWorkstation
        -- print('1################')
        -- print(prettyPrintTable(value))
        -- print('2################')
        local another_index = 0
        for key in inMatch:gmatch('[^.]+') do
            print('>>>>>> ', another_index, key)
            print(prettyPrintTable(value))
            
            value = value[key]
            if not value then
                break
            end
        end
        print(value)
        return value
    end

    local pattern = '%%{([^}]-)}'
    local retString = inString:gsub(pattern, replaceMatch)
    return retString
end

function Run()
    print('executed code')
    print(outputdir)
    local parsedString = parseString(outputdir)
    print(parsedString)
end