#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TRint.h"
#include "TCanvas.h"
#include "stdio.h"
#include "TLegend.h"
#include "TString.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "stdlib.h"
#include "Fit/BinData.h"

using namespace std;
using namespace ROOT::Fit;

int make_hist(TString width) {
    TString dir = "/cms/ldap_home/seungjun/plotnow/";

    TString hist_name = "emu_TwoBTag_TwoJet_mlb_minimax";
    TString width_name = dir + "combined_output_" + width + ".root";
    
    TFile *f080 = TFile::Open(width_name, "READ");
    if (!f080 || f080->IsZombie()) {
        cerr << "Error: Could not open file " << width_name << endl;
        return 1;
    }

    TH1F *hist_080 = (TH1F*)f080->Get(hist_name);
    if (!hist_080) {
        cerr << "Error: Could not find histogram " << hist_name << " in file " << width_name << endl;
        f080->Close();
        return 1;
    }

    hist_080->SetDirectory(0);
    f080->Close(); // Close the file after histogram retrieval

    TString hist_name2 = "hblmass0_" + width;
    hist_080->SetName(hist_name2);

    TFile *f130 = TFile::Open(dir + "combined_output_130.root", "READ");
    if (!f130 || f130->IsZombie()) {
        cerr << "Error: Could not open file combined_output_130.root" << endl;
        delete hist_080;
        return 1;
    }

    TH1F *hist_130 = (TH1F*)f130->Get(hist_name);
    if (!hist_130) {
        cerr << "Error: Could not find histogram " << hist_name << " in file combined_output_130.root" << endl;
        f130->Close();
        delete hist_080;
        return 1;
    }
    TH1F *newHist = new TH1F("newHist", "Rebinned Histogram", 15, 0, 500);
    newHist->SetName(hist_name2);
	for (int i = 1; i <= hist_080->GetEntries(); i++) {
	//for (int i = 1; i <= hist_080->GetNbinsX(); i++) {
	    double binContent = hist_080->GetBinContent(i);
	    double binCenter =  hist_080->GetBinCenter(i);
	    int newBin = newHist->FindBin(binCenter);
	    newHist->AddBinContent(newBin, binContent);
	}
	newHist->Draw();
    newHist->Scale(hist_130->GetEntries() / hist_080->GetEntries());
    //hist_080->Scale(hist_130->GetEntries() / hist_080->GetEntries());

    TFile *file = TFile::Open("root_file/template_file.root", "UPDATE");
    if (!file || file->IsZombie()) {
        cerr << "Error: Could not open file root_file/template_file.root" << endl;
        delete hist_080;
        f130->Close();
        return 1;
    }

    //hist_080->Write();
    newHist->Write();
    file->Close();

    f130->Close();

    delete hist_080; // Clean up memory

    return 0;
}

int main() {
    TString width[5] = {"80", "100", "130", "160", "180"};

    TFile *file = TFile::Open("root_file/template_file.root", "RECREATE");
    file->Close();

    for (int i = 0; i < 5; i++) {
        make_hist(width[i]);
    }

    TFile *file_basic = TFile::Open("/cms/ldap_home/seungjun/plotnow/combined_output_130.root", "READ");
    if (!file_basic || file_basic->IsZombie()) {
        cerr << "Error: Could not open file combined_output_130.root" << endl;
        return 1;
    }

    TH1F *basic = (TH1F*)file_basic->Get("emu_TwoBTag_TwoJet_mlb_minimax");
    if (!basic) {
        cerr << "Error: Could not find histogram emu_TwoBTag_TwoJet_mlb_minimax in file combined_output_130.root" << endl;
        file_basic->Close();
        return 1;
    }

    TFile *file_poisson = TFile::Open("root_file/poisson_viking.root", "RECREATE");
    if (!file_poisson || file_poisson->IsZombie()) {
        cerr << "Error: Could not open file root_file/poisson_viking.root" << endl;
        file_basic->Close();
        return 1;
    }

    double count = basic->GetEntries();
    double bin = count;
    int bins = 15;

    TRandom3 rd;
    rd.SetSeed(0);

    for (int i = 0; i < 1000; i++) {
        int ran1 = rd.Poisson(bin);
        TString hist_name = "hist_" + TString::Itoa(i, 10);
        TString hist_title = "Template Method " + TString::Itoa(i, 10) + " : 1.32  GeV";

        TH1F *hist_random = new TH1F(hist_name, hist_title, bins, 0, 500);
        for (int k = 0; k < ran1; k++) {
            double mlb = basic->GetRandom();
            hist_random->Fill(mlb);
        }

        file_poisson->cd();
        hist_random->Write();

        delete hist_random; // Clean up memory
    }

    file_poisson->Close();
    file_basic->Close();

    return 0;
}

