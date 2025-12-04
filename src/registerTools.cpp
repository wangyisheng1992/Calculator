#include "ToolRegistry.h"
#include "tools/CalculatorTool.h"
#include "tools/ColorPickerTool.h"
#include "tools/TextEncryptTool.h"
#include "tools/TextStatsTool.h"
#include "tools/UnitConverterTool.h"

__attribute__((constructor))
static void register_all_tools() {
    auto& r = ToolRegistry::instance();

    r.registerTool(std::make_unique<CalculatorTool>());
    r.registerTool(std::make_unique<ColorPickerTool>());
    r.registerTool(std::make_unique<TextEncryptTool>());
    r.registerTool(std::make_unique<TextStatsTool>());
    r.registerTool(std::make_unique<UnitConverterTool>());
}

