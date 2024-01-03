local function aux_prettyFormatTable(retString, inTable, spaces)
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
                retString = aux_prettyFormatTable(retString, value, spaces + 4)
                retString = retString .. formatting .. '}\n'
            else
                retString = aux_prettyFormatTable(retString, value, spaces + 4)
                retString = retString .. '}\n'
            end
        elseif type(value) == 'string' then
            -- retString = retString .. '####STRING####'
            retString = retString .. string.format('%s\"\n', value)
        elseif type(value) == 'function' then
            retString = retString .. '()}\n'
        else
            retString = retString .. '}\n'
        end
    end
    return retString
end

local function prettyFormatTable(inTable)
    return aux_prettyFormatTable('', inTable, 0)
end

function PrettyFormat(inVariable)
    return prettyFormatTable(inVariable)
end

function PrettyPrint(inVariable)
    io.write(PrettyFormat(inVariable))
end