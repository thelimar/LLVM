#include <iostream>

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

extern "C"
{
    void traceMyInstr(char *instr);
}

struct myPass : public FunctionPass
{
    static char ID;

    myPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &func) override {
        if (func.getName() == "traceMyInstr") {
            return false;
        }
        LLVMContext &context = func.getContext();
        IRBuilder<> builder(context);
        Type *returnType = Type::getVoidTy(context);

        ArrayRef<Type *> tracerInstrParamTypes = {
            builder.getInt8Ty()->getPointerTo(),
        };

        FunctionType *tracerInstrFuncTypes =
            FunctionType::get(returnType, tracerInstrParamTypes, false);
        FunctionCallee tracerInstrFunc = func.getParent()->getOrInsertFunction(
            "traceMyInstr", tracerInstrFuncTypes);

        for (auto &BB : func) {
            for (auto &inst : BB) {
                if (auto *call = dyn_cast<CallInst>(&inst)) {
                    Function *callee = call->getCalledFunction();
                    if (!callee || callee->getName() == "traceMyInstr") {
                        continue;
                    }
                }

                if (inst.mayHaveSideEffects() && !inst.isTerminator()) {
                    continue;
                }
                if (auto *phi = dyn_cast<PHINode>(&inst)) {
                    continue;
                }

                // Printing
                outs() << inst.getOpcodeName() << "\n";

                builder.SetInsertPoint(&inst);
                Value *instName =
                    builder.CreateGlobalStringPtr(inst.getOpcodeName());
                Value *args[] = {instName};
                builder.CreateCall(tracerInstrFunc, args);
            }
        }

        return true;
    }
};

char myPass::ID = 0;

// code below is from http://adriansampson.net/blog/clangpass.html
static void registerMyPass(const PassManagerBuilder &Builder,
                           llvm::legacy::PassManagerBase &PM) {
    PM.add(new myPass());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_OptimizerLast,
                   registerMyPass);