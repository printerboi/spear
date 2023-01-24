//
// Created by max on 13.01.23.
//

#include "LLVMHandler.h"

LLVMHandler::LLVMHandler(std::string file, Json::Value energy) {
    //this->module = std::move(llvm::parseIRFile(file, this->error, this->context));
    this->module = llvm::parseIRFile(file, this->error, this->context).release();
    this->energyValues = energy;
}

double LLVMHandler::getBasicBlockSum( llvm::BasicBlock &BB ){
    double blocksum = 0;

    for ( auto &I : BB ) {
        InstructionCategory::Category cat = InstructionCategory::getCategory(I );
        double iValue = this->energyValues[InstructionCategory::toString(cat)].asDouble();
        blocksum += iValue;
    }

    return blocksum;
}

std::set<llvm::StringRef> LLVMHandler::paramsin(llvm::BasicBlock &BB){
    std::set<llvm::StringRef> blockSet;
    return LLVMHandler::paramsin(BB, blockSet);
}

std::set<llvm::StringRef> LLVMHandler::paramsin(llvm::BasicBlock &BB, std::set<llvm::StringRef> blockSet){
    std::set<llvm::StringRef> params = LLVMHandler::gen(BB);

    for (auto BBS : llvm::successors(&BB)) {
        if( blockSet.find(BBS->getName()) == blockSet.end() ){
            blockSet.insert(BB.getName());
            LLVMHandler::paramsin(*BBS, blockSet);
        }
    }

    return params;
}

std::set<llvm::StringRef> LLVMHandler::paramsout(llvm::BasicBlock &BB){
    std::set<llvm::StringRef> blockSet;
    return LLVMHandler::paramsout(BB, blockSet);
}

std::set<llvm::StringRef> LLVMHandler::paramsout(llvm::BasicBlock &BB, std::set<llvm::StringRef> blockSet) {
    std::set<llvm::StringRef> params = LLVMHandler::kill(BB);
    std::set<llvm::StringRef> sect;
    std::set<llvm::StringRef> intersection;

    params.merge(paramsin(BB));

    for (auto BBS : llvm::successors(&BB)) {
        if( blockSet.find(BBS->getName()) == blockSet.end() ){
            blockSet.insert(BB.getName());
            std::set<llvm::StringRef> r = paramsout(*BBS, blockSet);
            sect.merge(r);
        }
    }

    //TODO Is this exclusion of the intersection needed? Added this line because intersection remains empty if no new successor can be determined (abortion of the recursion)
    if( !sect.empty() ){
        std::set_intersection( params.begin(), params.end(), sect.begin(), sect.end(), std::inserter( intersection, intersection.begin() ) );
        return intersection;
    }else{
        return params;
    }
}


std::set<llvm::StringRef> LLVMHandler::kill(llvm::BasicBlock &BB) {
    std::set<llvm::StringRef> killedVars;

    for ( auto &I : BB ) {
        std::set<llvm::StringRef> definedVars = def(I);
        if( !definedVars.empty() ){
            killedVars.merge(definedVars);
        }
    }

    return killedVars;
}

std::set<llvm::StringRef> LLVMHandler::gen(llvm::BasicBlock &BB) {
    std::set<llvm::StringRef> generatedVars;
    std::set<llvm::StringRef> definedVars;

    for ( auto &I : BB ) {
        std::set<llvm::StringRef> referedVars = ref(I);

        for (auto sr : referedVars) {
            if ( definedVars.count( sr ) == 0 ){
                generatedVars.insert(sr);
            }
        }

        definedVars.merge( def(I) );
    }

    return generatedVars;
}


std::set<llvm::StringRef> LLVMHandler::def(llvm::Instruction &I) {
    llvm::StringRef sr = I.getName();
    std::set<llvm::StringRef> definedVars;

    if( I.getName().compare("") != 0 ){
        if( InstructionCategory::isCallInstruction(I) || InstructionCategory::isMemloadInstruction(I) || !I.mayHaveSideEffects() ) {
            definedVars.insert(sr);
        }
    }

    return definedVars;
}

std::set<llvm::StringRef> LLVMHandler::ref(llvm::Instruction &I) {
    std::set<llvm::StringRef> referedVars;

    for (int i = 0; i < I.getNumOperands(); i++) {
        llvm::StringRef sr = I.getOperand(i)->getName();
        if(sr.compare("") != 0){
            referedVars.insert(sr);
        }
    }

    return referedVars;
}

void LLVMHandler::print(  ) {
    for ( auto &F : *this->module ) {
        if( F.getName() == "main" ){
            llvm::outs() << "Function " << F.getName() << "\n";
            for ( auto &BB : F ) {
                llvm::outs() << round(getBasicBlockSum( BB )) << '\n';
                std::set<llvm::StringRef> v = LLVMHandler::paramsin(BB);
                std::set<llvm::StringRef> g = LLVMHandler::paramsout(BB);

                llvm::outs() << "{";
                for( auto s : v){
                    llvm::outs() << s << ", ";
                }
                llvm::outs() << "}|";

                llvm::outs() << "{";
                for( auto s : g){
                    llvm::outs() << s << ", ";
                }
                llvm::outs() << "}\n";
            }
        }
    }
}