local g_allFuncs = {
    funcList = {},
    currentIndex = 1,

    assignUpdate = function(self, value)
        self.funcList[self.currentIndex] = value
        self.currentIndex = self.currentIndex + 1
    end,

    insertValue = function(self, value)
        local newValue = value
        newValue.index = self.currentIndex

        self:assignUpdate(newValue)
    end,
    
    insertFunc = function(self, genType, genInfo)
        self:assignUpdate({
            genType = genType,
            genInfo = genInfo,
            index = self.currentIndex,
        })
    end,

    getAll = function(self, sort)
        local inFuncList = self.funcList

        if sort or false then
            local function greater(a, b)
                return tonumber(a.index) <= tonumber(b.index)
            end

            table.sort(inFuncList, greater)
        end

        return inFuncList
    end,
}

local g_outputString = ''

local ident = 0

local function updateIdent(value)
    ident = ident + value
end

local function getIdent()
    return string.rep(' ', ident)
end

local function puts(in_string)
    io.write(in_string)
    -- g_outputString = g_outputString .. in_string
end

local dispatchAllGenFunc
local NotNone

local function generateBreak()
    puts('\n')
end

local function generateNone()
end

local function generateFormatPrint(inVar)
    puts('define ' .. inVar .. '\n')
    puts('\t$(SS)echo $(1)\n')
    puts('endef\n')
end

local function generateConditional(inVar)
    local fmt_string = string.format('ifeq ($(%s),%s)\n',
                                    inVar.variable_name, inVar.value)
    puts(fmt_string)

    

    if NotNone(inVar.inside) then
        updateIdent(4)
        dispatchAllGenFunc(inVar.inside)
        updateIdent(-4)
    end
    
    if NotNone(inVar.second) then
        puts(getIdent() .. 'else\n')
        updateIdent(4)
        dispatchAllGenFunc(inVar.second)
        updateIdent(-4)
    end

    puts(getIdent() .. 'endif\n')
end

local function generateVariableEq(inVar)
    puts(string.format('%s = %s\n', inVar.variable_name, inVar.value))
end

local function generateVariableEval(inVar)
    puts(string.format('%s := %s\n', inVar.variable_name, inVar.value))
end

local funcEnum = {
    FormatNone          = 0,
    FormatBreak         = 1,
    FormatFunc          = 2,
    FormatConditional   = 3,
    FormatVariableEq    = 4,
    FormatVariableEval  = 5,
}

local translateTable = {
    [funcEnum.FormatNone]          = generateNone,
    [funcEnum.FormatBreak]         = generateBreak,
    [funcEnum.FormatFunc]          = generateFormatPrint,
    [funcEnum.FormatConditional]   = generateConditional,
    [funcEnum.FormatVariableEq]    = generateVariableEq,
    [funcEnum.FormatVariableEval]  = generateVariableEval,
}

function NotNone(inValue)
    return inValue.genType ~= funcEnum.FormatNone
end

local function dispatchGenFunc(genType, genInfo)
    translateTable[genType](genInfo)
end

local function aux_dispatchAllGenFunc(inList)
    if #inList == 0 then
        puts(getIdent())
        dispatchGenFunc(inList.genType, inList.genInfo)
    else
        for _, value in pairs(inList) do
            puts(getIdent())
            dispatchGenFunc(value.genType, value.genInfo)
        end
    end
end

function dispatchAllGenFunc(inList)
    aux_dispatchAllGenFunc(inList, 0)
end


local function DeclareBreak()
    return {
        genType = funcEnum.FormatBreak,
        genInfo = {}
    }
end

local function DeclareNone()
    return {
        genType = funcEnum.FormatNone,
        genInfo = {},
    }
end

local function DeclareList(inTable)
    local retList = {}

    local num = #inTable

    for i = 1, num do
        local retFromSelect = inTable[i]

        retFromSelect.index = i

        table.insert(retList, retFromSelect)
    end

    return retList
end

local function DeclareVariableEq(variableName, assignmentExpression)
    return {
        genType = funcEnum.FormatVariableEq,
        genInfo = {
            variable_name = variableName,
            value = assignmentExpression
        }
    }
end

local function DeclareVariableEval(variableName, assignmentExpression)
    return {
        genType = funcEnum.FormatVariableEval,
        genInfo = {
            variable_name = variableName,
            value = assignmentExpression
        }
    }
end

local function DeclareConditional(variableName, valueToCompare, insideIf, insideElse)
    return {
        genType = funcEnum.FormatConditional,
        genInfo = {
            variable_name = variableName,
            value = valueToCompare,
            inside = insideIf,
            second = insideElse,
        }
    }
end

local function generateConfigs(inConfigs)
    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'fmt')

    g_allFuncs:insertValue(DeclareBreak())

    g_allFuncs:insertValue(
        DeclareConditional(
            'OS',
            'Windows_NT',
            DeclareList({
                DeclareVariableEq(
                    'dos',
                    'Windows'
                ),
                DeclareVariableEval(
                    'deps_d',
                    'bin'
                ),
                DeclareConditional(
                    'numa',
                    'numa_ie',
                    DeclareVariableEval(
                        'numa',
                        'numa_ie'
                    ),
                    DeclareConditional(
                        'var',
                        'das',
                        DeclareVariableEq('asd', 'ewq'),
                        DeclareVariableEq('iop', 'ujk')
                    )
                )
            }),
            DeclareVariableEq(
                'dos',
                '$(shell uname -s)'
            )
        )
    )

    g_allFuncs:insertValue(DeclareBreak())

    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'debug')
end

function GenerateAll(inConfigs)
    generateConfigs(inConfigs)

    print('before get all')
    local allFuncs = g_allFuncs:getAll(false)
    print('after get all')

    dispatchAllGenFunc(allFuncs)
end