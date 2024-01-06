local g_allFuncs = {
    funcList = {},
    currentIndex = 1,

    insertValue = function(self, value)
        local newValue = value
        newValue.index = self.currentIndex

        self.funcList[self.currentIndex] = newValue

        self.currentIndex = self.currentIndex + 1
    end, 

    insertFunc = function(self, genType, genInfo)
        self.funcList[self.currentIndex] = {
            genType = genType,
            genInfo = genInfo,
            index = self.currentIndex,
        }

        self.currentIndex = self.currentIndex + 1
    end,

    getAll = function(self, sort)
        local inFuncList = self.funcList

        if sort or false then
            print("runned")
            local function greater(a, b)
                return tonumber(a.index) <= tonumber(b.index)
            end

            table.sort(inFuncList, greater)
        end

        return inFuncList
    end,
}

local g_outputString = ''

local function puts(in_string)
    io.write(in_string)
    -- g_outputString = g_outputString .. in_string
end

local dispatchAllGenFunc

local function generateFormatPrint(inVar)
    puts('define ' .. inVar .. '\n')
    puts('\t$(SS)echo $(1)\n')
    puts('endef\n\n')
end

local function generateConditional(inVar)
    local fmt_string = string.format('ifeq ($(%s),%s)\n',
                                    inVar.variable_name, inVar.value)
    puts(fmt_string)

    if inVar.inside ~= nil then
        dispatchAllGenFunc(inVar.inside)
    end

    if inVar.second ~= nil then
        puts('else\n')
        dispatchAllGenFunc(inVar.second)
    end

    puts('endif\n\n')
end

local function generateVariableEq(inVar)
    puts(string.format('%s = %s\n', inVar.variable_name, inVar.value))
end

local function generateVariableEval(inVar)
    puts(string.format('%s := %s\n', inVar.variable_name, inVar.value))
end

local funcEnum = {
    FormatFunc          = 1,
    FormatConditional   = 2,
    FormatVariableEq    = 3,
    FormatVariableEval  = 4,
}

local translateTable = {
    [funcEnum.FormatFunc]          = generateFormatPrint,
    [funcEnum.FormatConditional]   = generateConditional,
    [funcEnum.FormatVariableEq]    = generateVariableEq,
    [funcEnum.FormatVariableEval]  = generateVariableEval,
}

local function dispatchGenFunc(genType, genInfo)
    translateTable[genType](genInfo)
end

function dispatchAllGenFunc(inList)
    if #inList == 0 then
        dispatchGenFunc(inList.genType, inList.genInfo)
    else
        for _, value in pairs(inList) do
            dispatchGenFunc(value.genType, value.genInfo)
        end
    end
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
                )
            }),
            DeclareVariableEq(
                'dos',
                '$(shell uname -s)'
            )
        )
    )

    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'debug')
end

function GenerateAll(inConfigs)
    generateConfigs(inConfigs)

    local allFuncs = g_allFuncs:getAll(false)

    dispatchAllGenFunc(allFuncs)
end