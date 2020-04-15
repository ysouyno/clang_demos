#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/Module.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Parse/ParseAST.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

class MyASTConsumer : public clang::ASTConsumer {
public:
  MyASTConsumer(clang::Rewriter &R) : the_rewriter(R) {}

private:
  clang::Rewriter the_rewriter;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    llvm::errs() << "Usage: this_executable_file <filename>\n";
    return 1;
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

  ci.createFileManager();
  clang::FileManager &file_mgr = ci.getFileManager();
  ci.createSourceManager(file_mgr);
  clang::SourceManager &source_mgr = ci.getSourceManager();
  ci.createPreprocessor(clang::TU_Module);
  ci.createASTContext();

  clang::Rewriter the_rewriter;
  the_rewriter.setSourceMgr(source_mgr, ci.getLangOpts());

  const clang::FileEntry *file_in = file_mgr.getFile(argv[1]);
  source_mgr.setMainFileID(source_mgr.createFileID(
      file_in, clang::SourceLocation(), clang::SrcMgr::C_User));
  ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(),
                                           &ci.getPreprocessor());

  MyASTConsumer the_consumer(the_rewriter);

  clang::ParseAST(ci.getPreprocessor(), &the_consumer, ci.getASTContext());

  return 0;
}
