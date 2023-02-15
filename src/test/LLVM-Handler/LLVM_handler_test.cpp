#include <gtest/gtest.h>
#include "llvm/IR/IRBuilder.h"
#include "../../main/include/LLVM-Handler/LLVMHandler.h"
#include "../../main/include/JSON-Handler/JSONHandler.h"
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/ScalarEvolution.h>
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IRReader/IRReader.h"


TEST(LLVM_HANDLER_TEST, BASICBLOCKSUMTEST){
    auto context = llvm::LLVMContext();
    auto *m = new llvm::Module("Testmodule", context);
    llvm::Function * testfunction = llvm::Function::Create( llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false), llvm::Function::ExternalLinkage, "main", m );
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "Testblock", testfunction);
    auto builder = llvm::IRBuilder(context);
    builder.SetInsertPoint(bb);

    builder.CreateLoad(llvm::Type::getInt32Ty(context), nullptr, "b");
    builder.CreateBr(bb);

    std::filesystem::path cwd = std::filesystem::current_path();
    std::string path = cwd.parent_path().parent_path().parent_path().string() + "/src/test/static/test.json";
    Json::Value energy = JSONHandler::read(path);

    auto *lh = new LLVMHandler(energy, 10000);

    double calced = lh->getBasicBlockSum(*bb, bb->getParent());

    EXPECT_EQ(calced, 160481.14999999999);
}

