#include <clang/Basic/LangOptions.h>
#include <clang/Basic/Module.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    llvm::errs() << "Usage: this_executable_file <filename>\n";
  }

  clang::CompilerInstance ci;
  ci.createDiagnostics();

  clang::LangOptions &lo = ci.getLangOpts();
  lo.CPlusPlus = 1;

  auto to = std::make_shared<clang::TargetOptions>();
  to->Triple = llvm::sys::getDefaultTargetTriple();
  clang::TargetInfo *ti =
      clang::TargetInfo::CreateTargetInfo(ci.getDiagnostics(), to);
  ci.setTarget(ti);
}
