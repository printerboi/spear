
#include "../../main/include/JSON-Handler/JSONHandler.h"
#include "../../main/include/LLVM-Handler/InstructionCategory.h"
#include <gtest/gtest.h>
#include <filesystem>


TEST(JSON_HANDLER_TEST, READ_TEST){
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string path = cwd.parent_path().parent_path().parent_path().string() + "/src/test/static/test.json";
    Json::Value jvalues = JSONHandler::read(path);

    EXPECT_NE(jvalues.size(), 0);
}