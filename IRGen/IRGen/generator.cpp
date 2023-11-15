#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "../sim.h"

#define FRAME_TICKS 200

static SDL_Renderer *Renderer = nullptr;
static SDL_Window *Window = nullptr;
static Uint32 Ticks = 0;

void simInit()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SIZE_X, SIZE_Y, 0, &Window, &Renderer);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
    SDL_RenderClear(Renderer);
    srand(time(NULL));
}

extern void app();

void simExit()
{
    SDL_Event event;
    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
}

void SimFlush()
{
    SDL_PumpEvents();
    assert(SDL_TRUE != SDL_HasEvent(SDL_QUIT) && "User-requested quit");
    Uint32 cur_ticks = SDL_GetTicks() - Ticks;
    if (cur_ticks < FRAME_TICKS)
    {
        SDL_Delay(FRAME_TICKS - cur_ticks);
    }
    SDL_RenderPresent(Renderer);
}

void SimPutPixel(int x, int y, int argb)
{
    assert(0 <= x && x < SIZE_X && "Out of range");
    assert(0 <= y && y < SIZE_Y && "Out of range");
    Uint8 a = argb >> 24;
    Uint8 r = (argb >> 16) & 0xFF;
    Uint8 g = (argb >> 8) & 0xFF;
    Uint8 b = argb & 0xFF;
    SDL_SetRenderDrawColor(Renderer, r, g, b, a);
    SDL_RenderDrawPoint(Renderer, x, y);
    Ticks = SDL_GetTicks();
}

int SimRand()
{
    return rand();
}

// end of sim.cpp

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/IntrinsicInst.h"

using namespace llvm;

