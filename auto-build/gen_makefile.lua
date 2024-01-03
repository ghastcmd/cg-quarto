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

local function generateFormatPrint(in_fmtFuncNAme)
    puts('define ' .. in_fmtFuncNAme .. '\n')
    puts('\t$(SS)echo $(1)\n')
    puts('endef\n\n')
end

local function generateConditional(inVar)
    local fmt_string = string.format('ifeq ($(%s),%s)\n',
                                    inVar.variable_name, inVar.value)
    puts(fmt_string)

    if inVar.second ~= nil then
        puts('else\n')
    end

    puts('endif\n')
end

local funcEnum = {
    FormatFunc = 1,
    FormatConditional = 2,
}

local translateTable = {
    [funcEnum.FormatFunc] = generateFormatPrint,
    [funcEnum.FormatConditional] = generateConditional,
}

local function dispatchGenFunc(genType, genInfo)
    translateTable[genType](genInfo)
end

local function generateConfigs(inConfigs)
    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'fmt')
    g_allFuncs:insertFunc(funcEnum.FormatConditional, {
        variable_name = 'OS',
        value = 'Windows_NT',
    })
end

function GenerateAll(inConfigs)
    generateConfigs(inConfigs)

    local allFuncs = g_allFuncs:getAll()

    for index, value in pairs(allFuncs) do
        dispatchGenFunc(value.genType, value.genInfo)
    end
end