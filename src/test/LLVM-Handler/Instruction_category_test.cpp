
#include <gtest/gtest.h>
#include "llvm/IR/IRBuilder.h"
#include <llvm/IR/Instructions.h>
#include "../../main/include/LLVM-Handler/InstructionCategory.h"

TEST(LLVM_HANDLER_TEST, MEMSTOREINSTTEST){
    auto context = llvm::LLVMContext();
    auto *m = new llvm::Module("Testmodule", context);
    llvm::Function * testfunction = llvm::Function::Create( llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false), llvm::Function::ExternalLinkage, "main", m );
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "Testblock", testfunction);
    auto builder = llvm::IRBuilder(context);
    builder.SetInsertPoint(bb);

    auto* L = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 41);
    auto* A = builder.CreateAlloca (llvm::Type::getInt32Ty(context), nullptr, "a");
    auto sI = builder.CreateStore (L, A, /*isVolatile=*/false);

    InstructionCategory::Category Icat = InstructionCategory::getCategory(*sI);
    EXPECT_EQ(InstructionCategory::toString(Icat), "Memstore");
}

TEST(LLVM_HANDLER_TEST, MEMLOADINSTTEST){
    auto context = llvm::LLVMContext();
    auto *m = new llvm::Module("Testmodule", context);
    llvm::Function * testfunction = llvm::Function::Create( llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false), llvm::Function::ExternalLinkage, "main", m );
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "Testblock", testfunction);
    auto builder = llvm::IRBuilder(context);
    builder.SetInsertPoint(bb);

    auto *W = builder.CreateLoad(llvm::Type::getInt32Ty(context), nullptr, "b");

    InstructionCategory::Category Icat = InstructionCategory::getCategory(*W);
    EXPECT_EQ(InstructionCategory::toString(Icat), "Memload");
}

TEST(LLVM_HANDLER_TEST, PROGRAMFLOWINSTTEST){
    auto context = llvm::LLVMContext();
    auto *m = new llvm::Module("Testmodule", context);
    llvm::Function * testfunction = llvm::Function::Create( llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false), llvm::Function::ExternalLinkage, "main", m );
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "Testblock", testfunction);
    auto builder = llvm::IRBuilder(context);
    builder.SetInsertPoint(bb);

    auto *R = builder.CreateBr(bb);

    InstructionCategory::Category Icat = InstructionCategory::getCategory(*R);
    EXPECT_EQ(InstructionCategory::toString(Icat), "Programflow");
}

TEST(LLVM_HANDLER_TEST, OTHERINSTTEST){
    auto context = llvm::LLVMContext();
    auto *m = new llvm::Module("Testmodule", context);
    llvm::Function * testfunction = llvm::Function::Create( llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false), llvm::Function::ExternalLinkage, "main", m );
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "Testblock", testfunction);
    auto builder = llvm::IRBuilder(context);
    builder.SetInsertPoint(bb);

    llvm::Value *p1 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 12);
    llvm::Value *p2 = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 6);

    auto *D = builder.CreateAdd(p1, p2 , "add");

    InstructionCategory::Category Icat = InstructionCategory::getCategory(reinterpret_cast<llvm::Instruction &>(D));
    EXPECT_EQ(InstructionCategory::toString(Icat), "Other");
}