int main() {
    LLVMContext context;
    // ; ModuleID = 'life'
    // source_filename = "life"
    Module *module = new Module("life", context);
    IRBuilder<> builder(context);
// --------------------------------------------------------------------------------
    Type* elementType = builder.getInt32Ty();
    Type* arrayType = llvm::ArrayType::get(elementType, 240000);

    GlobalVariable *current_gen = new GlobalVariable(
        *module,
        arrayType,
        false,
        GlobalValue::CommonLinkage,
        Constant::getNullValue(arrayType),
        "current_gen"
    );

    GlobalVariable *next_gen = new GlobalVariable(
        *module,
        arrayType,
        false,
        GlobalValue::CommonLinkage,
        Constant::getNullValue(arrayType),
        "next_gen"
    );
    current_gen->setAlignment(Align(16));
    next_gen->setAlignment(Align(16));

    // Declare external functions from sum.h
    FunctionType *simFlushFuncType = FunctionType::get(builder.getVoidTy(), false);
    FunctionCallee simFlushFunc = module->getOrInsertFunction("_Z8SimFlushv", simFlushFuncType);

    FunctionType *simPutPixelFuncType = FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty()}, false);
    FunctionCallee simPutPixelFunc = module->getOrInsertFunction("_Z11SimPutPixeliii", simPutPixelFuncType);

    FunctionType *simRandFuncType = FunctionType::get(builder.getInt32Ty(), false);
    FunctionCallee simRandFunc = module->getOrInsertFunction("_Z7SimRandv", simRandFuncType);

    // Define my func DrawGen from life.cpp
    FunctionType *drawGenFuncType = FunctionType::get(builder.getVoidTy(), false);
    Function *drawGenFunc = Function::Create(drawGenFuncType, Function::ExternalLinkage, "_Z7DrawGenv", module);
    drawGenFunc->setDSOLocal(true);
    drawGenFunc->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);

    // Define custom functions CalcNextGen from app.cpp
    FunctionType *calcNextGenFuncType = FunctionType::get(builder.getVoidTy(), false);
    Function *calcNextGenFunc = Function::Create(calcNextGenFuncType, Function::ExternalLinkage, "_Z11CalcNextGenv", module);
    calcNextGenFunc->setDSOLocal(true);
    calcNextGenFunc->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);

    // Define my func app from life.cpp
    FunctionType *appFuncType = FunctionType::get(builder.getVoidTy(), false);
    Function *appFunc = Function::Create(appFuncType, Function::ExternalLinkage, "_Z3appv", module);
    appFunc->setDSOLocal(true);
    appFunc->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);

    // Actual IR for DrawGen() function
    {
        // Declare all basic blocks in advance
        BasicBlock *BB0 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB3 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB6 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB7 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB10 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB19 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB22 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB23 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB26 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB27 = BasicBlock::Create(context, "", drawGenFunc);
        BasicBlock *BB30 = BasicBlock::Create(context, "", drawGenFunc);

        builder.SetInsertPoint(BB0); // 0: implicit
        AllocaInst *value1 = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "");
        AllocaInst *value2 = builder.CreateAlloca(builder.getInt32Ty());
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value1));
        builder.CreateBr(BB3); // br label %3

        //   3
        builder.SetInsertPoint(BB3);
        auto *value4 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value5 = builder.CreateICmpSLT(value4, builder.getInt32(400));
        builder.CreateCondBr(value5, BB6, BB30);

        //   6
        builder.SetInsertPoint(BB6);
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value2));
        builder.CreateBr(BB7);

        //   7
        builder.SetInsertPoint(BB7);
        auto *value8 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value9 = builder.CreateICmpSLT(value8, builder.getInt32(600));
        builder.CreateCondBr(value9, BB10, BB26);

        //   10
        builder.SetInsertPoint(BB10);
        auto *value11 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value12 = builder.CreateNSWMul(value11, builder.getInt32(600)); // mul nsw
        auto *value13 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value14 = builder.CreateNSWAdd(value12, value13);
        auto *value15 = builder.CreateSExt(value14, builder.getInt64Ty());
        auto *value16 = builder.CreateInBoundsGEP(arrayType, next_gen, {builder.getInt64(0), value15}); // error
        auto *value17 = builder.CreateLoad(builder.getInt32Ty(), value16);
        auto *value18 = builder.CreateICmpEQ(value17, builder.getInt32(1));
        builder.CreateCondBr(value18, BB19, BB22);

        //   19
        builder.SetInsertPoint(BB19);
        auto *value20 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value21 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        builder.CreateCall(simPutPixelFunc, {value20, value21, builder.getInt32(-65536)});
        builder.CreateBr(BB22);

        //   22
        builder.SetInsertPoint(BB22);
        builder.CreateBr(BB23);

        //   23
        builder.SetInsertPoint(BB23);
        auto *value24 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value25 = builder.CreateNSWAdd(value24, builder.getInt32(1));
        builder.CreateStore(value25, dyn_cast<llvm::Value>(value2));
        builder.CreateBr(BB7);

        //   26
        builder.SetInsertPoint(BB26);
        builder.CreateBr(BB27);

        //   27
        builder.SetInsertPoint(BB27);
        auto *value28 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value29 = builder.CreateNSWAdd(value28, builder.getInt32(1));
        builder.CreateStore(value29, value1);
        builder.CreateBr(BB3);

        //   30
        builder.SetInsertPoint(BB30);
        builder.CreateCall(simFlushFunc);
        builder.CreateRetVoid();
    }
    
    // Actual IR for CalcNextGen() function
    {
        // Declare all basic blocks in advance
        BasicBlock *BB0 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB8 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB11 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB12 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB15 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB16 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB19 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB20 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB23 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB26 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB29 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB30 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB39 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB42 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB45 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB48 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB58 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB59 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB62 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB63 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB66 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB75 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB78 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB81 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB88 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB95 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB96 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB99 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB106 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB113 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB114 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB115 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB118 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB119 = BasicBlock::Create(context, "", calcNextGenFunc);
        BasicBlock *BB122 = BasicBlock::Create(context, "", calcNextGenFunc);

        builder.SetInsertPoint(BB0); // 0: implicit
        AllocaInst *value1 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value2 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value3 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value4 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value5 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value6 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value7 = builder.CreateAlloca(builder.getInt32Ty());
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value1));
        builder.CreateBr(BB8);

        //  8
        builder.SetInsertPoint(BB8);
        auto *value9 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value10 = builder.CreateICmpSLT(value9, builder.getInt32(400));
        builder.CreateCondBr(value10, BB11, BB122);

        //  11
        builder.SetInsertPoint(BB11);
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value2));
        builder.CreateBr(BB12);

        //  12
        builder.SetInsertPoint(BB12);
        auto *value13 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value14 = builder.CreateICmpSLT(value13, builder.getInt32(600));
        builder.CreateCondBr(value14, BB15, BB118);

        //  15
        builder.SetInsertPoint(BB15);
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value3));
        builder.CreateStore(builder.getInt32(-1), dyn_cast<llvm::Value>(value4));
        builder.CreateBr(BB16);

        //  16
        builder.SetInsertPoint(BB16);
        auto *value17 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value18 = builder.CreateICmpSLE(value17, builder.getInt32(1));
        builder.CreateCondBr(value18, BB19, BB66);

        // 19
        builder.SetInsertPoint(BB19);
        builder.CreateStore(builder.getInt32(-1), dyn_cast<llvm::Value>(value5));
        builder.CreateBr(BB20);

        // 20
        builder.SetInsertPoint(BB20);
        auto *value21 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value5));
        auto *value22 = builder.CreateICmpSLE(value21, builder.getInt32(1));
        builder.CreateCondBr(value22, BB23, BB62);

        // 23
        builder.SetInsertPoint(BB23);
        auto *value24 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value25 = builder.CreateICmpEQ(value24, builder.getInt32(0));
        builder.CreateCondBr(value25, BB26, BB30);

        // 26
        builder.SetInsertPoint(BB26);
        auto *value27 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value5));
        auto *value28 = builder.CreateICmpEQ(value27, builder.getInt32(0));
        builder.CreateCondBr(value28, BB29, BB30);

        // 29
        builder.SetInsertPoint(BB29);
        builder.CreateBr(BB59);

        // 30
        builder.SetInsertPoint(BB30);
        auto *value31 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value32 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value33 = builder.CreateNSWAdd(value31, value32);
        builder.CreateStore(value33, dyn_cast<llvm::Value>(value6));
        auto *value34 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value35 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value5));
        auto *value36 = builder.CreateNSWAdd(value34, value35);
        builder.CreateStore(value36, dyn_cast<llvm::Value>(value7));
        auto *value37 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value6));
        auto *value38 = builder.CreateICmpSGE(value37, builder.getInt32(0));
        builder.CreateCondBr(value38, BB39, BB58);

        // 39
        builder.SetInsertPoint(BB39);
        auto *value40 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value6));
        auto *value41 = builder.CreateICmpSLT(value40, builder.getInt32(600));
        builder.CreateCondBr(value41, BB42, BB58);

        // 42
        builder.SetInsertPoint(BB42);
        auto *value43 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value7));
        auto *value44 = builder.CreateICmpSGE(value43, builder.getInt32(0));
        builder.CreateCondBr(value44, BB45, BB58);

        // 45
        builder.SetInsertPoint(BB45);
        auto *value46 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value7));
        auto *value47 = builder.CreateICmpSLT(value46, builder.getInt32(400));
        builder.CreateCondBr(value47, BB48, BB58);

        // 48
        builder.SetInsertPoint(BB48);
        auto *value49 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value7));
        auto *value50 = builder.CreateNSWMul(value49, builder.getInt32(600));
        auto *value51 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value6));
        auto *value52 = builder.CreateNSWAdd(value50, value51);
        auto *value53 = builder.CreateSExt(value52, Type::getInt64Ty(context));
        auto *value54 = builder.CreateInBoundsGEP(arrayType, current_gen, {builder.getInt64(0), value53});
        auto *value55 = builder.CreateLoad(builder.getInt32Ty(), value54);
        auto *value56 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value3));
        auto *value57 = builder.CreateNSWAdd(value56, value55);
        builder.CreateStore(value57, dyn_cast<llvm::Value>(value3));
        builder.CreateBr(BB58);

        // 58
        builder.SetInsertPoint(BB58);
        builder.CreateBr(BB59);

        // 59
        builder.SetInsertPoint(BB59);
        auto *value60 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value5));
        auto *value61 = builder.CreateNSWAdd(value60, builder.getInt32(1));
        builder.CreateStore(value61, dyn_cast<llvm::Value>(value5));
        builder.CreateBr(BB20);

        // 62
        builder.SetInsertPoint(BB62);
        builder.CreateBr(BB63);

        // 63
        builder.SetInsertPoint(BB63);
        auto *value64 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value65 = builder.CreateNSWAdd(value64, builder.getInt32(1));
        builder.CreateStore(value65, dyn_cast<llvm::Value>(value4));
        builder.CreateBr(BB16);

        // 66
        builder.SetInsertPoint(BB66);
        auto *value67 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value68 = builder.CreateNSWMul(value67, builder.getInt32(600));
        auto *value69 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value70 = builder.CreateNSWAdd(value68, value69);
        auto *value71 = builder.CreateSExt(value70, Type::getInt64Ty(context));
        auto *value72 = builder.CreateInBoundsGEP(arrayType, current_gen, {builder.getInt64(0), value71});
        auto *value73 = builder.CreateLoad(builder.getInt32Ty(), value72);
        auto *value74 = builder.CreateICmpEQ(value73, builder.getInt32(1));
        builder.CreateCondBr(value74, BB75, BB96);

        // 75
        builder.SetInsertPoint(BB75);
        auto *value76 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value3));
        auto *value77 = builder.CreateICmpSLT(value76, builder.getInt32(2));
        builder.CreateCondBr(value77, BB81, BB78);

        // 78
        builder.SetInsertPoint(BB78);
        auto *value79 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value3));
        auto *value80 = builder.CreateICmpSGT(value79, builder.getInt32(3));
        builder.CreateCondBr(value80, BB81, BB88);

        // 81
        builder.SetInsertPoint(BB81);
        auto *value82 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value83 = builder.CreateNSWMul(value82, builder.getInt32(600));
        auto *value84 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value85 = builder.CreateNSWAdd(value83, value84);
        auto *value86 = builder.CreateSExt(value85, Type::getInt64Ty(context));
        auto *value87 = builder.CreateInBoundsGEP(arrayType, next_gen, {builder.getInt64(0), value86});
        builder.CreateStore(builder.getInt32(400), value87);
        builder.CreateBr(BB95);

        // 88
        builder.SetInsertPoint(BB88);
        auto *value89 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value90 = builder.CreateNSWMul(value89, builder.getInt32(600));
        auto *value91 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value92 = builder.CreateNSWAdd(value90, value91);
        auto *value93 = builder.CreateSExt(value92, Type::getInt64Ty(context));
        auto *value94 = builder.CreateInBoundsGEP(arrayType, next_gen, {builder.getInt64(0), value93}); //maybe error
        builder.CreateStore(builder.getInt32(1), value94);
        builder.CreateBr(BB95);

        // 95
        builder.SetInsertPoint(BB95);
        builder.CreateBr(BB114);

        // 96
        builder.SetInsertPoint(BB96);
        auto *value97 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value3));
        auto *value98 = builder.CreateICmpEQ(value97, builder.getInt32(3));
        builder.CreateCondBr(value98, BB99, BB106);

        // 99
        builder.SetInsertPoint(BB99);
        auto *value100 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value101 = builder.CreateNSWMul(value100, builder.getInt32(600));
        auto *value102 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value103 = builder.CreateNSWAdd(value101, value102);
        auto *value104 = builder.CreateSExt(value103, Type::getInt64Ty(context));
        auto *value105 = builder.CreateInBoundsGEP(arrayType, next_gen, {builder.getInt64(0), value104}); //maybe error
        builder.CreateStore(builder.getInt32(1), value105);
        builder.CreateBr(BB113);

        // 106
        builder.SetInsertPoint(BB106);
        auto *value107 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value108 = builder.CreateNSWMul(value107, builder.getInt32(600));
        auto *value109 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value110 = builder.CreateNSWAdd(value108, value109);
        auto *value111 = builder.CreateSExt(value110, Type::getInt64Ty(context));
        auto *value112 = builder.CreateInBoundsGEP(arrayType, next_gen, {builder.getInt64(0), value111});
        builder.CreateStore(builder.getInt32(0), value112);
        builder.CreateBr(BB113);
        
        // 113
        builder.SetInsertPoint(BB113);
        builder.CreateBr(BB114);

        // 114
        builder.SetInsertPoint(BB114);
        builder.CreateBr(BB115);

        // 115
        builder.SetInsertPoint(BB115);
        auto *value116 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value117 = builder.CreateNSWAdd(value116, builder.getInt32(1));
        builder.CreateStore(value117, dyn_cast<llvm::Value>(value2));
        builder.CreateBr(BB12);

        // 118
        builder.SetInsertPoint(BB118);
        builder.CreateBr(BB119);

        // 119
        builder.SetInsertPoint(BB119);
        auto *value120 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value121 = builder.CreateNSWAdd(value120, builder.getInt32(1));
        builder.CreateStore(value121, dyn_cast<llvm::Value>(value1));
        builder.CreateBr(BB8);

        // 122
        builder.SetInsertPoint(BB122);
        builder.CreateRetVoid();
    }

    // Actual IR for app() function
    {
        // Declare all basic blocks in advance
        BasicBlock *BB0 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB6 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB9 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB10 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB13 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB22 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB25 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB26 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB29 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB30 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB33 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB34 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB37 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB38 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB41 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB55 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB58 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB59 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB62 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB63 = BasicBlock::Create(context, "", appFunc);
        BasicBlock *BB66 = BasicBlock::Create(context, "", appFunc);

        // Базовый блок 0
        builder.SetInsertPoint(BB0); // 0: implicit
        AllocaInst *value1 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value2 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value3 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value4 = builder.CreateAlloca(builder.getInt32Ty());
        AllocaInst *value5 = builder.CreateAlloca(builder.getInt32Ty());
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value1));
        builder.CreateBr(BB6);

        // 6
        builder.SetInsertPoint(BB6);
        auto *value7 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value8 = builder.CreateICmpSLT(value7, builder.getInt32(400));
        builder.CreateCondBr(value8, BB9, BB29);

        // 9
        builder.SetInsertPoint(BB9);
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value2));
        builder.CreateBr(BB10);

        // 10
        builder.SetInsertPoint(BB10);
        auto *value11 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value12 = builder.CreateICmpSLT(value11, builder.getInt32(600));
        builder.CreateCondBr(value12, BB13, BB25);

        // 13
        builder.SetInsertPoint(BB13);
        auto *value14 = builder.CreateCall(simRandFunc);
        auto *value15 = builder.CreateSRem(value14, builder.getInt32(2));
        auto *value16 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value17 = builder.CreateNSWMul(value16, builder.getInt32(600));
        auto *value18 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value19 = builder.CreateNSWAdd(value17, value18);
        auto *value20 = builder.CreateSExt(value19, Type::getInt64Ty(context));
        auto *value21 = builder.CreateInBoundsGEP(arrayType, current_gen, {builder.getInt64(0), value20});
        builder.CreateStore(value15, value21);
        builder.CreateBr(BB22);

        // 22
        builder.SetInsertPoint(BB22);
        auto *value23 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value2));
        auto *value24 = builder.CreateNSWAdd(value23, builder.getInt32(1));
        builder.CreateStore(value24, dyn_cast<llvm::Value>(value2));
        builder.CreateBr(BB10);

        // 25
        builder.SetInsertPoint(BB25);
        builder.CreateBr(BB26);

        // 26
        builder.SetInsertPoint(BB26);
        auto *value27 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value1));
        auto *value28 = builder.CreateNSWAdd(value27, builder.getInt32(1));
        builder.CreateStore(value28, dyn_cast<llvm::Value>(value1));
        builder.CreateBr(BB6);

        // 29
        builder.SetInsertPoint(BB29);
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value3));
        builder.CreateBr(BB30);

        // 30
        builder.SetInsertPoint(BB30);
        auto *value31 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value3));
        auto *value32 = builder.CreateICmpSLT(value31, builder.getInt32(10000));
        builder.CreateCondBr(value32, BB33, BB66);
        
        // 33
        builder.SetInsertPoint(BB33);
        builder.CreateCall(calcNextGenFunc);
        builder.CreateCall(drawGenFunc);
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value4));
        builder.CreateBr(BB34);

        // 34
        builder.SetInsertPoint(BB34);
        auto *value35 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value36 = builder.CreateICmpSLT(value35, builder.getInt32(600));
        builder.CreateCondBr(value36, BB37, BB62);


        // 37
        builder.SetInsertPoint(BB37);
        builder.CreateStore(builder.getInt32(0), dyn_cast<llvm::Value>(value5));
        builder.CreateBr(BB38);

        // 38
        builder.SetInsertPoint(BB38);
        auto *value39 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value5));
        auto *value40 = builder.CreateICmpSLT(value39, builder.getInt32(400));
        builder.CreateCondBr(value40, BB41, BB58);

        // 41
        builder.SetInsertPoint(BB41);
        auto *value42 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value5));
        auto *value43 = builder.CreateNSWMul(value42, builder.getInt32(600));
        auto *value44 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value45 = builder.CreateNSWAdd(value43, value44);
        auto *value46 = builder.CreateSExt(value45, Type::getInt64Ty(context));
        auto *value47 = builder.CreateInBoundsGEP(arrayType, next_gen, {builder.getInt64(0), value46});
        auto *value48 = builder.CreateLoad(builder.getInt32Ty(), value47);
        auto *value49 = builder.CreateLoad(builder.getInt32Ty(), value5);
        auto *value50 = builder.CreateNSWMul(value49, builder.getInt32(600));
        auto *value51 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value52 = builder.CreateNSWAdd(value50, value51);
        auto *value53 = builder.CreateSExt(value52, Type::getInt64Ty(context));
        auto *value54 = builder.CreateInBoundsGEP(arrayType, current_gen, {builder.getInt64(0), value53});
        builder.CreateStore(value48, value54);
        builder.CreateBr(BB55);
    
        // 55
        builder.SetInsertPoint(BB55);
        auto *value56 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value5));
        auto *value57 = builder.CreateNSWAdd(value56, builder.getInt32(1));
        builder.CreateStore(value57, dyn_cast<llvm::Value>(value5));
        builder.CreateBr(BB38);

        // 58
        builder.SetInsertPoint(BB58);
        builder.CreateBr(BB59);

        // 59
        builder.SetInsertPoint(BB59);
        auto *value60 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value4));
        auto *value61 = builder.CreateNSWAdd(value60, builder.getInt32(1));
        builder.CreateStore(value61, dyn_cast<llvm::Value>(value4));
        builder.CreateBr(BB34);

        // 62
        builder.SetInsertPoint(BB62);
        builder.CreateBr(BB63);

        // 63
        builder.SetInsertPoint(BB63);
        auto *value64 = builder.CreateLoad(builder.getInt32Ty(), dyn_cast<llvm::Value>(value3));
        auto *value65 = builder.CreateNSWAdd(value64, builder.getInt32(1));
        builder.CreateStore(value65, dyn_cast<llvm::Value>(value3));
        builder.CreateBr(BB30);

        // 66
        builder.SetInsertPoint(BB66);
        builder.CreateRetVoid();    
    }
        

// --------------------------------------------------------------------------------
    // Dump LLVM IR
    module->print(outs(), nullptr);

  
    // Interpreter of LLVM IR
    outs() << "Running code...\n";
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
    ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
    			if (fnName == "_Z11SimPutPixeliii") {
    				return reinterpret_cast<void *>(SimPutPixel);
    			}
    			if (fnName == "_Z8simFlushv") {
    				return reinterpret_cast<void *>(SimFlush);
    			}
    			if (fnName == "_Z7simRandv") {
    				return reinterpret_cast<void *>(SimRand);
    			}
    			return nullptr;
    		});
    ee->finalizeObject();

    simInit();

    ArrayRef<GenericValue> noargs;
    GenericValue v = ee->runFunction(appFunc, noargs);
    outs() << "Code was run.\n";
    
    simExit();
    return 0;
}