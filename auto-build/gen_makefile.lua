local g_allFuncs = {
    funcList = {},
    currentIndex = 1,
    
    insertFunc = function(self, genType, genInfo)
        self.funcList[self.currentIndex] = {
            genType = genType,
            genInfo = genInfo,
        }

        self.currentIndex = self.currentIndex + 1
    end,

    getAll = function(self)
        return self.funcList
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

local funcEnum = {
    FormatFunc        = 1,
    FormatConditional = 2,
    FormatVariableEq  = 3,
}

local translateTable = {
    [funcEnum.FormatFunc]        = generateFormatPrint,
    [funcEnum.FormatConditional] = generateConditional,
    [funcEnum.FormatVariableEq]  = generateVariableEq,
}

local function dispatchGenFunc(genType, genInfo)
    translateTable[genType](genInfo)
end

function dispatchAllGenFunc(inList)
    for _, value in pairs(inList) do
        dispatchGenFunc(value.genType, value.genInfo)
    end
end

local function generateConfigs(inConfigs)
    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'fmt')
    g_allFuncs:insertFunc(funcEnum.FormatConditional, {
        variable_name = 'OS',
        value = 'Windows_NT',
        inside = {
            [1] = {
                genType = funcEnum.FormatVariableEq,
                genInfo = {
                    variable_name = 'dos',
                    value = 'Windows'
                }
            },
        }
    })
end

function GenerateAll(inConfigs)
    generateConfigs(inConfigs)

    local allFuncs = g_allFuncs:getAll()

    dispatchAllGenFunc(allFuncs)
end