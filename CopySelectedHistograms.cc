#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TKey.h"
#include "TClass.h"

void CopySelectedHistograms(const char* inputFilename, const char* outputFilename, const std::vector<std::string>& histNames) {
    // 원본 파일 열기
    TFile *inputFile = TFile::Open(inputFilename, "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: cannot open file " << inputFilename << std::endl;
        return;
    }

    // 새 파일 만들기
    TFile *outputFile = TFile::Open(outputFilename, "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: cannot create file " << outputFilename << std::endl;
        inputFile->Close();
        return;
    }

    // 원본 파일에서 TKey를 통해 객체를 읽음
    TIter next(inputFile->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
        TClass *cl = gROOT->GetClass(key->GetClassName());
        if (!cl->InheritsFrom("TH1")) continue; // TH1 클래스가 아니면 스킵

        // 히스토그램 이름 가져오기
        std::string histName = key->GetName();
        
        // 선택된 히스토그램 이름 리스트에 있는지 확인
        if (std::find(histNames.begin(), histNames.end(), histName) != histNames.end()) {
            // 히스토그램 가져오기
            TH1 *hist = (TH1*)key->ReadObj();
            outputFile->cd(); // 새로운 파일로 이동
            hist->Write();    // 히스토그램 저장
        }
    }

    // 파일 닫기
    inputFile->Close();
    outputFile->Close();

    std::cout << "Selected histograms copied to " << outputFilename << std::endl;
}

