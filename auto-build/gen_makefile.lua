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

local g_ident = 0

local function updateIdent(value)
    g_ident = g_ident + value
end

local function getIdent()
    return string.rep(' ', g_ident)
end

local g_outputString = ''

local function puts(in_string)
    io.write(in_string)
    -- g_outputString = g_outputString .. in_string
end

local dispatchGenFunc
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
    puts(getIdent() .. 'ifeq (')
    dispatchGenFunc(inVar.valueLeft)
    puts(',')
    dispatchGenFunc(inVar.valueRight)
    puts(')\n')

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
    puts(string.format(
        getIdent() .. '%s = ',
        inVar.variableName
    ))

    dispatchAllGenFunc(inVar.expression)

    puts('\n')
end

local function generateVariableEval(inVar)
    puts(string.format(
        getIdent() .. '%s := ',
        inVar.variableName
    ))

    dispatchAllGenFunc(inVar.expression)

    puts('\n')
end

local function generateVariableGet(inVar, ident)
    puts(string.format(
        '$(%s)',
        inVar.variableName
    ))
end

local function generateName(inVar)
    if (inVar.index or 0) > 1 then
        puts(' ')
    end
    puts(inVar.nameValue)
end

local funcEnum = {
    FormatNone          = 0,
    FormatBreak         = 1,
    FormatFunc          = 2,
    FormatConditional   = 3,
    FormatVariableEq    = 4,
    FormatVariableEval  = 5,
    FormatVariableGet   = 6,
    FormatName          = 7,
}

local translateTable = {
    [funcEnum.FormatNone]          = generateNone,
    [funcEnum.FormatBreak]         = generateBreak,
    [funcEnum.FormatFunc]          = generateFormatPrint,
    [funcEnum.FormatConditional]   = generateConditional,
    [funcEnum.FormatVariableEq]    = generateVariableEq,
    [funcEnum.FormatVariableEval]  = generateVariableEval,
    [funcEnum.FormatVariableGet]   = generateVariableGet,
    [funcEnum.FormatName]          = generateName,
}

function NotNone(inValue)
    return inValue.genType ~= funcEnum.FormatNone
end

function dispatchGenFunc(inValue)
    local genType = inValue.genType
    local genInfo = inValue.genInfo
    translateTable[genType](genInfo)
end

function dispatchAllGenFunc(inList)
    if #inList == 0 then
        dispatchGenFunc(inList)
    else
        for _, value in pairs(inList) do
            dispatchGenFunc(value)
        end
    end
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
        retFromSelect.genInfo.index = i

        table.insert(retList, retFromSelect)
    end

    return retList
end

local function DeclareVariableEq(variableName, assignmentExpression)
    return {
        genType = funcEnum.FormatVariableEq,
        genInfo = {
            variableName = variableName,
            expression = assignmentExpression
        }
    }
end

local function DeclareVariableEval(variableName, assignmentExpression)
    return {
        genType = funcEnum.FormatVariableEval,
        genInfo = {
            variableName = variableName,
            expression = assignmentExpression
        }
    }
end

local function DeclareConditional(
    valueLeft, valueRight, insideIf, insideElse
)
    return {
        genType = funcEnum.FormatConditional,
        genInfo = {
            valueLeft = valueLeft,
            valueRight = valueRight,
            inside = insideIf,
            second = insideElse,
        }
    }
end

local function DeclareVariableGet(variableName)
    return {
        genType = funcEnum.FormatVariableGet,
        genInfo = {
            variableName = variableName,
        }
    }
end

local function DeclareName(nameValue)
    return {
        genType = funcEnum.FormatName,
        genInfo = {
            nameValue = nameValue,
        }
    }
end

local function generateConfigs(inConfigs)
    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'fmt')

    g_allFuncs:insertValue(DeclareBreak())

    g_allFuncs:insertValue(
        DeclareConditional(
            DeclareVariableGet('OS'),
            DeclareName('Windows_NT'),
            DeclareList({
                DeclareVariableEq(
                    'dos',
                    DeclareName('Windows')
                ),
                DeclareVariableEval(
                    'deps_d',
                    DeclareName('bin')
                ),
                DeclareConditional(
                    DeclareName('numa'),
                    DeclareVariableGet('numa_ie'),
                    DeclareVariableEval(
                        'numa',
                        DeclareName('numa_ie')
                    ),
                    DeclareConditional(
                        DeclareVariableGet('var'),
                        DeclareVariableGet('das'),
                        DeclareNone(),
                        DeclareVariableEq(
                            'iop',
                            DeclareName('ujk')
                        )
                    )
                )
            }),
            DeclareVariableEq(
                'dos',
                DeclareName('$(shell uname -s)')
            )
        )
    )

    g_allFuncs:insertValue(DeclareBreak())

    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'debug')

    g_allFuncs:insertValue(DeclareBreak())

    g_allFuncs:insertValue(DeclareVariableEval(
        'help-test',
        DeclareList({
            DeclareName('name'),
            DeclareName('config'),
        })
))
end

function GenerateAll(inConfigs)
    generateConfigs(inConfigs)

    local allFuncs = g_allFuncs:getAll(false)

    dispatchAllGenFunc(allFuncs)
end