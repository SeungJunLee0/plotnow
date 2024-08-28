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


int make_hist(TString width){

	
	TString dir	= "/cms/ldap_home/seungjun/plotnow/";


    TString hist_name = "emu_TwoBTag_TwoJet_mlb_minimax";

    TString width_name = dir+"combined_output_"+width+".root";//powheg met cut in emu channel
	
	TFile *f080 = new TFile(width_name,"read");
    TH1F *hist_080 =  (TH1F*) f080 -> Get(hist_name);
	cout<<hist_080->GetNbinsX()<<endl;
	hist_080->SetDirectory(0);

	TString hist_name2 = "hblmass0_"+width;

    hist_080->SetName(hist_name2);
	double count = hist_080->GetEntries();
	cout << width << " " <<count <<endl;
	TFile *f130 = new TFile("/cms/ldap_home/seungjun/plotnow/combined_output_130.root","read");
    TH1F *hist_130 =  (TH1F*) f130 -> Get(hist_name);
	//double count1 = bin =  9.0e4;
	//hist_080->Scale((double)hist_130->GetEntries()/(double)hist_080->GetEntries());
	cout << (double)hist_130->GetEntries()/(double)hist_080->GetEntries()<<endl;

    TFile *file =new TFile("root_file/template_file.root","update");
    hist_080->Write(); 
	file->Close();
	f130->Close();
    f080->Close();
	return 0;

}



int main()
{
	int a=0;
    TString width[5] = {"80",//  0
						 "100",//  2
						 "130",
						 "160",//  8
						 "180" //  10
	};
    
    TCanvas* c = new TCanvas("c", "Something", 0, 0, 800, 600);


    TFile *file =new TFile("root_file/template_file.root","recreate");
	file->Close();
    
	for(int i =0;i<5;i++){
		TString width_n= width[i];
		make_hist(width_n);
	}

    TFile *file_basic =new TFile("/cms/ldap_home/seungjun/plotnow/combined_output_130.root","read");
	TH1F *basic = (TH1F *) file_basic->Get("emu_TwoBTag_TwoJet_mlb_minimax"); 
    TFile *file_poisson =new TFile("root_file/poisson_viking.root","recreate");
	double count = basic->GetEntries();
    double bin = count;
	bin = 9.0e4;
	int bins=15;

	TRandom3 *rd = new TRandom3;
    rd->SetSeed(0);

    TString hist_name = "Invariant_Mass";

    for(int i=0; i <1000; i++){
    	int ran1 = rd->Poisson(bin);
		TH1F *exp = (TH1F*)basic ->Clone();
     	hist_name = "hist_"+ TString::Itoa(i,10); 
     	TString hist_name2 = "Template Method "+ TString::Itoa(i,10)+" : 1.32  GeV"; 

     	TH1F* hist_random = new TH1F(hist_name,hist_name2,bins,0,500);
		double mlb = 0;
		for(int k =0 ; k<ran1;k++){
			 mlb = exp->GetRandom();
			hist_random->Fill(mlb);
		}
	    //cout<<"new"<<hist_random->GetNbinsX()<<endl;
		file_poisson->cd();
     	hist_random->Write();
    }

    file_poisson->Close(); 
    //c->Modified(); c->Update();

    //app.Run();

    return 0;
}


