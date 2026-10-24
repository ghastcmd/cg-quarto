class AllFuncs
  attr_reader :curIndex, :funcList

  def initialize
    @funcList = []
    @curIndex = 0
  end

  protected def assignUpdate value
    @funcList[@curIndex] = value
    @curIndex += 1
  end

  def insertValue value
    newValue = value
    newValue[:index] = @curIndex

    assignUpdate(newValue)
  end

  def insertFunc genType, genInfo
    assignUpdate({
      genType: genType,
      genInfo: genInfo,
      index: @curIndex
    })
  end

  def getAll sort
    inFuncList = @funcList

    if sort then
      inFuncList.sort_by! { |v| v[:index] }
    end

    return inFuncList
  end
end

FuncEnum = {
  FormatNone:          0,
  FormatBreak:         1,
  FormatPrint:         2,
  FormatConditional:   3,
  FormatVariableEq:    4,
  FormatVariableEval:  5,
  FormatVariableGet:   6,
  FormatName:          7,
  FormatFunction:      8,
}

class MakefileGen
  def initialize
    @allFuncs = AllFuncs.new
    @ident = 0
    @outputString = ''

    @translateTable = {
      FuncEnum[:FormatNone]         => method(:generateNone),
      FuncEnum[:FormatBreak]        => method(:generateBreak),
      FuncEnum[:FormatPrint]        => method(:generateFormatPrint),
      FuncEnum[:FormatConditional]  => method(:generateConditional),
      FuncEnum[:FormatVariableEq]   => method(:generateVariableEq),
      FuncEnum[:FormatVariableEval] => method(:generateVariableEval),
      FuncEnum[:FormatVariableGet]  => method(:generateVariableGet),
      FuncEnum[:FormatName]         => method(:generateName),
      FuncEnum[:FormatFunction]     => method(:generateFunction),
    }
  end

  def generateAll inConfigs
    generateConfigs(inConfigs)

    allFuncs = @allFuncs.getAll(false)

    dispatchAllGenFunc(allFuncs)
  end

  private def updateIdent value
    @ident += value
  end

  private def getIdent
    return ' ' * @ident
  end

  private def puts inString
    if inString != nil then
      print inString
    end
    # @outputString += inString
  end

  private def generateBreak inVar
    puts("\n")
  end

  private def generateNone inVar
  end

  private def generateFormatPrint inVar
    puts("define " + inVar + "\n")
    puts("\t$(SS)echo $(1)\n")
    puts("endef\n")
  end

  private def generateConditional inVar
    puts(getIdent + "ifeq (")
    dispatchGenFunc(inVar[:valueLeft])
    puts(",")
    dispatchGenFunc(inVar[:valueRight])
    puts("\n")

    if notNone(inVar[:inside]) then
      updateIdent(4)
      dispatchAllGenFunc(inVar[:inside])
      updateIdent(-4)
    end

    if notNone(inVar[:second]) then
      puts(getIdent + "else\n")
      updateIdent(4)
      dispatchAllGenFunc(inVar[:second])
      updateIdent(-4)
    end

    puts(getIdent + "endif\n")
  end

  private def generateVariableEq inVar
    puts(getIdent + inVar[:variableName] + " = ")

    dispatchAllGenFunc(inVar[:expression])

    puts("\n")
  end

  private def generateVariableEval inVar
    puts(getIdent + inVar[:variableName] + " := ")

    dispatchAllGenFunc(inVar[:expression])

    puts("\n")
  end

  private def generateVariableGet inVar
    puts("$(" + inVar[:variableName] + ")")
  end

  private def generateName inVar
    if (inVar[:index] || 0) > 0 then
      puts(" ")
    end
    puts(inVar[:nameValue])
  end

  private def generateFunction inVar
    puts(inVar[:nameValue])
    puts(":")
    puts(inVar[:dependency])
    puts("\n")

    if inVar[:declList] != nil && notNone(inVar[:declList]) then
      updateIdent(4)
      dispatchAllGenFunc(inVar[:declList])
      updateIdent(-4)
    end

    puts("\n")
  end

  private def notNone inValue
    if inValue.is_a?(Array) then
      return true
    end

    return inValue[:genType] != FuncEnum[:FormatNone]
  end

  private def dispatchGenFunc inValue
    another_genType = inValue[:genType]
    another_genInfo = inValue[:genInfo]
    @translateTable[another_genType].call(another_genInfo)
  end

  private def dispatchAllGenFunc inList
    if inList.is_a?(Hash) then
      dispatchGenFunc(inList)
    else
      inList.each do |value|
        dispatchGenFunc(value)
      end
    end
  end

  private def declareBreak
    return {
      genType: FuncEnum[:FormatBreak],
      genInfo: {}
    }
  end

  private def declareNone
    return {
      genType: FuncEnum[:FormatNone],
      genInfo: {}
    }
  end

  private def declareList inTable
    retList = []

    inTable.each_with_index do |item, i|
      retFromSelect = item

      retFromSelect[:index] = i
      retFromSelect[:genInfo][:index] = i

      retList << retFromSelect
    end

    return retList
  end

  private def declareVariableEq variableName, assignmentExpression
    return {
      genType: FuncEnum[:FormatVariableEq],
      genInfo: {
        variableName: variableName,
        expression: assignmentExpression
      }
    }
  end

  private def declareVariableEval variableName, assignmentExpression
    return {
      genType: FuncEnum[:FormatVariableEval],
      genInfo: {
        variableName: variableName,
        expression: assignmentExpression
      }
    }
  end

  private def declareConditional valueLeft, valueRight, insideIf, insideElse
    return {
      genType: FuncEnum[:FormatConditional],
      genInfo: {
        valueLeft: valueLeft,
        valueRight: valueRight,
        inside: insideIf,
        second: insideElse
      }
    }
  end

  private def declareVariableGet variableName
    return {
      genType: FuncEnum[:FormatVariableGet],
      genInfo: {
        variableName: variableName
      }
    }
  end

  private def declareName nameValue
    return {
      genType: FuncEnum[:FormatName],
      genInfo: {
        nameValue: nameValue
      }
    }
  end

  private def declareFunction nameValue, dependency, declList
    return {
      genType: FuncEnum[:FormatFunction],
      genInfo: {
        nameValue: nameValue,
        dependency: dependency,
        declList: declList
      }
    }
  end

  private def generateConfigs inConfigs
    @allFuncs.insertFunc(FuncEnum[:FormatPrint], 'fmt')

    @allFuncs.insertValue(declareBreak())

    @allFuncs.insertValue(
      declareConditional(
        declareVariableGet('OS'),
        declareName('Windows_NT'),
        declareList([
          declareVariableEq(
            'dos',
            declareName('Windows')
          ),
          declareVariableEval(
            'deps_d',
            declareName('bin')
          ),
          declareConditional(
            declareName('numa'),
            declareVariableGet('numa_ie'),
            declareVariableEval(
              'numa',
              declareName('numa_ie')
            ),
            declareConditional(
              declareVariableGet('var'),
              declareVariableGet('das'),
              declareNone(),
              declareVariableEq(
                'iop',
                declareName('ujk')
              )
            )
          )
        ]),
        declareVariableEq(
          'dos',
          declareName('$(shell uname -s)')
        )
      )
    )

    @allFuncs.insertValue(declareBreak())

    @allFuncs.insertFunc(FuncEnum[:FormatPrint], 'debug')

    @allFuncs.insertValue(declareBreak())

    @allFuncs.insertValue(declareVariableEval(
      'help-test',
      declareList([
        declareName('name'),
        declareName('config')
      ])
    ))

    @allFuncs.insertValue(declareFunction(
      'simple_value',
      '',
      declareList([
        declareVariableEq(
          'name',
          declareName('$(shell uname -s)')
        )
      ])
    ))
  end
end


def test

  test = AllFuncs.new

  p test.curIndex
  test.insertValue({value: 10})
  test.insertValue({value: 13})
  test.insertValue({value: 9})
  test.insertValue({value: 13})

  swap = test.funcList[0]
  test.funcList[0] = test.funcList[1]
  test.funcList[1] = swap

  p test.curIndex
  p test.funcList
  test.getAll(true)
  p test.funcList


end


# test()
MakefileGen.new.generateAll(nil)
