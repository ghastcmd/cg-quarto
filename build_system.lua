local g_allVariables = {
    workstations = {},
    cfg = {},
    currentWorkstation = {},
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
            retString = retString .. string.format('%s%s\n', formatting, value)
        else
            retString = retString .. '}\n'
        end
    end
    return retString
end

local function prettyPrintTable(inTable)
    return aux_prettyPrintTable('', inTable, 0)
end

function Workspace(name)
    g_allVariables.currentWorkstation = {}
    g_allVariables.workstations[name] = g_allVariables.currentWorkstation
end

function buildcfg(num)
    g_allVariables.cfg.buildcfg = num
end

function Architecture(name)
    print(name)
end

function Configurations(random)
    g_allVariables.cfg['Configurations'] = random
    print('Configurations:')
    prettyPrintList(random)
end

local function parseString(inString)
    local index = 0
    local function replaceMatch(inMatch)
        print('>>>', index, inMatch)
        index = index + 1
        print('#########')
        local value = g_allVariables
        print('1################')
        print(prettyPrintTable(value))
        print('2################')
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