#ifndef X86TARGETMACHINE_H
#define X86TARGETMACHINE_H

#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetData.h>
#include <llvm/Target/TargetFrameInfo.h>
//#include <llvm/Target/TargetAsmInfo.h>
#include "X86Subtarget.h"
#include "X86InstrInfo.h"

namespace llvm {

class TargetAsmInfo;

class X86TargetMachine : public LLVMTargetMachine {
  const TargetData DataLayout;       // Calculates type size & alignment
  X86Subtarget Subtarget;
  //X86InstrInfo InstrInfo;
  TargetFrameInfo FrameInfo;
  
protected:
  virtual const TargetAsmInfo *createTargetAsmInfo() const;
  
public:
  X86TargetMachine(const Module &M, const std::string &FS);

//  virtual const X86InstrInfo *getInstrInfo() const {return &InstrInfo; }
  virtual const TargetFrameInfo *getFrameInfo() const {return &FrameInfo; }
//  virtual const TargetSubtarget *getSubtargetImpl() const{return &Subtarget; }
//  virtual const TargetRegisterInfo *getRegisterInfo() const {
//    return &InstrInfo.getRegisterInfo();
//  }
  virtual const TargetData *getTargetData() const { return &DataLayout; }
  static unsigned getModuleMatchQuality(const Module &M);

  // Pass Pipeline Configuration
  virtual bool addInstSelector(PassManagerBase &PM, bool Fast);
  virtual bool addPreEmitPass(PassManagerBase &PM, bool Fast);
};

}

#endif //X86TARGETMACHINE_H
