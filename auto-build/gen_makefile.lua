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
    print(in_string)
    -- g_outputString = g_outputString .. in_string
end

local function generateFormatPrint(in_fmtFuncNAme)
    puts('define ' .. in_fmtFuncNAme)
    puts('\t$(SS)echo $(1)')
    puts('endef\n')
end 

local funcEnum = {
    FormatFunc = 1,
}

local translateTable = {
    [funcEnum.FormatFunc] = generateFormatPrint,
}

local function dispatchGenFunc(genType, genInfo)
    translateTable[genType](genInfo)
end

local function generateConfigs(inConfigs)
    g_allFuncs:insertFunc(funcEnum.FormatFunc, 'fmt')
end

function GenerateAll(inConfigs)
    generateConfigs(inConfigs)

    local allFuncs = g_allFuncs:getAll()

    for index, value in pairs(allFuncs) do
        dispatchGenFunc(value.genType, value.genInfo)
    end
end